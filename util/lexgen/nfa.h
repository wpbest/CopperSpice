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

#ifndef NFA_H
#define NFA_H

#include <QMap>
#include <QHash>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QStack>
#include <QByteArray>

#include "global.h"

typedef QHash<InputType, int> TransitionMap;

struct State
{
    QString symbol;
    TransitionMap transitions;
};

inline QDataStream &operator<<(QDataStream &stream, const State &state)
{
    return stream << state.symbol << state.transitions;
}

inline QDataStream &operator>>(QDataStream &stream, State &state)
{
    return stream >> state.symbol >> state.transitions;
}

struct DFA : public QVector<State>
{
    void debug() const;
    DFA minimize() const;
};

class NFA
{
public:
    static NFA createSingleInputNFA(InputType input);
    static NFA createSymbolNFA(const QString &symbol);
    static NFA createAlternatingNFA(const NFA &a, const NFA &b);
    static NFA createConcatenatingNFA(const NFA &a, const NFA &b);
    static NFA createOptionalNFA(const NFA &a);

    // convenience
    static NFA createStringNFA(const QByteArray &str);
    static NFA createSetNFA(const QSet<InputType> &set);
    static NFA createZeroOrOneNFA(const NFA &a);
    static NFA applyQuantity(const NFA &a, int minOccurrences, int maxOccurrences);

    void setTerminationSymbol(const QString &symbol);
    
    DFA toDFA() const;

    inline bool isEmpty() const { return states.isEmpty(); }
    inline int stateCount() const { return states.count(); }

    void debug();

private:
    void initialize(int size);
    void addTransition(int from, InputType input, int to);
    void copyFrom(const NFA &other, int baseState);

    void initializeFromPair(const NFA &a, const NFA &b,
                            int *initialA, int *finalA,
                            int *initialB, int *finalB);

    QSet<int> epsilonClosure(const QSet<int> &initialClosure) const;

    inline void assertValidState(int state)
    { Q_UNUSED(state); Q_ASSERT(state >= 0); Q_ASSERT(state < states.count()); }
    
#if defined(AUTOTEST)
public:
#endif
    int initialState;
    int finalState;

    QVector<State> states;
};

#endif // NFA_H

