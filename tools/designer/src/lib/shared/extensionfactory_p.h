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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef SHARED_EXTENSIONFACTORY_H
#define SHARED_EXTENSIONFACTORY_H

#include <QtDesigner/default_extensionfactory.h>
#include <QtDesigner/QExtensionManager>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// Extension factory for registering an extension for an object type.
template <class ExtensionInterface, class Object, class Extension>
class ExtensionFactory: public QExtensionFactory
{
public:
    explicit ExtensionFactory(const QString &iid, QExtensionManager *parent = 0);

    // Convenience for registering the extension. Do not use for derived classes.
    static void registerExtension(QExtensionManager *mgr, const QString &iid);

protected:
    virtual QObject *createExtension(QObject *qObject, const QString &iid, QObject *parent) const;

private:
    // Can be overwritten to perform checks on the object.
    // Default does a qobject_cast to the desired class.
    virtual Object *checkObject(QObject *qObject) const;

    const QString m_iid;
};

template <class ExtensionInterface, class Object, class Extension>
ExtensionFactory<ExtensionInterface, Object, Extension>::ExtensionFactory(const QString &iid, QExtensionManager *parent) :
    QExtensionFactory(parent),
    m_iid(iid)
{
}

template <class ExtensionInterface, class Object, class Extension>
Object *ExtensionFactory<ExtensionInterface, Object, Extension>::checkObject(QObject *qObject) const
{
    return qobject_cast<Object*>(qObject);
}

template <class ExtensionInterface, class Object, class Extension>
QObject *ExtensionFactory<ExtensionInterface, Object, Extension>::createExtension(QObject *qObject, const QString &iid, QObject *parent) const
{
    if (iid != m_iid)
        return 0;

    Object *object = checkObject(qObject);
    if (!object)
        return 0;

    return new Extension(object, parent);
}

template <class ExtensionInterface, class Object, class Extension>
void ExtensionFactory<ExtensionInterface, Object, Extension>::registerExtension(QExtensionManager *mgr, const QString &iid)
{
    ExtensionFactory *factory = new ExtensionFactory(iid, mgr);
    mgr->registerExtensions(factory, iid);
}
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SHARED_EXTENSIONFACTORY_H
