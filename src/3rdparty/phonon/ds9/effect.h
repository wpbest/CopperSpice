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

#ifndef DS9_EFFECT_H
#define DS9_EFFECT_H

#include <QtCore/QObject>
#include <phonon/effectinterface.h>
#include "backendnode.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PHONON_EFFECT

namespace Phonon
{
    namespace DS9
    {
        class EffectInterface;

        class Effect : public BackendNode, public Phonon::EffectInterface
        {
            CS_OBJECT(Effect)
                CS_INTERFACES(Phonon::EffectInterface)
        public:
            Effect(CLSID effectClass, QObject *parent);
            ~Effect();

            QList<Phonon::EffectParameter> parameters() const;
            QVariant parameterValue(const Phonon::EffectParameter &) const;
            void setParameterValue(const Phonon::EffectParameter &, const QVariant &);


        protected:
            //this is the constructor called by the explicit subclasses of effect
            Effect(QObject *parent);
        };
    }
}

#endif //QT_NO_PHONON_EFFECT

QT_END_NAMESPACE

#endif // PHONON_AUDIOEFFECT_H
