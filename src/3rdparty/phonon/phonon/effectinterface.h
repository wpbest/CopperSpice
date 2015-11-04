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

/********************************************************
**  Copyright (C) 2005-2006 Matthias Kretz <kretz@kde.org
********************************************************/

#ifndef PHONON_EFFECTINTERFACE_H
#define PHONON_EFFECTINTERFACE_H

#include "phononnamespace.h"
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PHONON_EFFECT

namespace Phonon
{
    class EffectParameter;
    /** \class EffectInterface effectinterface.h Phonon/EffectInterface
     * \short Interface for Effect objects
     *
     * \author Matthias Kretz <kretz@kde.org>
     */
    class EffectInterface
    {
        public:
            virtual ~EffectInterface() {}
            /**
             * Returns the EffectParameter objects to describe the parameters of this effect.
             */
            virtual QList<EffectParameter> parameters() const = 0;
            /**
             * Returns the value for the selected parameter.
             */
            virtual QVariant parameterValue(const EffectParameter &) const = 0;
            /**
             * Sets the value for the selected parameter.
             */
            virtual void setParameterValue(const EffectParameter &, const QVariant &newValue) = 0;
    };
} //namespace Phonon

CS_DECLARE_INTERFACE(Phonon::EffectInterface, "EffectInterface0.phonon.kde.org")

#endif //QT_NO_PHONON_EFFECT

QT_END_NAMESPACE

#endif // PHONON_EFFECTINTERFACE_H
