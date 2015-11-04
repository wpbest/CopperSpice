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

#ifndef PHONON_EFFECTPARAMETER_P_H
#define PHONON_EFFECTPARAMETER_P_H

#include "effectparameter.h"
#include <QtCore/QSharedData>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PHONON_EFFECT

namespace Phonon
{

class EffectParameterPrivate : public QSharedData
{
    public:
        int parameterId;
        QVariant min;
        QVariant max;
        QVariant defaultValue;
        QString name;
        QString description;
        QVariantList possibleValues;
        EffectParameter::Hints hints;
};

} // namespace Phonon

#endif //QT_NO_PHONON_EFFECT

QT_END_NAMESPACE

#endif // EFFECTPARAMETER_P_H

