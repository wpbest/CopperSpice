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

#define _POSIX_
#include "qplatformdefs.h"
#include "qabstractfileengine.h"
#include "qfsfileengine_p.h"
#include "qfilesystemengine_p.h"
#include <qdebug.h>

#include "qfile.h"
#include "qdir.h"
#include "qmutexpool_p.h"
#include "qvarlengtharray.h"
#include "qdatetime.h"
#include "qt_windows.h"

#  include <sys/types.h>
#  include <direct.h>
#  include <winioctl.h>

#include <objbase.h>
#include <shlobj.h>
#include <initguid.h>
#include <accctrl.h>
#include <ctype.h>
#include <limits.h>
#define SECURITY_WIN32
#include <security.h>

#ifndef PATH_MAX
#define PATH_MAX FILENAME_MAX
#endif

QT_BEGIN_NAMESPACE

static inline bool isUncPath(const QString &path)
{
    // Starts with \\, but not \\.
    return (path.startsWith(QLatin1String("\\\\"))
            && path.size() > 2 && path.at(2) != QLatin1Char('.'));
}


/*!
    \internal
*/
QString QFSFileEnginePrivate::longFileName(const QString &path)
{
    if (path.startsWith(QLatin1String("\\\\.\\")))
        return path;

    QString absPath = QFileSystemEngine::nativeAbsoluteFilePath(path);

    QString prefix = QLatin1String("\\\\?\\");
    if (isUncPath(absPath)) {
        prefix.append(QLatin1String("UNC\\")); // "\\\\?\\UNC\\"
        absPath.remove(0, 2);
    }
    return prefix + absPath;

}

/*
    \internal
*/
bool QFSFileEnginePrivate::nativeOpen(QIODevice::OpenMode openMode)
{
    Q_Q(QFSFileEngine);

    // All files are opened in share mode (both read and write).
    DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

    int accessRights = 0;
    if (openMode & QIODevice::ReadOnly)
        accessRights |= GENERIC_READ;
    if (openMode & QIODevice::WriteOnly)
        accessRights |= GENERIC_WRITE;

    SECURITY_ATTRIBUTES securityAtts = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };

    // WriteOnly can create files, ReadOnly cannot.
    DWORD creationDisp = (openMode & QIODevice::WriteOnly) ? OPEN_ALWAYS : OPEN_EXISTING;
    // Create the file handle.
    fileHandle = CreateFile((const wchar_t*)fileEntry.nativeFilePath().utf16(),
                            accessRights,
                            shareMode,
                            &securityAtts,
                            creationDisp,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    // Bail out on error.
    if (fileHandle == INVALID_HANDLE_VALUE) {
        q->setError(QFile::OpenError, qt_error_string());
        return false;
    }

    // Truncate the file after successfully opening it if Truncate is passed.
    if (openMode & QIODevice::Truncate)
        q->setSize(0);

    // Seek to the end when in Append mode.
    if (openMode & QFile::Append) {
        ::SetFilePointer(fileHandle, 0, 0, FILE_END);
    }

    return true;
}

/*
    \internal
*/
bool QFSFileEnginePrivate::nativeClose()
{
    Q_Q(QFSFileEngine);
    if (fh || fd != -1) {
        // stdlib / stdio mode.
        return closeFdFh();
    }

    // Windows native mode.
    bool ok = true;


    if (cachedFd != -1) {
        if (::_close(cachedFd) && !::CloseHandle(fileHandle)) {
            q->setError(QFile::UnspecifiedError, qt_error_string());
            ok = false;
        }

        // System handle is closed with associated file descriptor.
        fileHandle = INVALID_HANDLE_VALUE;
        cachedFd = -1;

        return ok;
    }

    if ((fileHandle == INVALID_HANDLE_VALUE || !::CloseHandle(fileHandle))) {
        q->setError(QFile::UnspecifiedError, qt_error_string());
        ok = false;
    }
    fileHandle = INVALID_HANDLE_VALUE;
    return ok;
}

/*
    \internal
*/
bool QFSFileEnginePrivate::nativeFlush()
{
    if (fh) {
        // Buffered stdlib mode.
        return flushFh();
    }
    if (fd != -1) {
        // Unbuffered stdio mode; always succeeds (no buffer).
        return true;
    }

    // Windows native mode; flushing is unnecessary.
    return true;
}

/*
    \internal
    \since 5.1
*/
bool QFSFileEnginePrivate::nativeSyncToDisk()
{
    if (fh || fd != -1) {
        // stdlib / stdio mode. No API available.
        return false;
    }
    return FlushFileBuffers(fileHandle);
}

/*
    \internal
*/
qint64 QFSFileEnginePrivate::nativeSize() const
{
    Q_Q(const QFSFileEngine);
    QFSFileEngine *thatQ = const_cast<QFSFileEngine *>(q);

    // ### Don't flush; for buffered files, we should get away with ftell.
    thatQ->flush();

    // Always retrive the current information
    metaData.clearFlags(QFileSystemMetaData::SizeAttribute);

    bool filled = false;
    if (fileHandle != INVALID_HANDLE_VALUE && openMode != QIODevice::NotOpen )
        filled = QFileSystemEngine::fillMetaData(fileHandle, metaData,
                                                 QFileSystemMetaData::SizeAttribute);
    else
        filled = doStat(QFileSystemMetaData::SizeAttribute);

    if (!filled) {
        thatQ->setError(QFile::UnspecifiedError, qt_error_string(errno));
    }
    return metaData.size();
}

/*
    \internal
*/
qint64 QFSFileEnginePrivate::nativePos() const
{
    Q_Q(const QFSFileEngine);
    QFSFileEngine *thatQ = const_cast<QFSFileEngine *>(q);

    if (fh || fd != -1) {
        // stdlib / stido mode.
        return posFdFh();
    }

    // Windows native mode.
    if (fileHandle == INVALID_HANDLE_VALUE)
        return 0;

    LARGE_INTEGER currentFilePos;
    LARGE_INTEGER offset;
    offset.QuadPart = 0;
    if (!::SetFilePointerEx(fileHandle, offset, &currentFilePos, FILE_CURRENT)) {
        thatQ->setError(QFile::UnspecifiedError, qt_error_string());
        return 0;
    }

    return qint64(currentFilePos.QuadPart);

}

/*
    \internal
*/
bool QFSFileEnginePrivate::nativeSeek(qint64 pos)
{
    Q_Q(QFSFileEngine);

    if (fh || fd != -1) {
        // stdlib / stdio mode.
        return seekFdFh(pos);
    }

    LARGE_INTEGER currentFilePos;
    LARGE_INTEGER offset;
    offset.QuadPart = pos;
    if (!::SetFilePointerEx(fileHandle, offset, &currentFilePos, FILE_BEGIN)) {
        q->setError(QFile::UnspecifiedError, qt_error_string());
        return false;
    }

    return true;
}

/*
    \internal
*/
qint64 QFSFileEnginePrivate::nativeRead(char *data, qint64 maxlen)
{
    Q_Q(QFSFileEngine);

    if (fh || fd != -1) {
        // stdio / stdlib mode.
        if (fh && nativeIsSequential() && feof(fh)) {
            q->setError(QFile::ReadError, qt_error_string(int(errno)));
            return -1;
        }

        return readFdFh(data, maxlen);
    }

    // Windows native mode.
    if (fileHandle == INVALID_HANDLE_VALUE)
        return -1;

    DWORD bytesToRead = DWORD(maxlen); // <- lossy

    // Reading on Windows fails with ERROR_NO_SYSTEM_RESOURCES when
    // the chunks are too large, so we limit the block size to 32MB.
    static const DWORD maxBlockSize = 32 * 1024 * 1024;

    qint64 totalRead = 0;
    do {
        DWORD blockSize = qMin<DWORD>(bytesToRead, maxBlockSize);
        DWORD bytesRead;
        if (!ReadFile(fileHandle, data + totalRead, blockSize, &bytesRead, NULL)) {
            if (totalRead == 0) {
                // Note: only return failure if the first ReadFile fails.
                q->setError(QFile::ReadError, qt_error_string());
                return -1;
            }
            break;
        }
        if (bytesRead == 0)
            break;
        totalRead += bytesRead;
        bytesToRead -= bytesRead;
    } while (totalRead < maxlen);
    return qint64(totalRead);
}

/*
    \internal
*/
qint64 QFSFileEnginePrivate::nativeReadLine(char *data, qint64 maxlen)
{
    Q_Q(QFSFileEngine);

    if (fh || fd != -1) {
        // stdio / stdlib mode.
        return readLineFdFh(data, maxlen);
    }

    // Windows native mode.
    if (fileHandle == INVALID_HANDLE_VALUE)
        return -1;

    // ### No equivalent in Win32?
    return q->QAbstractFileEngine::readLine(data, maxlen);
}

/*
    \internal
*/
qint64 QFSFileEnginePrivate::nativeWrite(const char *data, qint64 len)
{
    Q_Q(QFSFileEngine);

    if (fh || fd != -1) {
        // stdio / stdlib mode.
        return writeFdFh(data, len);
    }

    // Windows native mode.
    if (fileHandle == INVALID_HANDLE_VALUE)
        return -1;

    qint64 bytesToWrite = DWORD(len); // <- lossy

    // Writing on Windows fails with ERROR_NO_SYSTEM_RESOURCES when
    // the chunks are too large, so we limit the block size to 32MB.
    static const DWORD maxBlockSize = 32 * 1024 * 1024;

    qint64 totalWritten = 0;
    do {
        DWORD blockSize = qMin<DWORD>(bytesToWrite, maxBlockSize);
        DWORD bytesWritten;
        if (!WriteFile(fileHandle, data + totalWritten, blockSize, &bytesWritten, NULL)) {
            if (totalWritten == 0) {
                // Note: Only return error if the first WriteFile failed.
                q->setError(QFile::WriteError, qt_error_string());
                return -1;
            }
            break;
        }
        if (bytesWritten == 0)
            break;
        totalWritten += bytesWritten;
        bytesToWrite -= bytesWritten;
    } while (totalWritten < len);
    return qint64(totalWritten);
}

/*
    \internal
*/
int QFSFileEnginePrivate::nativeHandle() const
{
    if (fh || fd != -1)
        return fh ? QT_FILENO(fh) : fd;

    if (cachedFd != -1)
        return cachedFd;

    int flags = 0;
    if (openMode & QIODevice::Append)
        flags |= _O_APPEND;
    if (!(openMode & QIODevice::WriteOnly))
        flags |= _O_RDONLY;
    cachedFd = _open_osfhandle((intptr_t) fileHandle, flags);
    return cachedFd;

}

/*
    \internal
*/
bool QFSFileEnginePrivate::nativeIsSequential() const
{

    HANDLE handle = fileHandle;
    if (fh || fd != -1)
        handle = (HANDLE)_get_osfhandle(fh ? QT_FILENO(fh) : fd);
    if (handle == INVALID_HANDLE_VALUE)
        return false;

    DWORD fileType = GetFileType(handle);
    return (fileType == FILE_TYPE_CHAR)
            || (fileType == FILE_TYPE_PIPE);

}

bool QFSFileEngine::remove()
{
    Q_D(QFSFileEngine);
    QSystemError error;
    bool ret = QFileSystemEngine::removeFile(d->fileEntry, error);
    if (!ret)
        setError(QFile::RemoveError, error.toString());
    return ret;
}

bool QFSFileEngine::copy(const QString &copyName)
{
    Q_D(QFSFileEngine);
    QSystemError error;
    bool ret = QFileSystemEngine::copyFile(d->fileEntry, QFileSystemEntry(copyName), error);
    if (!ret)
        setError(QFile::CopyError, error.toString());
    return ret;
}

bool QFSFileEngine::rename(const QString &newName)
{
    Q_D(QFSFileEngine);
    QSystemError error;
    bool ret = QFileSystemEngine::renameFile(d->fileEntry, QFileSystemEntry(newName), error);
    if (!ret)
        setError(QFile::RenameError, error.toString());
    return ret;
}

bool QFSFileEngine::renameOverwrite(const QString &newName)
{
    Q_D(QFSFileEngine);
    bool ret = ::MoveFileEx((wchar_t*)d->fileEntry.nativeFilePath().utf16(),
                            (wchar_t*)QFileSystemEntry(newName).nativeFilePath().utf16(),
                            MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != 0;
    if (!ret)
        setError(QFile::RenameError, QSystemError(::GetLastError(), QSystemError::NativeError).toString());
    return ret;
}

bool QFSFileEngine::mkdir(const QString &name, bool createParentDirectories) const
{
    return QFileSystemEngine::createDirectory(QFileSystemEntry(name), createParentDirectories);
}

bool QFSFileEngine::rmdir(const QString &name, bool recurseParentDirectories) const
{
    return QFileSystemEngine::removeDirectory(QFileSystemEntry(name), recurseParentDirectories);
}

bool QFSFileEngine::caseSensitive() const
{
    return false;
}

bool QFSFileEngine::setCurrentPath(const QString &path)
{
    return QFileSystemEngine::setCurrentPath(QFileSystemEntry(path));
}

QString QFSFileEngine::currentPath(const QString &fileName)
{
    QString ret;
    //if filename is a drive: then get the pwd of that drive
    if (fileName.length() >= 2 &&
        fileName.at(0).isLetter() && fileName.at(1) == QLatin1Char(':')) {
        int drv = fileName.toUpper().at(0).toLatin1() - 'A' + 1;
        if (_getdrive() != drv) {
            wchar_t buf[PATH_MAX];
            ::_wgetdcwd(drv, buf, PATH_MAX);
            ret = QString::fromWCharArray(buf);
        }
    }
    if (ret.isEmpty()) {
        //just the pwd
        ret = QFileSystemEngine::currentPath().filePath();
    }
    if (ret.length() >= 2 && ret[1] == QLatin1Char(':'))
        ret[0] = ret.at(0).toUpper(); // Force uppercase drive letters.
    return ret;

}

QString QFSFileEngine::homePath()
{
    return QFileSystemEngine::homePath();
}

QString QFSFileEngine::rootPath()
{
    return QFileSystemEngine::rootPath();
}

QString QFSFileEngine::tempPath()
{
    return QFileSystemEngine::tempPath();
}

QFileInfoList QFSFileEngine::drives()
{
    QFileInfoList ret;

#if defined(Q_OS_WIN32)
    quint32 driveBits = (quint32) GetLogicalDrives() & 0x3ffffff;
#endif

    char driveName[] = "A:/";

    while (driveBits) {
        if (driveBits & 1)
            ret.append(QFileInfo(QLatin1String(driveName)));
        driveName[0]++;
        driveBits = driveBits >> 1;
    }
    return ret;
}

bool QFSFileEnginePrivate::doStat(QFileSystemMetaData::MetaDataFlags flags) const
{
    if (!tried_stat || !metaData.hasFlags(flags)) {
        tried_stat = true;

        int localFd = fd;
        if (fh && fileEntry.isEmpty())
            localFd = QT_FILENO(fh);
        if (localFd != -1)
            QFileSystemEngine::fillMetaData(localFd, metaData, flags);

        if (metaData.missingFlags(flags) && !fileEntry.isEmpty())
            QFileSystemEngine::fillMetaData(fileEntry, metaData, metaData.missingFlags(flags));
    }

    return metaData.exists();
}


bool QFSFileEngine::link(const QString &newName)
{
    bool ret = false;

    QString linkName = newName;
    //### assume that they add .lnk

    IShellLink *psl;
    bool neededCoInit = false;

    HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);

    if (hres == CO_E_NOTINITIALIZED) { // COM was not initialized
        neededCoInit = true;
        CoInitialize(NULL);
        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);
    }

    if (SUCCEEDED(hres)) {
        hres = psl->SetPath((wchar_t *)fileName(AbsoluteName).replace(QLatin1Char('/'), QLatin1Char('\\')).utf16());
        if (SUCCEEDED(hres)) {
            hres = psl->SetWorkingDirectory((wchar_t *)fileName(AbsolutePathName).replace(QLatin1Char('/'), QLatin1Char('\\')).utf16());
            if (SUCCEEDED(hres)) {
                IPersistFile *ppf;
                hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
                if (SUCCEEDED(hres)) {
                    hres = ppf->Save((wchar_t*)linkName.utf16(), TRUE);
                    if (SUCCEEDED(hres))
                         ret = true;
                    ppf->Release();
                }
            }
        }
        psl->Release();
    }
    if (!ret)
        setError(QFile::RenameError, qt_error_string());

    if (neededCoInit)
        CoUninitialize();

    return ret;
}

/*!
    \reimp
*/
QAbstractFileEngine::FileFlags QFSFileEngine::fileFlags(QAbstractFileEngine::FileFlags type) const
{
    Q_D(const QFSFileEngine);

    if (type & Refresh)
        d->metaData.clear();

    QAbstractFileEngine::FileFlags ret = 0;

    if (type & FlagsMask)
        ret |= LocalDiskFlag;

    bool exists;
    {
        QFileSystemMetaData::MetaDataFlags queryFlags = 0;

        queryFlags |= QFileSystemMetaData::MetaDataFlags(uint(type))
                & QFileSystemMetaData::Permissions;

        // AliasType and BundleType are 0x0
        if (type & TypesMask)
            queryFlags |= QFileSystemMetaData::AliasType
                    | QFileSystemMetaData::LinkType
                    | QFileSystemMetaData::FileType
                    | QFileSystemMetaData::DirectoryType
                    | QFileSystemMetaData::BundleType;

        if (type & FlagsMask)
            queryFlags |= QFileSystemMetaData::HiddenAttribute
                    | QFileSystemMetaData::ExistsAttribute;

        queryFlags |= QFileSystemMetaData::LinkType;

        exists = d->doStat(queryFlags);
    }

    if (exists && (type & PermsMask))
        ret |= FileFlags(uint(d->metaData.permissions()));

    if (type & TypesMask) {
        if ((type & LinkType) && d->metaData.isLegacyLink())
            ret |= LinkType;
        if (d->metaData.isDirectory()) {
            ret |= DirectoryType;
        } else {
            ret |= FileType;
        }
    }
    if (type & FlagsMask) {
        if (d->metaData.exists()) {
            ret |= ExistsFlag;
            if (d->fileEntry.isRoot())
                ret |= RootFlag;
            else if (d->metaData.isHidden())
                ret |= HiddenFlag;
        }
    }
    return ret;
}

QString QFSFileEngine::fileName(FileName file) const
{
    Q_D(const QFSFileEngine);
    if (file == BaseName) {
        return d->fileEntry.fileName();
    } else if (file == PathName) {
        return d->fileEntry.path();
    } else if (file == AbsoluteName || file == AbsolutePathName) {
        QString ret;

        if (!isRelativePath()) {

            if (d->fileEntry.filePath().startsWith(QLatin1Char('/')) || // It's a absolute path to the current drive, so \a.txt -> Z:\a.txt
                d->fileEntry.filePath().size() == 2 ||                  // It's a drive letter that needs to get a working dir appended
                (d->fileEntry.filePath().size() > 2 && d->fileEntry.filePath().at(2) != QLatin1Char('/')) || // It's a drive-relative path, so Z:a.txt -> Z:\currentpath\a.txt
                d->fileEntry.filePath().contains(QLatin1String("/../")) || d->fileEntry.filePath().contains(QLatin1String("/./")) ||
                d->fileEntry.filePath().endsWith(QLatin1String("/..")) || d->fileEntry.filePath().endsWith(QLatin1String("/.")))
            {
                ret = QDir::fromNativeSeparators(QFileSystemEngine::nativeAbsoluteFilePath(d->fileEntry.filePath()));
            } else

            {
                ret = d->fileEntry.filePath();
            }
        } else {
            ret = QDir::cleanPath(QDir::currentPath() + QLatin1Char('/') + d->fileEntry.filePath());
        }

        // The path should be absolute at this point.
        // From the docs :
        // Absolute paths begin with the directory separator "/"
        // (optionally preceded by a drive specification under Windows).
        if (ret.at(0) != QLatin1Char('/')) {
            Q_ASSERT(ret.length() >= 2);
            Q_ASSERT(ret.at(0).isLetter());
            Q_ASSERT(ret.at(1) == QLatin1Char(':'));

            // Force uppercase drive letters.
            ret[0] = ret.at(0).toUpper();
        }

        if (file == AbsolutePathName) {
            int slash = ret.lastIndexOf(QLatin1Char('/'));
            if (slash < 0)
                return ret;
            else if (ret.at(0) != QLatin1Char('/') && slash == 2)
                return ret.left(3);      // include the slash
            else
                return ret.left(slash > 0 ? slash : 1);
        }
        return ret;
    } else if (file == CanonicalName || file == CanonicalPathName) {
        if (!(fileFlags(ExistsFlag) & ExistsFlag))
            return QString();
        QFileSystemEntry entry(QFileSystemEngine::canonicalName(QFileSystemEntry(fileName(AbsoluteName)), d->metaData));

        if (file == CanonicalPathName)
            return entry.path();
        return entry.filePath();
    } else if (file == LinkName) {
        return QFileSystemEngine::getLinkTarget(d->fileEntry, d->metaData).filePath();
    } else if (file == BundleName) {
        return QString();
    }
    return d->fileEntry.filePath();
}

bool QFSFileEngine::isRelativePath() const
{
    Q_D(const QFSFileEngine);
    // drive, e.g. "a:", or UNC root, e.q. "//"
    return d->fileEntry.isRelative();
}

uint QFSFileEngine::ownerId(FileOwner /*own*/) const
{
    static const uint nobodyID = (uint) -2;
    return nobodyID;
}

QString QFSFileEngine::owner(FileOwner own) const
{
    Q_D(const QFSFileEngine);
    return QFileSystemEngine::owner(d->fileEntry, own);
}

bool QFSFileEngine::setPermissions(uint perms)
{
    Q_D(QFSFileEngine);
    QSystemError error;
    bool ret = QFileSystemEngine::setPermissions(d->fileEntry, QFile::Permissions(perms), error);
    if (!ret)
        setError(QFile::PermissionsError, error.toString());
    return ret;
}

bool QFSFileEngine::setSize(qint64 size)
{
    Q_D(QFSFileEngine);

    if (d->fileHandle != INVALID_HANDLE_VALUE || d->fd != -1 || d->fh) {
        // resize open file
        HANDLE fh = d->fileHandle;

        if (fh == INVALID_HANDLE_VALUE) {
            if (d->fh)
                fh = (HANDLE)_get_osfhandle(QT_FILENO(d->fh));
            else
                fh = (HANDLE)_get_osfhandle(d->fd);
        }

        if (fh == INVALID_HANDLE_VALUE)
            return false;
        qint64 currentPos = pos();

        if (seek(size) && SetEndOfFile(fh)) {
            seek(qMin(currentPos, size));
            return true;
        }

        seek(currentPos);
        return false;
    }

    if (!d->fileEntry.isEmpty()) {
        // resize file on disk
        QFile file(d->fileEntry.filePath());
        if (file.open(QFile::ReadWrite)) {
            bool ret = file.resize(size);
            if (!ret)
                setError(QFile::ResizeError, file.errorString());
            return ret;
        }
    }
    return false;
}


QDateTime QFSFileEngine::fileTime(FileTime time) const
{
    Q_D(const QFSFileEngine);

    if (d->doStat(QFileSystemMetaData::Times))
        return d->metaData.fileTime(time);

    return QDateTime();
}

uchar *QFSFileEnginePrivate::map(qint64 offset, qint64 size,
                                 QFile::MemoryMapFlags flags)
{
    Q_Q(QFSFileEngine);
    Q_UNUSED(flags);
    if (openMode == QFile::NotOpen) {
        q->setError(QFile::PermissionsError, qt_error_string(ERROR_ACCESS_DENIED));
        return 0;
    }
    if (offset == 0 && size == 0) {
        q->setError(QFile::UnspecifiedError, qt_error_string(ERROR_INVALID_PARAMETER));
        return 0;
    }

    if (mapHandle == NULL) {
        // get handle to the file
        HANDLE handle = fileHandle;

        if (handle == INVALID_HANDLE_VALUE && fh)
            handle = (HANDLE)::_get_osfhandle(QT_FILENO(fh));

        if (handle == INVALID_HANDLE_VALUE) {
            q->setError(QFile::PermissionsError, qt_error_string(ERROR_ACCESS_DENIED));
            return 0;
        }

        // first create the file mapping handle
        DWORD protection = (openMode & QIODevice::WriteOnly) ? PAGE_READWRITE : PAGE_READONLY;
        mapHandle = ::CreateFileMapping(handle, 0, protection, 0, 0, 0);
        if (mapHandle == NULL) {
            q->setError(QFile::PermissionsError, qt_error_string());
            return 0;
        }
    }

    // setup args to map
    DWORD access = 0;
    if (openMode & QIODevice::ReadOnly) access = FILE_MAP_READ;
    if (openMode & QIODevice::WriteOnly) access = FILE_MAP_WRITE;

    DWORD offsetHi = offset >> 32;
    DWORD offsetLo = offset & Q_UINT64_C(0xffffffff);
    SYSTEM_INFO sysinfo;
    ::GetSystemInfo(&sysinfo);
    DWORD mask = sysinfo.dwAllocationGranularity - 1;
    DWORD extra = offset & mask;
    if (extra)
        offsetLo &= ~mask;

    // attempt to create the map
    LPVOID mapAddress = ::MapViewOfFile(mapHandle, access,
                                      offsetHi, offsetLo, size + extra);
    if (mapAddress) {
        uchar *address = extra + static_cast<uchar*>(mapAddress);
        maps[address] = extra;
        return address;
    }

    switch(GetLastError()) {
    case ERROR_ACCESS_DENIED:
        q->setError(QFile::PermissionsError, qt_error_string());
        break;
    case ERROR_INVALID_PARAMETER:
        // size are out of bounds
    default:
        q->setError(QFile::UnspecifiedError, qt_error_string());
    }

    ::CloseHandle(mapHandle);
    mapHandle = NULL;
    return 0;
}

bool QFSFileEnginePrivate::unmap(uchar *ptr)
{
    Q_Q(QFSFileEngine);
    if (!maps.contains(ptr)) {
        q->setError(QFile::PermissionsError, qt_error_string(ERROR_ACCESS_DENIED));
        return false;
    }
    uchar *start = ptr - maps[ptr];
    if (!UnmapViewOfFile(start)) {
        q->setError(QFile::PermissionsError, qt_error_string());
        return false;
    }

    maps.remove(ptr);
    if (maps.isEmpty()) {
        ::CloseHandle(mapHandle);
        mapHandle = NULL;
    }

    return true;
}

QT_END_NAMESPACE
