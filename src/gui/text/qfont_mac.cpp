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

#include "qfont.h"
#include "qfont_p.h"
#include "qfontengine_p.h"
#include "qfontengine_mac_p.h"
#include "qfontengine_coretext_p.h"
#include "qfontinfo.h"
#include "qfontmetrics.h"
#include "qpaintdevice.h"
#include "qstring.h"
#include <private/qt_mac_p.h>
#include <private/qtextengine_p.h>
#include <private/qunicodetables_p.h>
#include <qapplication.h>
#include "qfontdatabase.h"
#include <qpainter.h>
#include "qtextengine_p.h"
#include <stdlib.h>

QT_BEGIN_NAMESPACE

extern float qt_mac_defaultDpi_x(); //qpaintdevice_mac.cpp

int qt_mac_pixelsize(const QFontDef &def, int dpi)
{
    float ret;
    if(def.pixelSize == -1)
        ret = def.pointSize *  dpi / qt_mac_defaultDpi_x();
    else
        ret = def.pixelSize;
    return qRound(ret);
}
int qt_mac_pointsize(const QFontDef &def, int dpi)
{
    float ret;
    if(def.pointSize < 0)
        ret = def.pixelSize * qt_mac_defaultDpi_x() / float(dpi);
    else
        ret = def.pointSize;
    return qRound(ret);
}

QString QFont::rawName() const
{
    return family();
}

void QFont::setRawName(const QString &name)
{
    setFamily(name);
}

void QFont::cleanup()
{
    QFontCache::cleanup();
}

/*!
  Returns an ATSUFontID
*/
quint32 QFont::macFontID() const  // ### need 64-bit version
{
#ifdef QT_MAC_USE_COCOA
    return 0;
#elif 1
    QFontEngine *fe = d->engineForScript(QUnicodeTables::Common);
    if (fe && fe->type() == QFontEngine::Multi)
        return static_cast<QFontEngineMacMulti*>(fe)->macFontID();
#else
    Str255 name;
    if(FMGetFontFamilyName((FMFontFamily)((UInt32)handle()), name) == noErr) {
        short fnum;
        GetFNum(name, &fnum);
        return fnum;
    }
#endif
    return 0;
}

// Returns an ATSUFonFamilyRef
Qt::HANDLE QFont::handle() const
{
#ifdef QT_MAC_USE_COCOA
    QFontEngine *fe = d->engineForScript(QUnicodeTables::Common);
    if (fe && fe->type() == QFontEngine::Multi)
        return (Qt::HANDLE)static_cast<QCoreTextFontEngineMulti*>(fe)->macFontID();
#endif
    return 0;
}

void QFont::initialize()
{ }

QString QFont::defaultFamily() const
{
    switch(d->request.styleHint) {
        case QFont::Times:
            return QString::fromLatin1("Times New Roman");
        case QFont::Courier:
            return QString::fromLatin1("Courier New");
        case QFont::Monospace:
            return QString::fromLatin1("Courier");
        case QFont::Decorative:
            return QString::fromLatin1("Bookman Old Style");
        case QFont::Cursive:
            return QString::fromLatin1("Apple Chancery");
        case QFont::Fantasy:
            return QString::fromLatin1("Papyrus");
        case QFont::Helvetica:
        case QFont::System:
        default:
            return QString::fromLatin1("Helvetica");
    }
}

QString QFont::lastResortFamily() const
{
    return QString::fromLatin1("Helvetica");
}

QString QFont::lastResortFont() const
{
    return QString::fromLatin1("Geneva");
}

QT_END_NAMESPACE
