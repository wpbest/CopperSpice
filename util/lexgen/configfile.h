/***********************************************************************
*
* Copyright (c) 2012-2013 Barbara Geller
* Copyright (c) 2012-2013 Ansel Sermersheim
* Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QStringList>
#include <QMap>
#include <QVector>

struct ConfigFile
{
    struct Entry
    {
        inline Entry() : lineNumber(-1) {}
        int lineNumber;
        QString key;
        QString value;
    };
    struct Section : public QVector<Entry>
    {
        inline bool contains(const QString &key) const
        {
            for (int i = 0; i < count(); ++i)
                if (at(i).key == key)
                    return true;
            return false;
        }
        inline QString value(const QString &key, const QString &defaultValue = QString()) const
        {
            for (int i = 0; i < count(); ++i)
                if (at(i).key == key)
                    return at(i).value;
            return defaultValue;
        }
    };
    typedef QMap<QString, Section> SectionMap;

    static SectionMap parse(const QString &fileName);
    static SectionMap parse(QIODevice *dev);
};

#endif // CONFIGFILE_H

