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

#ifndef QFILESYSTEMWATCHER_H
#define QFILESYSTEMWATCHER_H

#include <QtCore/qobject.h>
#include <QScopedPointer>

#ifndef QT_NO_FILESYSTEMWATCHER

QT_BEGIN_NAMESPACE

class QFileSystemWatcherPrivate;

class Q_CORE_EXPORT QFileSystemWatcher : public QObject
{
    CS_OBJECT(QFileSystemWatcher)
    Q_DECLARE_PRIVATE(QFileSystemWatcher)

public:
    QFileSystemWatcher(QObject *parent = 0);
    QFileSystemWatcher(const QStringList &paths, QObject *parent = 0);
    ~QFileSystemWatcher();

    void addPath(const QString &file);
    void addPaths(const QStringList &files);
    void removePath(const QString &file);
    void removePaths(const QStringList &files);

    QStringList files() const;
    QStringList directories() const;

    CORE_CS_SIGNAL_1(Public, void fileChanged(const QString & path))
    CORE_CS_SIGNAL_2(fileChanged,path) 

    CORE_CS_SIGNAL_1(Public, void directoryChanged(const QString & path))
    CORE_CS_SIGNAL_2(directoryChanged,path) 

private:
    CORE_CS_SLOT_1(Private, void _q_fileChanged(const QString &path, bool removed))
    CORE_CS_SLOT_2(_q_fileChanged)

    CORE_CS_SLOT_1(Private, void _q_directoryChanged(const QString &path, bool removed))
    CORE_CS_SLOT_2(_q_directoryChanged)

protected:
	 QScopedPointer<QFileSystemWatcherPrivate> d_ptr;
    
};

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER

#endif // QFILESYSTEMWATCHER_H
