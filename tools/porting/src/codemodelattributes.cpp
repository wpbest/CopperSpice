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

#include "codemodelattributes.h"
#include "tokenengine.h"

QT_BEGIN_NAMESPACE

using namespace CodeModel;
using namespace TokenEngine;

/*
     Walk the codemodel.
*/
void CodeModelAttributes::createAttributes(TranslationUnit translationUnit)
{
    m_translationUnit = translationUnit;
    parseScope(const_cast<CodeModel::NamespaceScope *>(translationUnit.codeModel()));
}

/*
    Create attributes for each name use and assign to the token.
*/
void CodeModelAttributes::parseNameUse(CodeModel::NameUse *nameUse)
{
    // Get the container for this token.
    TokenRef ref = nameUse->nameToken();
    const int containerIndex = ref.containerIndex();
    TokenAttributes *attributes = ref.tokenContainer().tokenAttributes();

    if (!areAttributesEnabled(attributes))
        return;

    // Test if the nameUse refers to a UnknownType. If so we add and
    // "unknown" attribute.
    if (TypeMember *typeMember =  nameUse->declaration()->toTypeMember()) {
        if (typeMember->type()->toUnknownType()) {
            attributes->addAttribute(containerIndex, "unknown", nameUse->name());
            return;
        }
    }

    // Add attributes this namnUse.
    attributes->addAttribute(containerIndex, "nameUse", nameUse->name());
    attributes->addAttribute(containerIndex, "parentScope",
                             nameUse->declaration()->parent()->name() );
    if (CodeModel::Scope * skop = nameUse->declaration()->parent()->parent()) {
        attributes->addAttribute(containerIndex, "grandParentScope", skop->name());
    }

    createNameTypeAttribute(nameUse);
}

/*
    Create attributes for members and assign to token.
*/
void CodeModelAttributes::parseMember(CodeModel::Member *member)
{
    if(!member || member->name() == QByteArray())
        return;

    //get the container for this token
    TokenRef ref = member->nameToken();
    const int containerIndex = ref.containerIndex();
    TokenAttributes *attributes = ref.tokenContainer().tokenAttributes();

    if (areAttributesEnabled(attributes)) {
        //add attributes for this declaration
        static const QByteArray textDeclaration = "declaration";
        attributes->addAttribute(containerIndex, textDeclaration, member->name());
        createNameTypeAttribute(member);
    }
    CodeModelWalker::parseMember(member);
}
    
void CodeModelAttributes::parseFunctionMember(CodeModel::FunctionMember *member)
{
    CodeModel::ArgumentCollection arguments = member->arguments();
    CodeModel::ArgumentCollection::ConstIterator it = arguments.constBegin();
    TokenRef ref = member->nameToken();
    TokenAttributes *attributes = ref.tokenContainer().tokenAttributes();
    
    if (areAttributesEnabled(attributes)) {
        while (it != arguments.constEnd()) {
            const int containerIndex = (*it)->nameToken().containerIndex();
            const QByteArray name = (*it)->name();
            attributes->addAttribute(containerIndex, "declaration", name);
            attributes->addAttribute(containerIndex, "nameType", "variable");
            ++it;
        }
    }
    CodeModelWalker::parseFunctionMember(member);
}

/*
    NameType attributes gives information on what kind of member this is.
*/
void CodeModelAttributes::createNameTypeAttribute(CodeModel::Member *member)
{
    if(!member)
        return;
    //get the container for the token accosiated with this member.
    TokenRef ref = member->nameToken();
    const int containerIndex = ref.containerIndex();
    TokenAttributes *attributes = ref.tokenContainer().tokenAttributes();

    createNameTypeAttributeAtIndex(attributes, containerIndex, member);
}

/*
    A NameUse has the same NameType as the declaration it is referring to.
*/
void CodeModelAttributes::createNameTypeAttribute(CodeModel::NameUse *nameUse)
{
    if(!nameUse)
        return;

    //get the container for the token accosiated with this NameUse.
    TokenRef ref = nameUse->nameToken();
    const int containerIndex = ref.containerIndex();
    TokenAttributes *attributes = ref.tokenContainer().tokenAttributes();

    createNameTypeAttributeAtIndex(attributes, containerIndex, nameUse->declaration());
}

void CodeModelAttributes::createNameTypeAttributeAtIndex(TokenEngine::TokenAttributes *attributes,
                                        int index, CodeModel::Member *member)
{
    QByteArray nameType = "unknown";
    if (member->toFunctionMember()) {
        nameType = "function";
    } else if (CodeModel::VariableMember *variableMember = member->toVariableMember()) {
        if (variableMember->type()->toEnumType())
            nameType = "enumerator";
        else
            nameType = "variable";
    } else if (CodeModel::TypeMember *typeMember =  member->toTypeMember()) {
        if (CodeModel::Type *type = typeMember->type()) {
            if (type->toClassType()) {
                nameType = "class";
            } else if (type->toEnumType()) {
                nameType = "enum";
            }
        }
    }
    attributes->addAttribute(index, "nameType", nameType);
}

bool CodeModelAttributes::areAttributesEnabled(const TokenAttributes *attributes) const
{
    static const QByteArray tstCreateAttributes("CreateAttributes");
    static const QByteArray tstTrue("True");
    return (attributes->attribute(tstCreateAttributes) == tstTrue);
}


QT_END_NAMESPACE
