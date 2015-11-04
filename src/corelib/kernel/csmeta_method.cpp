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

#include "qobject.h"
#include "csmeta.h"
#include "qmetaobject.h"

QMetaMethod::QMetaMethod(const char *typeName,
                           const char *signature,
                           QList<QByteArray> paramNames,
                           QMetaMethod::Access access,
                           QMetaMethod::MethodType methodType,
                           QMetaMethod::Attributes attributes,
                           QMetaObject *obj)
{
   m_typeName   = typeName;
   m_signature  = signature;  
   m_paramNames = paramNames;
   m_access     = access;
   m_methodType = methodType;
   m_attributes = attributes;
   m_metaObject = obj;      
  
   m_bento     = nullptr;
   m_tag       = "";
   m_revision  = 0;  
}

QMetaMethod::QMetaMethod()
{
   m_typeName   = "";
   m_signature  = nullptr;
   // m_paramNames
   m_access     = Private;
   m_methodType = Method;
   m_attributes = Attributes(0);
   m_metaObject = nullptr;

   m_bento = nullptr;
   m_tag   = "";
}

QMetaMethod::Access QMetaMethod::access() const
{
   return m_access;
}

QMetaMethod::Attributes QMetaMethod::attributes() const
{
   return m_attributes;
}

bool QMetaMethod::compare(const BentoAbstract &method) const
{
   bool ok = false;

   if (m_bento) {
      if (*m_bento == method) {
         ok = true;
      }
   }

   return ok;
}

const BentoAbstract *QMetaMethod::getBentoBox() const
{
   return m_bento;
}

const QMetaObject *QMetaMethod::getMetaObject() const
{
   return m_metaObject;
}

int QMetaMethod::methodIndex() const
{
   if (! m_metaObject) {
      return -1;
   }

   return m_metaObject->indexOfMethod(m_signature);
}

QMetaMethod::MethodType QMetaMethod::methodType() const
{
   return m_methodType;
}

QList<QByteArray> QMetaMethod::parameterNames() const
{
   return m_paramNames;
}

QList<QByteArray> QMetaMethod::parameterTypes() const
{
   QList<QByteArray> retval;

   const char *temp = m_signature;
   char letter;

   while (*temp)  {
      letter = *temp;

      if (letter == '(') {
         break;
      }

      ++temp;
   }

   ++temp;

   QByteArray word;
   int angleLevel    = 0;
   int bracketLevel  = 0;
   int parenLevel    = 0;

   while (*temp)  {
      letter = *temp;

      // A
      if (letter == '<') {
         ++angleLevel;

      } else if (letter == '>')  {
         --angleLevel;

      } else if (letter == '[')  {
         ++bracketLevel;

      } else if (letter == ']')  {
         --bracketLevel;

      } else if (letter == '(')  {
         ++parenLevel;

      } else if (letter == ')')  {
         --parenLevel;

      }

      // B
      if ((angleLevel == 0 && bracketLevel == 0 && parenLevel == 0) && letter == ',') {
         retval.append(word);
         word = "";

      } else {
         word += letter;

      }

      ++temp;
   }

   // remove last letter
   word.chop(1);

   if (! word.isEmpty() ) {
      retval.append(word);
   }

   return retval;
}

int QMetaMethod::revision() const
{
   return m_revision;
}

void QMetaMethod::setBentoBox(const BentoAbstract *method)
{
   m_bento = method;
}

const char * QMetaMethod::signature() const
{
   return m_signature;
}

void QMetaMethod::setRevision(int revision)
{
   m_revision = revision;
}

void QMetaMethod::setTag(const char *data)
{
   m_tag = data;
}

const char * QMetaMethod::tag() const
{
   return m_tag;
}

const char * QMetaMethod::typeName() const
{
   return m_typeName;
}

