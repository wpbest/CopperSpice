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

#ifndef QFONTENGINE_CORETEXT_P_H
#define QFONTENGINE_CORETEXT_P_H

#include <qfontengine_p.h>

#if defined(Q_OS_IOS)
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <qcore_mac_p.h>
#endif

#if !defined(Q_WS_MAC) || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)

QT_BEGIN_NAMESPACE

class QRawFontPrivate;
class QCoreTextFontEngineMulti;
class QCoreTextFontEngine : public QFontEngine
{
public:
    QCoreTextFontEngine(CTFontRef font, const QFontDef &def);
    QCoreTextFontEngine(CGFontRef font, const QFontDef &def);
    ~QCoreTextFontEngine();

    virtual bool stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs, QTextEngine::ShaperFlags flags) const;
    virtual void recalcAdvances(QGlyphLayout *, QTextEngine::ShaperFlags) const;

    virtual glyph_metrics_t boundingBox(const QGlyphLayout &glyphs);
    virtual glyph_metrics_t boundingBox(glyph_t glyph);

    virtual QFixed ascent() const;
    virtual QFixed descent() const;
    virtual QFixed leading() const;
    virtual QFixed xHeight() const;
    virtual qreal maxCharWidth() const;
    virtual QFixed averageCharWidth() const;

    virtual void addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int numGlyphs,
                                 QPainterPath *path, QTextItem::RenderFlags);

    virtual const char *name() const { return "QCoreTextFontEngine"; }

    virtual bool canRender(const QChar *string, int len);

    virtual int synthesized() const { return synthesisFlags; }
    virtual bool supportsSubPixelPositions() const { return true; }

    virtual Type type() const { return QFontEngine::Mac; }

    void draw(CGContextRef ctx, qreal x, qreal y, const QTextItemInt &ti, int paintDeviceHeight);

    virtual FaceId faceId() const;
    virtual bool getSfntTableData(uint /*tag*/, uchar * /*buffer*/, uint * /*length*/) const;
    virtual void getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics);
    virtual QImage alphaMapForGlyph(glyph_t, QFixed subPixelPosition);
    virtual QImage alphaRGBMapForGlyph(glyph_t, QFixed subPixelPosition, int margin, const QTransform &t);
    virtual qreal minRightBearing() const;
    virtual qreal minLeftBearing() const;
    virtual QFixed emSquareSize() const;

    virtual QFontEngine *cloneWithSize(qreal pixelSize) const;
    virtual int glyphMargin(QFontEngineGlyphCache::Type type) { Q_UNUSED(type); return 0; }

    static bool supportsColorGlyphs()
    {
#if defined(Q_OS_IOS)
        return true;
#elif MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
  #if MAC_OS_X_VERSION_MIN_REQUIRED < 1070
        return &CTFontDrawGlyphs;
  #else
        return true;
  #endif
#else
        return false;
#endif
    }

    static int antialiasingThreshold;

private:
    friend class QRawFontPrivate;

    void init();
    QImage imageForGlyph(glyph_t glyph, QFixed subPixelPosition, int margin, bool colorful);
    CTFontRef ctfont;
    CGFontRef cgFont;
    int synthesisFlags;
    CGAffineTransform transform;
    QFixed avgCharWidth;
    friend class QCoreTextFontEngineMulti;
};

class QCoreTextFontEngineMulti : public QFontEngineMulti
{
public:
    QCoreTextFontEngineMulti(const QCFString &name, const QFontDef &fontDef, bool kerning);
    QCoreTextFontEngineMulti(CTFontRef ctFontRef, const QFontDef &fontDef, bool kerning);
    ~QCoreTextFontEngineMulti();

    virtual bool stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs,
                              QTextEngine::ShaperFlags flags) const;
    bool stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs,
                      QTextEngine::ShaperFlags flags,
                      unsigned short *logClusters, const HB_CharAttributes *charAttributes,
                      QScriptItem *si) const;

    virtual const char *name() const { return "CoreText"; }
    inline CTFontRef macFontID() const { return ctfont; }

protected:
    virtual void loadEngine(int at);

private:
    void init(bool kerning);
    inline const QCoreTextFontEngine *engineAt(int i) const
    { return static_cast<const QCoreTextFontEngine *>(engines.at(i)); }

    uint fontIndexForFont(CTFontRef font) const;
    CTFontRef ctfont;
    mutable QCFType<CFMutableDictionaryRef> attributeDict;
    CGAffineTransform transform;
    friend class QFontDialogPrivate;
    bool transformAdvances;
};

CGAffineTransform qt_transform_from_fontdef(const QFontDef &fontDef);

QT_END_NAMESPACE

#endif// !defined(Q_WS_MAC) || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)

#endif // QFONTENGINE_CORETEXT_P_H
