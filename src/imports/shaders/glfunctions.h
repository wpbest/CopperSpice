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

#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#ifndef QT_OPENGL_ES

#ifndef Q_WS_MAC
# ifndef APIENTRYP
#   ifdef APIENTRY
#     define APIENTRYP APIENTRY *
#   else
#     define APIENTRY
#     define APIENTRYP *
#   endif
# endif
#else
# define APIENTRY
# define APIENTRYP *
#endif

#define GL_TEXTURE0 0x84C0
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_BGRA 0x80E1

typedef void (APIENTRYP type_glActiveTexture)(GLenum texture);
typedef void (APIENTRYP type_glGenerateMipmap)(GLenum target);
typedef void (APIENTRYP type_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);

#define glActiveTexture ((type_glActiveTexture)QGLContext::currentContext()->getProcAddress(QLatin1String("glActiveTexture")))
#define glGenerateMipmap ((type_glGenerateMipmap)QGLContext::currentContext()->getProcAddress(QLatin1String("glGenerateMipmap")))
#define glVertexAttribPointer ((type_glVertexAttribPointer)QGLContext::currentContext()->getProcAddress(QLatin1String("glVertexAttribPointer")))

#endif

#endif // GLFUNCTIONS_H
