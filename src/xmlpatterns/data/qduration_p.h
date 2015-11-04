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

#ifndef QDuration_P_H
#define QDuration_P_H

#include "qabstractduration_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    class Duration : public AbstractDuration
    {
    public:
        typedef AtomicValue::Ptr Ptr;

        /**
         * Creates an instance from the lexical representation @p string.
         */
        static Duration::Ptr fromLexical(const QString &string);
        static Duration::Ptr fromComponents(const bool isPositive,
                                            const YearProperty years,
                                            const MonthProperty months,
                                            const DayCountProperty days,
                                            const HourProperty hours,
                                            const MinuteProperty minutes,
                                            const SecondProperty seconds,
                                            const MSecondProperty mseconds);

        virtual ItemType::Ptr type() const;
        virtual QString stringValue() const;

        /**
         * Always results in an assert crash. Calling this function makes no
         * sense due to that the value space of xs:duration is not well defined.
         */
        virtual Value value() const;

        /**
         * Always results in an assert crash. Calling this function makes no
         * sense due to that the value space of xs:duration is not well defined.
         */
        virtual Item fromValue(const Value val) const;

        virtual YearProperty years() const;
        virtual MonthProperty months() const;
        virtual DayCountProperty days() const;
        virtual HourProperty hours() const;
        virtual MinuteProperty minutes() const;
        virtual SecondProperty seconds() const;
        virtual MSecondProperty mseconds() const;

    protected:
        friend class CommonValues;

        Duration(const bool isPositive,
                 const YearProperty years,
                 const MonthProperty months,
                 const DayCountProperty days,
                 const HourProperty hours,
                 const MinuteProperty minutes,
                 const SecondProperty seconds,
                 const MSecondProperty mseconds);
    private:
        const YearProperty      m_years;
        const MonthProperty     m_months;
        const DayCountProperty  m_days;
        const HourProperty      m_hours;
        const MinuteProperty    m_minutes;
        const SecondProperty    m_seconds;
        const MSecondProperty   m_mseconds;
    };
}

QT_END_NAMESPACE

#endif
