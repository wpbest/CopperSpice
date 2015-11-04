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

#ifndef Patternist_XsdValidatedXmlNodeModel_H
#define Patternist_XsdValidatedXmlNodeModel_H

#include "qabstractxmlnodemodel.h"
#include "qabstractxmlforwarditerator_p.h"
#include "qitem_p.h"
#include "qschematype_p.h"
#include "qxsdelement_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
 
    class XsdValidatedXmlNodeModel : public QAbstractXmlNodeModel
    {
        public:
            typedef QExplicitlySharedDataPointer<XsdValidatedXmlNodeModel> Ptr;
            typedef QList<Ptr> List;

            /**
             * Creates a new validated xml node model.
             */
            XsdValidatedXmlNodeModel(const QAbstractXmlNodeModel *model);

            /**
             * Destroys the validated xml node model.
             */
            virtual ~XsdValidatedXmlNodeModel();

            virtual QUrl baseUri(const QXmlNodeModelIndex &ni) const;
            virtual QUrl documentUri(const QXmlNodeModelIndex &ni) const;
            virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex &ni) const;
            virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex &ni1, const QXmlNodeModelIndex &ni2) const;
            virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex &n) const;
            virtual QXmlName name(const QXmlNodeModelIndex &ni) const;
            virtual QString stringValue(const QXmlNodeModelIndex &n) const;
            virtual QVariant typedValue(const QXmlNodeModelIndex &n) const;
            virtual QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QXmlNodeModelIndex> > iterate(const QXmlNodeModelIndex &ni, QXmlNodeModelIndex::Axis axis) const;
            virtual QPatternist::ItemIteratorPtr sequencedTypedValue(const QXmlNodeModelIndex &ni) const;
            virtual QPatternist::ItemTypePtr type(const QXmlNodeModelIndex &ni) const;
            virtual QXmlName::NamespaceCode namespaceForPrefix(const QXmlNodeModelIndex &ni, const QXmlName::PrefixCode prefix) const;
            virtual bool isDeepEqual(const QXmlNodeModelIndex &ni1, const QXmlNodeModelIndex &ni2) const;
            virtual void sendNamespaces(const QXmlNodeModelIndex &n, QAbstractXmlReceiver *const receiver) const;
            virtual QVector<QXmlName> namespaceBindings(const QXmlNodeModelIndex &n) const;
            virtual QXmlNodeModelIndex elementById(const QXmlName &NCName) const;
            virtual QVector<QXmlNodeModelIndex> nodesByIdref(const QXmlName &NCName) const;
            virtual void copyNodeTo(const QXmlNodeModelIndex &node, QAbstractXmlReceiver *const receiver, const NodeCopySettings &) const;

            /**
             * Sets the @p element that is assigned to the xml node at @p index.
             */
            void setAssignedElement(const QXmlNodeModelIndex &index, const XsdElement::Ptr &element);

            /**
             * Returns the element that is assigned to the xml node at @p index.
             */
            XsdElement::Ptr assignedElement(const QXmlNodeModelIndex &index) const;

            /**
             * Sets the @p attribute that is assigned to the xml node at @p index.
             */
            void setAssignedAttribute(const QXmlNodeModelIndex &index, const XsdAttribute::Ptr &attribute);

            /**
             * Returns the attribute that is assigned to the xml node at @p index.
             */
            XsdAttribute::Ptr assignedAttribute(const QXmlNodeModelIndex &index) const;

            /**
             * Sets the @p type that is assigned to the xml node at @p index.
             *
             * @note The type can be a different than the type of the element or
             *       attribute that is assigned to the index, since the instance
             *       document can overwrite it by xsi:type.
             */
            void setAssignedType(const QXmlNodeModelIndex &index, const SchemaType::Ptr &type);

            /**
             * Returns the type that is assigned to the xml node at @p index.
             */
            SchemaType::Ptr assignedType(const QXmlNodeModelIndex &index) const;

            /**
             * Adds the attribute or element @p binding with the given @p id.
             */
            void addIdIdRefBinding(const QString &id, const NamedSchemaComponent::Ptr &binding);

            /**
             * Returns a list of all binding ids.
             */
            QStringList idIdRefBindingIds() const;

            /**
             * Returns the set of bindings with the given @p id.
             */
            QSet<NamedSchemaComponent::Ptr> idIdRefBindings(const QString &id) const;

        protected:
            virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis axis, const QXmlNodeModelIndex &origin) const;
            virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex &element) const;

        private:
            QExplicitlySharedDataPointer<const QAbstractXmlNodeModel> m_internalModel;
            QHash<QXmlNodeModelIndex, XsdElement::Ptr>                m_assignedElements;
            QHash<QXmlNodeModelIndex, XsdAttribute::Ptr>              m_assignedAttributes;
            QHash<QXmlNodeModelIndex, SchemaType::Ptr>                m_assignedTypes;
            QHash<QString, QSet<NamedSchemaComponent::Ptr> >          m_idIdRefBindings;
    };
}

QT_END_NAMESPACE

#endif
