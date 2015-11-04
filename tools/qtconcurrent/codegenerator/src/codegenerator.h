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
#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <QByteArray>
#include <QString>
#include <QList>
#include <QStack>

namespace CodeGenerator
{
    enum GeneratorType {NoopType, CompoundType, TextType, RepeaterType, CounterType, GroupType};
    class BaseGenerator;
    typedef QStack<BaseGenerator *> GeneratorStack;
    
    template <typename ValueType>
    class Stacker {
    public:
        Stacker(QStack<ValueType> *stack, ValueType value) : stack(stack) { stack->push(value); }
        ~Stacker() { stack->pop();}
    private:
        QStack<ValueType> *stack;
    };
    typedef Stacker<BaseGenerator *> GeneratorStacker;
    
    class BaseGenerator
    {
    public:
        BaseGenerator(GeneratorType type = NoopType) : type(type) {}
        virtual ~BaseGenerator() {};
        virtual QByteArray generate(GeneratorStack *stack) { Q_UNUSED(stack); return QByteArray(); };
        int currentCount(GeneratorStack *stack) const;
        int repeatCount(GeneratorStack *stack) const;
        GeneratorType type;   
    };

    class Item
    {
    public:
        Item(BaseGenerator * const base) : generator(base) {}
        Item(const char * const text);
        QByteArray generate() const
        {  GeneratorStack stack; return generator->generate(&stack);  }
        // ### TODO: Fix memory leak!
        // QExplicitlySharedDataPointer<BaseGenerator> generator;
        BaseGenerator * const generator;
    };
    
    class CompoundGenerator : public BaseGenerator
    {
    public:
        CompoundGenerator(BaseGenerator * const a, BaseGenerator * const b) 
          : BaseGenerator(CompoundType), a(a), b(b) {}
        virtual QByteArray generate(GeneratorStack *stack)
        { return a->generate(stack) + b->generate(stack); };
    protected:    
        BaseGenerator * const a;
        BaseGenerator * const b;
    };
    
    class Compound : public Item
    {
    public:
        Compound(const Item &a, const Item &b) : Item(new CompoundGenerator(a.generator, b.generator)) {}
    };
    
    class TextGenerator : public BaseGenerator
    {
    public:
        TextGenerator(const QByteArray &text) : BaseGenerator(TextType), text(text) {}
        virtual QByteArray generate(GeneratorStack *) { return text; };
    protected:    
        QByteArray text;
    };
        
    class Text : public Item {
    public:
        Text(const QByteArray &text) : Item(new TextGenerator(text)) {}
        Text(const char * const text) : Item(new TextGenerator(QByteArray(text))) {}
    };

    class RepeaterGenerator : public BaseGenerator
    {
    public:
        RepeaterGenerator(BaseGenerator * const childGenerator) 
          : BaseGenerator(RepeaterType), repeatCount(1), repeatOffset(0), childGenerator(childGenerator) {}
        virtual QByteArray generate(GeneratorStack *stack);

        int repeatCount;
        int repeatOffset;
        int currentRepeat;
        BaseGenerator * const childGenerator;
    };
    
    class Repeater : public Item {
    public:
        Repeater(const Item &item) : Item(new RepeaterGenerator(item.generator)) {}
        void setRepeatCount(int count) 
        { static_cast<RepeaterGenerator * const>(generator)->repeatCount = count;  }
        void setRepeatOffset(int offset)
        { static_cast<RepeaterGenerator * const>(generator)->repeatOffset = offset;  }
    };
    
    class CounterGenerator : public BaseGenerator
    {
    public:
        CounterGenerator() : BaseGenerator(CounterType), offset(0), increment(1), reverse(false) {}
        QByteArray generate(GeneratorStack *stack)
        { 
            if (reverse)
                return QByteArray::number(repeatCount(stack) - (currentCount(stack) * increment) + offset + 1);
            else
                return QByteArray::number((currentCount(stack) * increment) + offset);
        }
        int offset;
        int increment;
        bool reverse;
    };
    
    class Counter : public Item {
    public:
        Counter() : Item(new CounterGenerator()) {}
        Counter(int offset) : Item(new CounterGenerator()) { setOffset(offset); }
        void setOffset(int offset)
        { static_cast<CounterGenerator *>(generator)->offset = offset; }
        void setIncrement(int increment)
        { static_cast<CounterGenerator *>(generator)->increment = increment; }
        void setReverse(bool reverse)
        { static_cast<CounterGenerator *>(generator)->reverse = reverse; }

    };
    
    class GroupGenerator : public BaseGenerator
    {
    public:
        GroupGenerator(BaseGenerator * const childGenerator)
          : BaseGenerator(GroupType), currentRepeat(0), childGenerator(childGenerator), 
            separator(new BaseGenerator()), prefix(new BaseGenerator()), postfix(new BaseGenerator()) { }
        virtual QByteArray generate(GeneratorStack *stack);
        int currentRepeat;
        BaseGenerator * const childGenerator;
        BaseGenerator *separator;
        BaseGenerator *prefix;
        BaseGenerator *postfix;
    };
    
    class Group : public Item
    {
    public:
        Group(const Item &item) : Item(new GroupGenerator(item.generator)) { setSeparator(", "); }
        void setSeparator(const Item &separator)        
        { static_cast<GroupGenerator *>(generator)->separator = separator.generator; }
        void setPrefix(const Item &prefix)        
        { static_cast<GroupGenerator *>(generator)->prefix = prefix.generator; }
        void setPostfix(const Item &postfix)        
        { static_cast<GroupGenerator *>(generator)->postfix = postfix.generator; }
    };

    const Compound operator+(const Item &a, const Item &b); 
    const Compound operator+(const Item &a, const char * const text);
    const Compound operator+(const char * const text, const Item &b);

} //namespace CodeGenerator

#endif
