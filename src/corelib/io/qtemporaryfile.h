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

#ifndef QTEMPORARYFILE_H
#define QTEMPORARYFILE_H

#include <QtCore/qiodevice.h>
#include <QtCore/qfile.h>

#ifdef open
#error qtemporaryfile.h must be included before any header file that defines open
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TEMPORARYFILE

class QTemporaryFilePrivate;

class Q_CORE_EXPORT QTemporaryFile : public QFile
{
    CS_OBJECT(QTemporaryFile)

    Q_DECLARE_PRIVATE(QTemporaryFile)

public:
    QTemporaryFile();
    explicit QTemporaryFile(const QString &templateName);

    explicit QTemporaryFile(QObject *parent);
    QTemporaryFile(const QString &templateName, QObject *parent);

    ~QTemporaryFile();

    bool autoRemove() const;
    void setAutoRemove(bool b);

    // ### Hides open(flags)
    bool open() { return open(QIODevice::ReadWrite); }

    QString fileName() const;
    QString fileTemplate() const;
    void setFileTemplate(const QString &name);

    inline static QTemporaryFile *createLocalFile(const QString &fileName)
        { QFile file(fileName); return createLocalFile(file); }
    static QTemporaryFile *createLocalFile(QFile &file);

protected:
    bool open(OpenMode flags);

private:
    friend class QFile;
    Q_DISABLE_COPY(QTemporaryFile)
};

#endif // QT_NO_TEMPORARYFILE

QT_END_NAMESPACE

#endif // QTEMPORARYFILE_H
