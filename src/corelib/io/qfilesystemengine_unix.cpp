/***********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2012-2014 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or 
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see 
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#include "qplatformdefs.h"
#include "qfilesystemengine_p.h"
#include "qplatformdefs.h"
#include "qfsfileengine.h"
#include "qfile.h"
#include "qfileinfo.h"

#include <QtCore/qvarlengtharray.h>

#include <stdlib.h> // for realpath()
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#if defined(Q_OS_MAC)
# include <qcore_mac_p.h>
#endif

#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
# include <CoreFoundation/CFBundle.h>
#endif

QT_BEGIN_NAMESPACE

#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
static inline bool _q_isMacHidden(const char *nativePath)
{
    OSErr err;

    FSRef fsRef;
    err = FSPathMakeRefWithOptions(reinterpret_cast<const UInt8 *>(nativePath),
            kFSPathMakeRefDoNotFollowLeafSymlink, &fsRef, 0);
    if (err != noErr)
        return false;

    FSCatalogInfo catInfo;
    err = FSGetCatalogInfo(&fsRef, kFSCatInfoFinderInfo, &catInfo, NULL, NULL, NULL);
    if (err != noErr)
        return false;

    FileInfo * const fileInfo = reinterpret_cast<FileInfo*>(&catInfo.finderInfo);
    return (fileInfo->finderFlags & kIsInvisible);
}

static bool isPackage(const QFileSystemMetaData &data, const QFileSystemEntry &entry)
{
    if (!data.isDirectory())
        return false;

    QFileInfo info(entry.filePath());
    QString suffix = info.suffix();

    if (suffix.length() > 0) {
        // First step: is the extension known ?
        CFStringRef extensionRef = QCFString::toCFStringRef(suffix);
        CFStringRef uniformTypeIdentifier = UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, extensionRef, NULL);
        if (UTTypeConformsTo(uniformTypeIdentifier, kUTTypeBundle))
            return true;

        // Second step: check if an application knows the package type
        CFStringRef path = QCFString::toCFStringRef(entry.filePath());
        QCFType<CFURLRef> url = CFURLCreateWithFileSystemPath(0, path, kCFURLPOSIXPathStyle, true);

        UInt32 type, creator;
        // Well created packages have the PkgInfo file
        if (CFBundleGetPackageInfoInDirectory(url, &type, &creator))
            return true;

        // Find if an application other than Finder claims to know how to handle the package
        QCFType<CFURLRef> application;
        LSGetApplicationForURL(url,
                               kLSRolesEditor|kLSRolesViewer|kLSRolesViewer,
                               NULL,
                               &application);

        if (application) {
            QCFType<CFBundleRef> bundle = CFBundleCreate(kCFAllocatorDefault, application);
            CFStringRef identifier = CFBundleGetIdentifier(bundle);
            QString applicationId = QCFString::toQString(identifier);
            if (applicationId != QLatin1String("com.apple.finder"))
                return true;
        }
    }

    // Third step: check if the directory has the package bit set
    FSRef packageRef;
    FSPathMakeRef((UInt8 *)entry.nativeFilePath().constData(), &packageRef, NULL);

    FSCatalogInfo catalogInfo;
    FSGetCatalogInfo(&packageRef,
                     kFSCatInfoFinderInfo,
                     &catalogInfo,
                     NULL,
                     NULL,
                     NULL);

    FolderInfo *folderInfo = reinterpret_cast<FolderInfo *>(catalogInfo.finderInfo);
    return folderInfo->finderFlags & kHasBundle;
}

#else
static inline bool _q_isMacHidden(const char *nativePath)
{
    Q_UNUSED(nativePath);
    // no-op
    return false;
}
#endif

bool QFileSystemEngine::isCaseSensitive()
{
    return true;
}

//static
QFileSystemEntry QFileSystemEngine::getLinkTarget(const QFileSystemEntry &link, QFileSystemMetaData &data)
{
#if defined(__GLIBC__) && !defined(PATH_MAX)
#define PATH_CHUNK_SIZE 256
    char *s = 0;
    int len = -1;
    int size = PATH_CHUNK_SIZE;

    while (1) {
        s = (char *) ::realloc(s, size);
        Q_CHECK_PTR(s);
        len = ::readlink(link.nativeFilePath().constData(), s, size);
        if (len < 0) {
            ::free(s);
            break;
        }
        if (len < size) {
            break;
        }
        size *= 2;
    }
#else
    char s[PATH_MAX+1];
    int len = readlink(link.nativeFilePath().constData(), s, PATH_MAX);
#endif
    if (len > 0) {
        QString ret;
        if (!data.hasFlags(QFileSystemMetaData::DirectoryType))
            fillMetaData(link, data, QFileSystemMetaData::DirectoryType);
        if (data.isDirectory() && s[0] != '/') {
            QDir parent(link.filePath());
            parent.cdUp();
            ret = parent.path();
            if (!ret.isEmpty() && !ret.endsWith(QLatin1Char('/')))
                ret += QLatin1Char('/');
        }
        s[len] = '\0';
        ret += QFile::decodeName(QByteArray(s));
#if defined(__GLIBC__) && !defined(PATH_MAX)
        ::free(s);
#endif

        if (!ret.startsWith(QLatin1Char('/'))) {
            if (link.filePath().startsWith(QLatin1Char('/'))) {
                ret.prepend(link.filePath().left(link.filePath().lastIndexOf(QLatin1Char('/')))
                            + QLatin1Char('/'));
            } else {
                ret.prepend(QDir::currentPath() + QLatin1Char('/'));
            }
        }
        ret = QDir::cleanPath(ret);
        if (ret.size() > 1 && ret.endsWith(QLatin1Char('/')))
            ret.chop(1);
        return QFileSystemEntry(ret);
    }
#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
    {
        FSRef fref;
        if (FSPathMakeRef((const UInt8 *)QFile::encodeName(QDir::cleanPath(link.filePath())).data(), &fref, 0) == noErr) {
            // TODO get the meta data info from the QFileSystemMetaData object
            Boolean isAlias, isFolder;
            if (FSResolveAliasFile(&fref, true, &isFolder, &isAlias) == noErr && isAlias) {
                AliasHandle alias;
                if (FSNewAlias(0, &fref, &alias) == noErr && alias) {
                    QCFString cfstr;
                    if (FSCopyAliasInfo(alias, 0, 0, &cfstr, 0, 0) == noErr)
                        return QFileSystemEntry(QCFString::toQString(cfstr));
                }
            }
        }
    }
#endif
    return QFileSystemEntry();
}

//static
QFileSystemEntry QFileSystemEngine::canonicalName(const QFileSystemEntry &entry, QFileSystemMetaData &data)
{
    if (entry.isEmpty() || entry.isRoot())
        return entry;

#if !defined(Q_OS_MAC) && _POSIX_VERSION < 200809L
    // realpath(X,0) is not supported
    Q_UNUSED(data);
    return QFileSystemEntry(slowCanonicalized(absoluteName(entry).filePath()));
#else
    char *ret = 0;
# if defined(Q_OS_MAC) && !defined(Q_OS_IOS)
    // When using -mmacosx-version-min=10.4, we get the legacy realpath implementation,
    // which does not work properly with the realpath(X,0) form. See QTBUG-28282.
    if (QSysInfo::MacintoshVersion >= QSysInfo::MV_10_6) {
        ret = (char*)malloc(PATH_MAX + 1);
        if (ret && realpath(entry.nativeFilePath().constData(), (char*)ret) == 0) {
            const int savedErrno = errno; // errno is checked below, and free() might change it
            free(ret);
            errno = savedErrno;
            ret = 0;
        }
    } else {
        // on 10.5 we can use FSRef to resolve the file path.
        QString path = QDir::cleanPath(entry.filePath());
        FSRef fsref;
        if (FSPathMakeRef((const UInt8 *)path.toUtf8().data(), &fsref, 0) == noErr) {
            CFURLRef urlref = CFURLCreateFromFSRef(NULL, &fsref);
            CFStringRef canonicalPath = CFURLCopyFileSystemPath(urlref, kCFURLPOSIXPathStyle);
            QString ret = QCFString::toQString(canonicalPath);
            CFRelease(canonicalPath);
            CFRelease(urlref);
            return QFileSystemEntry(ret);
        }
    }
# else
#  if _POSIX_VERSION >= 200801L
    ret = realpath(entry.nativeFilePath().constData(), (char*)0);
#  else
    ret = (char*)malloc(PATH_MAX + 1);
    if (realpath(entry.nativeFilePath().constData(), (char*)ret) == 0) {
        const int savedErrno = errno; // errno is checked below, and free() might change it
        free(ret);
        errno = savedErrno;
        ret = 0;
    }
#  endif
# endif
    if (ret) {
        data.knownFlagsMask |= QFileSystemMetaData::ExistsAttribute;
        data.entryFlags |= QFileSystemMetaData::ExistsAttribute;
        QString canonicalPath = QDir::cleanPath(QString::fromLocal8Bit(ret));
        free(ret);
        return QFileSystemEntry(canonicalPath);
    } else if (errno == ENOENT) { // file doesn't exist
        data.knownFlagsMask |= QFileSystemMetaData::ExistsAttribute;
        data.entryFlags &= ~(QFileSystemMetaData::ExistsAttribute);
        return QFileSystemEntry();
    }
    return entry;
#endif
}

//static
QFileSystemEntry QFileSystemEngine::absoluteName(const QFileSystemEntry &entry)
{
    if (entry.isAbsolute() && entry.isClean())
        return entry;

    QByteArray orig = entry.nativeFilePath();
    QByteArray result;
    if (orig.isEmpty() || !orig.startsWith('/')) {
        QFileSystemEntry cur(currentPath());
        result = cur.nativeFilePath();
    }
    if (!orig.isEmpty() && !(orig.length() == 1 && orig[0] == '.')) {
        if (!result.isEmpty() && !result.endsWith('/'))
            result.append('/');
        result.append(orig);
    }

    if (result.length() == 1 && result[0] == '/')
        return QFileSystemEntry(result, QFileSystemEntry::FromNativePath());
    const bool isDir = result.endsWith('/');

    /* as long as QDir::cleanPath() operates on a QString we have to convert to a string here.
     * ideally we never convert to a string since that loses information. Please fix after
     * we get a QByteArray version of QDir::cleanPath()
     */
    QFileSystemEntry resultingEntry(result, QFileSystemEntry::FromNativePath());
    QString stringVersion = QDir::cleanPath(resultingEntry.filePath());
    if (isDir)
        stringVersion.append(QLatin1Char('/'));
    return QFileSystemEntry(stringVersion);
}

//static
QByteArray QFileSystemEngine::id(const QFileSystemEntry &entry)
{
    struct stat statResult;
    if (stat(entry.nativeFilePath().constData(), &statResult)) {
        qErrnoWarning("stat() failed for '%s'", entry.nativeFilePath().constData());
        return QByteArray();
    }
    QByteArray result = QByteArray::number(quint64(statResult.st_dev), 16);
    result += ':';
    result += QByteArray::number(quint64(statResult.st_ino), 16);
    return result;
}

//static
QString QFileSystemEngine::resolveUserName(uint userId)
{
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
    int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    QVarLengthArray<char, 1024> buf(size_max);
#endif

    struct passwd *pw = 0;

#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
    struct passwd entry;
    getpwuid_r(userId, &entry, buf.data(), buf.size(), &pw);
#else
    pw = getpwuid(userId);
#endif


    if (pw)
        return QFile::decodeName(QByteArray(pw->pw_name));
    return QString();
}

//static
QString QFileSystemEngine::resolveGroupName(uint groupId)
{
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
    int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    QVarLengthArray<char, 1024> buf(size_max);
#endif

    struct group *gr = 0;

#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
    size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    buf.resize(size_max);
    struct group entry;
    // Some large systems have more members than the POSIX max size
    // Loop over by doubling the buffer size (upper limit 250k)
    for (unsigned size = size_max; size < 256000; size += size)
    {
        buf.resize(size);
        // ERANGE indicates that the buffer was too small
        if (!getgrgid_r(groupId, &entry, buf.data(), buf.size(), &gr)
            || errno != ERANGE)
            break;
    }
#else
    gr = getgrgid(groupId);
#endif

    if (gr)
        return QFile::decodeName(QByteArray(gr->gr_name));
    return QString();
}

#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
//static
QString QFileSystemEngine::bundleName(const QFileSystemEntry &entry)
{
    QCFType<CFURLRef> url = CFURLCreateWithFileSystemPath(0, QCFString(entry.filePath()),
            kCFURLPOSIXPathStyle, true);
    if (QCFType<CFDictionaryRef> dict = CFBundleCopyInfoDictionaryForURL(url)) {
        if (CFTypeRef name = (CFTypeRef)CFDictionaryGetValue(dict, kCFBundleNameKey)) {
            if (CFGetTypeID(name) == CFStringGetTypeID())
                return QCFString::toQString((CFStringRef)name);
        }
    }
    return QString();
}
#endif

//static
bool QFileSystemEngine::fillMetaData(const QFileSystemEntry &entry, QFileSystemMetaData &data,
        QFileSystemMetaData::MetaDataFlags what)
{
#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
    if (what & QFileSystemMetaData::BundleType) {
        if (!data.hasFlags(QFileSystemMetaData::DirectoryType))
            what |= QFileSystemMetaData::DirectoryType;
    }
#endif

#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC) \
        && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
    if (what & QFileSystemMetaData::HiddenAttribute) {
        // Mac OS >= 10.5: st_flags & UF_HIDDEN
        what |= QFileSystemMetaData::PosixStatFlags;
    }
#endif

    if (what & QFileSystemMetaData::PosixStatFlags)
        what |= QFileSystemMetaData::PosixStatFlags;

    if (what & QFileSystemMetaData::ExistsAttribute) {
        //  FIXME:  Would other queries being performed provide this bit?
        what |= QFileSystemMetaData::PosixStatFlags;
    }

    data.entryFlags &= ~what;

    const char * nativeFilePath;
    int nativeFilePathLength;
    {
        const QByteArray &path = entry.nativeFilePath();
        nativeFilePath = path.constData();
        nativeFilePathLength = path.size();
        Q_UNUSED(nativeFilePathLength);
    }

    bool entryExists = true; // innocent until proven otherwise

    QT_STATBUF statBuffer;
    bool statBufferValid = false;
    if (what & QFileSystemMetaData::LinkType) {
        if (QT_LSTAT(nativeFilePath, &statBuffer) == 0) {
            if (S_ISLNK(statBuffer.st_mode)) {
                data.entryFlags |= QFileSystemMetaData::LinkType;
            } else {
                statBufferValid = true;
                data.entryFlags &= ~QFileSystemMetaData::PosixStatFlags;
            }
        } else {
            entryExists = false;
        }

        data.knownFlagsMask |= QFileSystemMetaData::LinkType;
    }

    if (statBufferValid || (what & QFileSystemMetaData::PosixStatFlags)) {
        if (entryExists && !statBufferValid)
            statBufferValid = (QT_STAT(nativeFilePath, &statBuffer) == 0);

        if (statBufferValid)
            data.fillFromStatBuf(statBuffer);
        else {
            entryExists = false;
            data.creationTime_ = 0;
            data.modificationTime_ = 0;
            data.accessTime_ = 0;
            data.size_ = 0;
            data.userId_ = (uint) -2;
            data.groupId_ = (uint) -2;
        }

        // reset the mask
        data.knownFlagsMask |= QFileSystemMetaData::PosixStatFlags
            | QFileSystemMetaData::ExistsAttribute;
    }

#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
    if (what & QFileSystemMetaData::AliasType)
    {
        if (entryExists) {
            FSRef fref;
            if (FSPathMakeRef((const UInt8 *)nativeFilePath, &fref, NULL) == noErr) {
                Boolean isAlias, isFolder;
                if (FSIsAliasFile(&fref, &isAlias, &isFolder) == noErr) {
                    if (isAlias)
                        data.entryFlags |= QFileSystemMetaData::AliasType;
                }
            }
        }
        data.knownFlagsMask |= QFileSystemMetaData::AliasType;
    }
#endif

    if (what & QFileSystemMetaData::UserPermissions) {
        // calculate user permissions

        if (entryExists) {
            if (what & QFileSystemMetaData::UserReadPermission) {
                if (QT_ACCESS(nativeFilePath, R_OK) == 0)
                    data.entryFlags |= QFileSystemMetaData::UserReadPermission;
            }
            if (what & QFileSystemMetaData::UserWritePermission) {
                if (QT_ACCESS(nativeFilePath, W_OK) == 0)
                    data.entryFlags |= QFileSystemMetaData::UserWritePermission;
            }
            if (what & QFileSystemMetaData::UserExecutePermission) {
                if (QT_ACCESS(nativeFilePath, X_OK) == 0)
                    data.entryFlags |= QFileSystemMetaData::UserExecutePermission;
            }
        }
        data.knownFlagsMask |= (what & QFileSystemMetaData::UserPermissions);
    }

    if (what & QFileSystemMetaData::HiddenAttribute
            && !data.isHidden()) {
        QString fileName = entry.fileName();
        if ((fileName.size() > 0 && fileName.at(0) == QLatin1Char('.'))
                || (entryExists && _q_isMacHidden(nativeFilePath)))
            data.entryFlags |= QFileSystemMetaData::HiddenAttribute;
        data.knownFlagsMask |= QFileSystemMetaData::HiddenAttribute;
    }

#if !defined(QWS) && !defined(Q_WS_QPA) && defined(Q_OS_MAC)
    if (what & QFileSystemMetaData::BundleType) {
        if (entryExists && isPackage(data, entry))
            data.entryFlags |= QFileSystemMetaData::BundleType;
        data.knownFlagsMask |= QFileSystemMetaData::BundleType;
    }
#endif

    return data.hasFlags(what);
}

//static
bool QFileSystemEngine::createDirectory(const QFileSystemEntry &entry, bool createParents)
{
    QString dirName = entry.filePath();
    if (createParents) {
        dirName = QDir::cleanPath(dirName);
        for (int oldslash = -1, slash=0; slash != -1; oldslash = slash) {
            slash = dirName.indexOf(QDir::separator(), oldslash+1);
            if (slash == -1) {
                if (oldslash == dirName.length())
                    break;
                slash = dirName.length();
            }
            if (slash) {
                const QByteArray chunk = QFile::encodeName(dirName.left(slash));
                if (QT_MKDIR(chunk.constData(), 0777) != 0) {
                    if (errno == EEXIST) {
                        QT_STATBUF st;
                        if (QT_STAT(chunk.constData(), &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR)
                            continue;
                    }
                    return false;
                }
            }
        }
        return true;
    }
#if defined(Q_OS_DARWIN)  // Mac X doesn't support trailing /'s
    if (dirName.endsWith(QLatin1Char('/')))
        dirName.chop(1);
#endif
    return (QT_MKDIR(QFile::encodeName(dirName).constData(), 0777) == 0);
}

//static
bool QFileSystemEngine::removeDirectory(const QFileSystemEntry &entry, bool removeEmptyParents)
{
    if (removeEmptyParents) {
        QString dirName = QDir::cleanPath(entry.filePath());
        for (int oldslash = 0, slash=dirName.length(); slash > 0; oldslash = slash) {
            const QByteArray chunk = QFile::encodeName(dirName.left(slash));
            QT_STATBUF st;
            if (QT_STAT(chunk.constData(), &st) != -1) {
                if ((st.st_mode & S_IFMT) != S_IFDIR)
                    return false;
                if (::rmdir(chunk.constData()) != 0)
                    return oldslash != 0;
            } else {
                return false;
            }
            slash = dirName.lastIndexOf(QDir::separator(), oldslash-1);
        }
        return true;
    }
    return rmdir(QFile::encodeName(entry.filePath()).constData()) == 0;
}

//static
bool QFileSystemEngine::createLink(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error)
{
    if (::symlink(source.nativeFilePath().constData(), target.nativeFilePath().constData()) == 0)
        return true;
    error = QSystemError(errno, QSystemError::StandardLibraryError);
    return false;
}

//static
bool QFileSystemEngine::copyFile(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error)
{
    Q_UNUSED(source);
    Q_UNUSED(target);
    error = QSystemError(ENOSYS, QSystemError::StandardLibraryError); //Function not implemented
    return false;
}

//static
bool QFileSystemEngine::renameFile(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error)
{
    if (::rename(source.nativeFilePath().constData(), target.nativeFilePath().constData()) == 0)
        return true;
    error = QSystemError(errno, QSystemError::StandardLibraryError);
    return false;
}

//static
bool QFileSystemEngine::removeFile(const QFileSystemEntry &entry, QSystemError &error)
{
    if (unlink(entry.nativeFilePath().constData()) == 0)
        return true;
    error = QSystemError(errno, QSystemError::StandardLibraryError);
    return false;

}

//static
bool QFileSystemEngine::setPermissions(const QFileSystemEntry &entry, QFile::Permissions permissions, QSystemError &error, QFileSystemMetaData *data)
{
    mode_t mode = 0;
    if (permissions & QFile::ReadOwner)
        mode |= S_IRUSR;
    if (permissions & QFile::WriteOwner)
        mode |= S_IWUSR;
    if (permissions & QFile::ExeOwner)
        mode |= S_IXUSR;
    if (permissions & QFile::ReadUser)
        mode |= S_IRUSR;
    if (permissions & QFile::WriteUser)
        mode |= S_IWUSR;
    if (permissions & QFile::ExeUser)
        mode |= S_IXUSR;
    if (permissions & QFile::ReadGroup)
        mode |= S_IRGRP;
    if (permissions & QFile::WriteGroup)
        mode |= S_IWGRP;
    if (permissions & QFile::ExeGroup)
        mode |= S_IXGRP;
    if (permissions & QFile::ReadOther)
        mode |= S_IROTH;
    if (permissions & QFile::WriteOther)
        mode |= S_IWOTH;
    if (permissions & QFile::ExeOther)
        mode |= S_IXOTH;

    bool success = ::chmod(entry.nativeFilePath().constData(), mode) == 0;
    if (success && data) {
        data->entryFlags &= ~QFileSystemMetaData::Permissions;
        data->entryFlags |= QFileSystemMetaData::MetaDataFlag(uint(permissions));
        data->knownFlagsMask |= QFileSystemMetaData::Permissions;
    }
    if (!success)
        error = QSystemError(errno, QSystemError::StandardLibraryError);
    return success;
}

QString QFileSystemEngine::homePath()
{
    QString home = QFile::decodeName(qgetenv("HOME"));
    if (home.isEmpty())
        home = rootPath();
    return QDir::cleanPath(home);
}

QString QFileSystemEngine::rootPath()
{
    return QLatin1String("/");
}

QString QFileSystemEngine::tempPath()
{
#ifdef QT_UNIX_TEMP_PATH_OVERRIDE
    return QLatin1String(QT_UNIX_TEMP_PATH_OVERRIDE);

#else
    QString temp = QFile::decodeName(qgetenv("TMPDIR"));
    if (temp.isEmpty())
        temp = QLatin1String("/tmp/");
    return QDir::cleanPath(temp);
#endif
}

bool QFileSystemEngine::setCurrentPath(const QFileSystemEntry &path)
{
    int r;
    r = QT_CHDIR(path.nativeFilePath().constData());
    return r >= 0;
}

QFileSystemEntry QFileSystemEngine::currentPath()
{
    QFileSystemEntry result;
    QT_STATBUF st;
    if (QT_STAT(".", &st) == 0) {
#if defined(__GLIBC__) && !defined(PATH_MAX)
        char *currentName = ::get_current_dir_name();
        if (currentName) {
            result = QFileSystemEntry(QByteArray(currentName), QFileSystemEntry::FromNativePath());
            ::free(currentName);
        }
#else
        char currentName[PATH_MAX+1];
        if (::getcwd(currentName, PATH_MAX))
            result = QFileSystemEntry(QByteArray(currentName), QFileSystemEntry::FromNativePath());
# if defined(QT_DEBUG)
        if (result.isEmpty())
            qWarning("QFSFileEngine::currentPath: getcwd() failed");
# endif
#endif
    } else {
# if defined(QT_DEBUG)
        qWarning("QFSFileEngine::currentPath: stat(\".\") failed");
# endif
    }
    return result;
}
QT_END_NAMESPACE
