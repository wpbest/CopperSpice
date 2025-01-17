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

#ifndef QT_MAC_P_H
#define QT_MAC_P_H

#include "qmacdefines_mac.h"

#ifdef __OBJC__
#include <Cocoa/Cocoa.h>

#ifdef QT_MAC_USE_COCOA
#include <objc/runtime.h>
#endif

#endif

#include <CoreServices/CoreServices.h>

#include "QtCore/qglobal.h"
#include "QtCore/qvariant.h"
#include "QtCore/qmimedata.h"
#include "QtCore/qpointer.h"
#include "qcore_mac_p.h"
#include "QtGui/qpainter.h"
#include <Carbon/Carbon.h>

QT_BEGIN_NAMESPACE
class QWidget;
class QDragMoveEvent;

/* Event masks */
// internal Qt types

 // Event class for our own Carbon events.
#if defined(QT_NAMESPACE) && defined(QT_NAMESPACE_MAC_CRC)
// Take the CRC we generated at configure time. This *may* result in a
// collision with another value If that is the case, please change the value
// here to something other than 'Cute'.
const UInt32 kEventClassQt = QT_NAMESPACE_MAC_CRC;
#else
const UInt32 kEventClassQt = 'Cute';
#endif

enum {
    //AE types
    typeAEClipboardChanged = 1,
    //types
    typeQWidget = 1,  /* QWidget *  */
    //params
    kEventParamQWidget = 'qwid',   /* typeQWidget */
    //events
    kEventQtRequestContext = 13,
    kEventQtRequestMenubarUpdate = 14,
    kEventQtRequestShowSheet = 17,
    kEventQtRequestActivate = 18,
    kEventQtRequestWindowChange = 20
};

// Simple class to manage short-lived regions
class QMacSmartQuickDrawRegion
{
    RgnHandle qdRgn;
    Q_DISABLE_COPY(QMacSmartQuickDrawRegion)
public:
    explicit QMacSmartQuickDrawRegion(RgnHandle rgn) : qdRgn(rgn) {}
    ~QMacSmartQuickDrawRegion() {
        extern void qt_mac_dispose_rgn(RgnHandle); // qregion_mac.cpp
        qt_mac_dispose_rgn(qdRgn);
    }
    operator RgnHandle() {
        return qdRgn;
    }
};

// Class for chaining to gether a bunch of fades. It pretty much is only used for qmenu fading.
class QMacWindowFader
{
    QWidgetList m_windowsToFade;
    float m_duration;
    Q_DISABLE_COPY(QMacWindowFader)
public:
    QMacWindowFader(); // PLEASE DON'T CALL THIS.
    static QMacWindowFader *currentFader();
    void registerWindowToFade(QWidget *window);
    void setFadeDuration(float durationInSecs) { m_duration = durationInSecs; }
    float fadeDuration() const { return m_duration; }
    void performFade();
};

class Q_GUI_EXPORT QMacCocoaAutoReleasePool
{
private:
    void *pool;
public:
    QMacCocoaAutoReleasePool();
    ~QMacCocoaAutoReleasePool();

    inline void *handle() const { return pool; }
};

QString qt_mac_removeMnemonics(const QString &original); //implemented in qmacstyle_mac.cpp

class Q_GUI_EXPORT QMacWindowChangeEvent
{
private:
    static QList<QMacWindowChangeEvent*> *change_events;
public:
    QMacWindowChangeEvent() {
    }
    virtual ~QMacWindowChangeEvent() {
    }
    static inline void exec(bool ) {
    }
protected:
    virtual void windowChanged() = 0;
    virtual void flushWindowChanged() = 0;
};

class QMacCGContext
{
    CGContextRef context;
public:
    QMacCGContext(QPainter *p); //qpaintengine_mac.cpp
    inline QMacCGContext() { context = 0; }
    inline QMacCGContext(const QPaintDevice *pdev) {
        extern CGContextRef qt_mac_cg_context(const QPaintDevice *);
        context = qt_mac_cg_context(pdev);
    }
    inline QMacCGContext(CGContextRef cg, bool takeOwnership=false) {
        context = cg;
        if(!takeOwnership)
            CGContextRetain(context);
    }
    inline QMacCGContext(const QMacCGContext &copy) : context(0) { *this = copy; }
    inline ~QMacCGContext() {
        if(context)
            CGContextRelease(context);
    }
    inline bool isNull() const { return context; }
    inline operator CGContextRef() { return context; }
    inline QMacCGContext &operator=(const QMacCGContext &copy) {
        if(context)
            CGContextRelease(context);
        context = copy.context;
        CGContextRetain(context);
        return *this;
    }
    inline QMacCGContext &operator=(CGContextRef cg) {
        if(context)
            CGContextRelease(context);
        context = cg;
        CGContextRetain(context); //we do not take ownership
        return *this;
    }
};

class QMacPasteboardMime;
class QMimeData;

class QMacPasteboard
{
    struct Promise {
        Promise() : itemId(0), convertor(0) { }
        Promise(int itemId, QMacPasteboardMime *c, QString m, QVariant d, int o=0) : itemId(itemId), offset(o), convertor(c), mime(m), data(d) { }
        int itemId, offset;
        QMacPasteboardMime *convertor;
        QString mime;
        QVariant data;
    };
    QList<Promise> promises;

    OSPasteboardRef paste;
    uchar mime_type;
    mutable QPointer<QMimeData> mime;
    mutable bool mac_mime_source;
    static OSStatus promiseKeeper(OSPasteboardRef, PasteboardItemID, CFStringRef, void *);
    void clear_helper();
public:
    QMacPasteboard(OSPasteboardRef p, uchar mime_type=0);
    QMacPasteboard(uchar mime_type);
    QMacPasteboard(CFStringRef name=0, uchar mime_type=0);
    ~QMacPasteboard();

    bool hasFlavor(QString flavor) const;
    bool hasOSType(int c_flavor) const;

    OSPasteboardRef pasteBoard() const;
    QMimeData *mimeData() const;
    void setMimeData(QMimeData *mime);

    QStringList formats() const;
    bool hasFormat(const QString &format) const;
    QVariant retrieveData(const QString &format, QVariant::Type) const;

    void clear();
    bool sync() const;
};

extern QPaintDevice *qt_mac_safe_pdev; //qapplication_mac.cpp

extern OSWindowRef qt_mac_window_for(const QWidget*); //qwidget_mac.mm
extern OSViewRef qt_mac_nativeview_for(const QWidget *); //qwidget_mac.mm
extern QPoint qt_mac_nativeMapFromParent(const QWidget *child, const QPoint &pt); //qwidget_mac.mm

#ifdef check
# undef check
#endif

QFont qfontForThemeFont(ThemeFontID themeID);

QColor qcolorForTheme(ThemeBrush brush);

QColor qcolorForThemeTextColor(ThemeTextColor themeColor);

struct QMacDndAnswerRecord {
    QRect rect;
    Qt::KeyboardModifiers modifiers;
    Qt::MouseButtons buttons;
    Qt::DropAction lastAction;
    unsigned int lastOperation;
    void clear() {
        rect = QRect();
        modifiers = Qt::NoModifier;
        buttons = Qt::NoButton;
        lastAction = Qt::IgnoreAction;
        lastOperation = 0;
    }
};
extern QMacDndAnswerRecord qt_mac_dnd_answer_rec;
void qt_mac_copy_answer_rect(const QDragMoveEvent &event);
bool qt_mac_mouse_inside_answer_rect(QPoint mouse);

QT_END_NAMESPACE

#endif // QT_MAC_P_H
