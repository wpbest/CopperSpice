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

#include "qgenericpluginfactory_qpa.h"

#include "qapplication.h"
#include "private/qfactoryloader_p.h"
#include "qgenericplugin_qpa.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

#if !defined(Q_OS_WIN32) || defined(QT_MAKEDLL)

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
    (QGenericPluginFactoryInterface_iid,
     QLatin1String("/generic"), Qt::CaseInsensitive))

#endif //QT_MAKEDLL

/*!
    \class QGenericPluginFactory
    \ingroup qpa
    \since 4.8

    \brief The QGenericPluginFactory class creates window-system
    related plugin drivers in Qt QPA.

    Note that this class is only available in Qt QPA.

    \sa QGenericPlugin
*/

/*!
    Creates the driver specified by \a key, using the given \a specification.

    Note that the keys are case-insensitive.

    \sa keys()
*/
QObject *QGenericPluginFactory::create(const QString& key, const QString &specification)
{
    QString driver = key.toLower();

#if !defined(Q_OS_WIN32) || defined(QT_MAKEDLL)
    if (QGenericPluginFactoryInterface *factory = qobject_cast<QGenericPluginFactoryInterface*>(loader()->instance(driver)))
        return factory->create(driver, specification);
#endif
    return 0;
}

/*!
    Returns the list of valid keys, i.e. the available mouse drivers.

    \sa create()
*/
QStringList QGenericPluginFactory::keys()
{
    QStringList list;

#if !defined(Q_OS_WIN32) || defined(QT_MAKEDLL)
    QStringList plugins = loader()->keys();
    for (int i = 0; i < plugins.size(); ++i) {
        if (!list.contains(plugins.at(i)))
            list += plugins.at(i);
    }
#endif //QT_MAKEDLL
    return list;
}

QT_END_NAMESPACE
