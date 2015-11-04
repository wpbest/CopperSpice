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

#include "qdesigner_q3widgetstack_p.h"
#include "../../../lib/shared/qdesigner_propertycommand_p.h"

#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>

#include <QtCore/QEvent>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

namespace {
    QToolButton *createToolButton(QWidget *parent, Qt::ArrowType at, const QString &name) {
         QToolButton *rc =  new QToolButton();
         rc->setAttribute(Qt::WA_NoChildEventsForParent, true);
         rc->setParent(parent);
         rc->setObjectName(name);
         rc->setArrowType(at);
         rc->setAutoRaise(true);
         rc->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
         rc->setFixedSize(QSize(15, 15));
         return rc;
     }
}

QDesignerQ3WidgetStack::QDesignerQ3WidgetStack(QWidget *parent) : 
    Q3WidgetStack(parent), 
    m_prev(createToolButton(this, Qt::LeftArrow,  QLatin1String("__qt__passive_prev"))),
    m_next(createToolButton(this, Qt::RightArrow, QLatin1String("__qt__passive_next")))
{
    connect(m_prev, SIGNAL(clicked()), this, SLOT(prevPage()));
    connect(m_next, SIGNAL(clicked()), this, SLOT(nextPage()));
    updateButtons();

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));
}

QDesignerFormWindowInterface *QDesignerQ3WidgetStack::formWindow()
{
    return QDesignerFormWindowInterface::findFormWindow(this);
}

QDesignerContainerExtension *QDesignerQ3WidgetStack::container()
{
    if (formWindow()) {
        QDesignerFormEditorInterface *core = formWindow()->core();
        return qt_extension<QDesignerContainerExtension*>(core->extensionManager(), this);
    }
    return 0;
}

int QDesignerQ3WidgetStack::count()
{
    return container() ? container()->count() : 0;
}

int QDesignerQ3WidgetStack::currentIndex()
{
    return container() ? container()->currentIndex() : -1;
}

void QDesignerQ3WidgetStack::setCurrentIndex(int index)
{
    if (container() && (index >= 0) && (index < count())) {
        container()->setCurrentIndex(index);
        emit currentChanged(index);
    }
}

QWidget *QDesignerQ3WidgetStack::widget(int index)
{
    return container() ? container()->widget(index) : 0;
}

void QDesignerQ3WidgetStack::updateButtons()
{
    if (m_prev) {
        m_prev->move(width() - 31, 1);
        m_prev->show();
        m_prev->raise();
    }

    if (m_next) {
        m_next->move(width() - 16, 1);
        m_next->show();
        m_next->raise();
    }
}

void QDesignerQ3WidgetStack::gotoPage(int page) {
    // Are we on a form or in a preview?
    if (QDesignerFormWindowInterface *fw = formWindow()) {
        qdesigner_internal::SetPropertyCommand *cmd = new qdesigner_internal::SetPropertyCommand(fw);
        cmd->init(this, QLatin1String("currentIndex"), page);
        fw->commandHistory()->push(cmd);
        fw->emitSelectionChanged(); // Magically prevent an endless loop triggered by auto-repeat.
    } else {
        setCurrentIndex(page);
    }
    updateButtons();
}


void QDesignerQ3WidgetStack::prevPage()
{
    if (count() > 1) {
        int newIndex = currentIndex() - 1;
        if (newIndex < 0)
            newIndex = count() - 1;
        gotoPage(newIndex);
    }
}

void QDesignerQ3WidgetStack::nextPage()
{
    if (count() > 1)
        gotoPage((currentIndex() + 1) % count());
}

QString QDesignerQ3WidgetStack::currentPageName()
{
    if (currentIndex() == -1)
        return QString();

    return widget(currentIndex())->objectName();
}

void QDesignerQ3WidgetStack::setCurrentPageName(const QString &pageName)
{
    if (currentIndex() == -1)
        return;

    if (QWidget *w = widget(currentIndex()))
        w->setObjectName(pageName);
}

bool QDesignerQ3WidgetStack::event(QEvent *e)
{
    if (e->type() == QEvent::LayoutRequest) {
        updateButtons();
    }

    return Q3WidgetStack::event(e);
}

void QDesignerQ3WidgetStack::childEvent(QChildEvent *e)
{
    Q3WidgetStack::childEvent(e);
    updateButtons();
}

void QDesignerQ3WidgetStack::resizeEvent(QResizeEvent *e)
{
    Q3WidgetStack::resizeEvent(e);
    updateButtons();
}

void QDesignerQ3WidgetStack::showEvent(QShowEvent *e)
{
    Q3WidgetStack::showEvent(e);
    updateButtons();
}

void QDesignerQ3WidgetStack::slotCurrentChanged(int index)
{
    if (widget(index)) {
        if (QDesignerFormWindowInterface *fw = formWindow()) {
            fw->clearSelection();
            fw->selectWidget(this, true);
        }
    }
}

QT_END_NAMESPACE
