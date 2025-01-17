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

#ifndef Patternist_CallSite_H
#define Patternist_CallSite_H

#include "qunlimitedcontainer_p.h"
#include "qcalltargetdescription_p.h"
#include "qxmlname.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class CallSite : public UnlimitedContainer
    {
    public:
        QXmlName name() const;
        bool isRecursive() const;
        void setIsRecursive(const bool value);

        /**
         * Called in the earliest stages of the compilation process. @p sign can
         * be any function signature for a user declared function. If @p sign
         * matches this UserFunctionCallsite, it means the UserFunction represented
         * by @p sign is recursive and that this UserFunctionCallsite should take
         * appropriate measures.
         *
         * @returns @c true if is recursive, otherwise @c false
         */
        virtual bool configureRecursion(const CallTargetDescription::Ptr &sign) = 0;

        /**
         * @short Returns the body of the function/template/component that is
         * being called.
         */
        virtual Expression::Ptr body() const = 0;

        virtual CallTargetDescription::Ptr callTargetDescription() const = 0;

    protected:
        CallSite(const QXmlName &name = QXmlName());

    private:
        Q_DISABLE_COPY(CallSite)
        bool            m_isRecursive;
        const QXmlName  m_name;
    };
}

QT_END_NAMESPACE

#endif
