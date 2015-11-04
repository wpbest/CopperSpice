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

#ifndef QLIBRARYINFO_H
#define QLIBRARYINFO_H

#include <QtCore/qstring.h>
#include <QtCore/QDate>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_SETTINGS

class Q_CORE_EXPORT QLibraryInfo
{
public:
    static QString licensee();
    static QString licensedProducts();
    static QString buildKey();

#ifndef QT_NO_DATESTRING
    static QDate buildDate();
#endif //QT_NO_DATESTRING

    enum LibraryLocation
    {
        PrefixPath,
        DocumentationPath,
        HeadersPath,
        LibrariesPath,
        BinariesPath,
        PluginsPath,
        DataPath,
        TranslationsPath,
        SettingsPath,
        DemosPath,
        ExamplesPath,
        ImportsPath
    };
    static QString location(LibraryLocation); // ### Qt5/consider renaming it to path()

private:
    QLibraryInfo();
};

#endif /* QT_NO_SETTINGS */

QT_END_NAMESPACE

#endif // QLIBRARYINFO_H
