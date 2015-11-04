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

#ifndef Patternist_XsdModelGroup_H
#define Patternist_XsdModelGroup_H

#include "qxsdparticle_p.h"
#include "qxsdterm_p.h"

QT_BEGIN_NAMESPACE

template<typename N> class QList;

namespace QPatternist
{  
    class XsdModelGroup : public XsdTerm
    {
        public:
            typedef QExplicitlySharedDataPointer<XsdModelGroup> Ptr;
            typedef QList<XsdModelGroup::Ptr> List;

            /**
             * Describes the <a href="http://www.w3.org/TR/xmlschema11-1/#mg-compositor">compositor</a> of the model group.
             */
            enum ModelCompositor
            {
                SequenceCompositor,     ///< The model group is a sequence.
                ChoiceCompositor,       ///< The model group is a choice.
                AllCompositor           ///< The model group contains elements only.
            };

            /**
             * Creates a new model group object.
             */
            XsdModelGroup();

            /**
             * Returns always @c true, used to avoid dynamic casts.
             */
            virtual bool isModelGroup() const;

            /**
             * Sets the @p compositor of the model group.
             *
             * @see ModelCompositor
             */
            void setCompositor(ModelCompositor compositor);

            /**
             * Returns the compositor of the model group.
             */
            ModelCompositor compositor() const;

            /**
             * Sets the list of @p particles of the model group.
             *
             * @see <a href="http://www.w3.org/TR/xmlschema11-1/#mg-particles">Particles Definition</a>
             */
            void setParticles(const XsdParticle::List &particles);

            /**
             * Returns the list of particles of the model group.
             */
            XsdParticle::List particles() const;

        private:
            ModelCompositor   m_compositor;
            XsdParticle::List m_particles;
    };
}

QT_END_NAMESPACE

#endif
