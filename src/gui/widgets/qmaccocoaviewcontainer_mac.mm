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

#import <Cocoa/Cocoa.h>
#include <private/qwidget_p.h>
#include "qmaccocoaviewcontainer_mac.h"
#include <private/qt_mac_p.h>

/*!
    \class QMacCocoaViewContainer
    \since 4.5

    \brief The QMacCocoaViewContainer class provides a widget for Mac OS X that can be used to wrap arbitrary
    Cocoa views (i.e., NSView subclasses) and insert them into Qt hierarchies.

    \ingroup advanced

    While Qt offers a lot of classes for writing your application, Apple's
    Cocoa framework offers lots of functionality that is not currently in Qt or
    may never end up in Qt. Using QMacCocoaViewContainer, it is possible to put an
    arbitrary NSView-derived class from Cocoa and put it in a Qt hierarchy.
    Depending on how comfortable you are with using objective-C, you can use
    QMacCocoaViewContainer directly, or subclass it to wrap further functionality
    of the underlying NSView.

    QMacCocoaViewContainer works regardless if Qt is built against Carbon or
    Cocoa. However, QCocoaContainerView requires Mac OS X 10.5 or better to be
    used with Carbon.

    It should be also noted that at the low level on Mac OS X, there is a
    difference between windows (top-levels) and view (widgets that are inside a
    window). For this reason, make sure that the NSView that you are wrapping
    doesn't end up as a top-level. The best way to ensure this is to make sure
    you always have a parent and not set the parent to 0.

    If you are using QMacCocoaViewContainer as a sub-class and are mixing and
    matching objective-C with C++ (a.k.a. objective-C++). It is probably
    simpler to have your file end with \tt{.mm} than \tt{.cpp}. Most Apple tools will
    correctly identify the source as objective-C++.

    QMacCocoaViewContainer requires knowledge of how Cocoa works, especially in
    regard to its reference counting (retain/release) nature. It is noted in
    the functions below if there is any change in the reference count. Cocoa
    views often generate temporary objects that are released by an autorelease
    pool. If this is done outside of a running event loop, it is up to the
    developer to provide the autorelease pool.

    The following is a snippet of subclassing QMacCocoaViewContainer to wrap a NSSearchField.
    \snippet demos/macmainwindow/macmainwindow.mm 0

*/

QT_BEGIN_NAMESPACE

class QMacCocoaViewContainerPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QMacCocoaViewContainer)
public:
    NSView *nsview;
#ifndef QT_MAC_USE_COCOA
    HIViewRef wrapperView;
#endif
    QMacCocoaViewContainerPrivate();
    ~QMacCocoaViewContainerPrivate();
};

QMacCocoaViewContainerPrivate::QMacCocoaViewContainerPrivate()
     : nsview(0)
#ifndef QT_MAC_USE_COCOA
       , wrapperView(0)
#endif
{
}

QMacCocoaViewContainerPrivate::~QMacCocoaViewContainerPrivate()
{
    [nsview release];
#ifndef QT_MAC_USE_COCOA
    if (wrapperView)
        CFRelease(wrapperView);
#endif
}

/*!
    \fn QMacCocoaViewContainer::QMacCocoaViewContainer(void *cocoaViewToWrap, QWidget *parent)
 
    Create a new QMacCocoaViewContainer using the NSView pointer in \a
    cocoaViewToWrap with parent, \a parent. QMacCocoaViewContainer will
    retain \a cocoaViewToWrap.

    \a cocoaViewToWrap is a void pointer that allows the header to be included
    with C++ source.
*/
QMacCocoaViewContainer::QMacCocoaViewContainer(void *cocoaViewToWrap, QWidget *parent)
   : QWidget(*new QMacCocoaViewContainerPrivate, parent, 0)
{
    if (cocoaViewToWrap)
        setCocoaView(cocoaViewToWrap);

    // QMacCocoaViewContainer requires a native window handle.
    setAttribute(Qt::WA_NativeWindow);
}

/*!
    Destroy the QMacCocoaViewContainer and release the wrapped view.
*/
QMacCocoaViewContainer::~QMacCocoaViewContainer()
{
}

/*!
    Returns the NSView that has been set on this container.  The returned view
    has been autoreleased, so you will need to retain it if you want to make
    use of it.
*/
void *QMacCocoaViewContainer::cocoaView() const
{
    Q_D(const QMacCocoaViewContainer);
    return [[d->nsview retain] autorelease];
}

/*!
    Sets the NSView to contain to be \a cocoaViewToWrap and retains it. If this
    container already had a view set, it will release the previously set view.
*/
void QMacCocoaViewContainer::setCocoaView(void *cocoaViewToWrap)
{
    Q_D(QMacCocoaViewContainer);
    QMacCocoaAutoReleasePool pool;
    NSView *view = static_cast<NSView *>(cocoaViewToWrap);
    NSView *oldView = d->nsview;
    destroy(true, true);
    [view retain];
    d->nsview = view;
#ifndef QT_MAC_USE_COCOA
    if (QSysInfo::MacintoshVersion < QSysInfo::MV_10_5) {
        qWarning("QMacCocoaViewContainer::setCocoaView: You cannot use this class with Carbon on versions of Mac OS X less than 10.5.");
        return;
    }
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
    if (d->wrapperView)
        CFRelease(d->wrapperView);
    HICocoaViewCreate(d->nsview, 0, &d->wrapperView);
    create(WId(d->wrapperView), false, true);
#endif
#else
    create(WId(d->nsview), false, true);
#endif
    [oldView release];
}

QT_END_NAMESPACE
