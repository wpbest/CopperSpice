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

#include "formwindow_widgetstack.h"
#include <QtDesigner/QDesignerFormWindowToolInterface>

#include <QtGui/QWidget>
#include <QtGui/qevent.h>
#include <QtGui/QAction>
#include <QtGui/QStackedLayout>
#include <QtGui/QVBoxLayout>

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

FormWindowWidgetStack::FormWindowWidgetStack(QObject *parent) :
    QObject(parent),
    m_formContainer(new QWidget),
    m_formContainerLayout(new QStackedLayout),
    m_layout(new QStackedLayout)
{
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->setStackingMode(QStackedLayout::StackAll);

    // We choose a QStackedLayout as immediate layout for
    // the form windows as it ignores the sizePolicy of
    // its child (for example, Fixed would cause undesired side effects).
    m_formContainerLayout->setMargin(0);
    m_formContainer->setObjectName(QLatin1String("formContainer"));
    m_formContainer->setLayout(m_formContainerLayout);
    m_formContainerLayout->setStackingMode(QStackedLayout::StackAll);
    // System settings might have different background colors, autofill them
    // (affects for example mainwindow status bars)
    m_formContainer->setAutoFillBackground(true);
}

FormWindowWidgetStack::~FormWindowWidgetStack()
{
}

int FormWindowWidgetStack::count() const
{
    return m_tools.count();
}

QDesignerFormWindowToolInterface *FormWindowWidgetStack::currentTool() const
{
    return tool(currentIndex());
}

void FormWindowWidgetStack::setCurrentTool(int index)
{
    const int cnt = count();
    if (index < 0 || index >= cnt) {
        qDebug("FormWindowWidgetStack::setCurrentTool(): invalid index: %d", index);
        return;
    }

    const int cur = currentIndex();
    if (index == cur)
        return;

    if (cur != -1)
        m_tools.at(cur)->deactivated();


    m_layout->setCurrentIndex(index);
    // Show the widget editor and the current tool
    for (int i = 0; i < cnt; i++)
        m_tools.at(i)->editor()->setVisible(i == 0 || i == index);

    QDesignerFormWindowToolInterface *tool = m_tools.at(index);
    tool->activated();

    emit currentToolChanged(index);
}

void FormWindowWidgetStack::setSenderAsCurrentTool()
{
    QDesignerFormWindowToolInterface *tool = 0;
    QAction *action = qobject_cast<QAction*>(sender());
    if (action == 0) {
        qDebug("FormWindowWidgetStack::setSenderAsCurrentTool(): sender is not a QAction");
        return;
    }

    foreach (QDesignerFormWindowToolInterface *t, m_tools) {
        if (action == t->action()) {
            tool = t;
            break;
        }
    }

    if (tool == 0) {
        qDebug("FormWindowWidgetStack::setSenderAsCurrentTool(): unknown tool");
        return;
    }

    setCurrentTool(tool);
}

int FormWindowWidgetStack::indexOf(QDesignerFormWindowToolInterface *tool) const
{
    return m_tools.indexOf(tool);
}

void FormWindowWidgetStack::setCurrentTool(QDesignerFormWindowToolInterface *tool)
{
    int index = indexOf(tool);
    if (index == -1) {
        qDebug("FormWindowWidgetStack::setCurrentTool(): unknown tool");
        return;
    }

    setCurrentTool(index);
}

void FormWindowWidgetStack::setMainContainer(QWidget *w)
{
    // This code is triggered once by the formwindow and
    // by integrations doing "revert to saved". Anything changing?
    const int previousCount = m_formContainerLayout->count();
    QWidget *previousMainContainer = previousCount ? m_formContainerLayout->itemAt(0)->widget() : static_cast<QWidget*>(0);
    if (previousMainContainer == w)
        return;
    // Swap
    if (previousCount)
        delete m_formContainerLayout->takeAt(0);
    if (w)
        m_formContainerLayout->addWidget(w);
}

void FormWindowWidgetStack::addTool(QDesignerFormWindowToolInterface *tool)
{
    if (QWidget *w = tool->editor()) {
        w->setVisible(m_layout->count() == 0); // Initially only form editor is visible
        m_layout->addWidget(w);
    } else {
        // The form editor might not have a tool initially, use dummy. Assert on anything else
        Q_ASSERT(m_tools.empty());
        m_layout->addWidget(m_formContainer);
    }

    m_tools.append(tool);

    connect(tool->action(), SIGNAL(triggered()), this, SLOT(setSenderAsCurrentTool()));
}

QDesignerFormWindowToolInterface *FormWindowWidgetStack::tool(int index) const
{
    if (index < 0 || index >= count())
        return 0;

    return m_tools.at(index);
}

int FormWindowWidgetStack::currentIndex() const
{
    return m_layout->currentIndex();
}

QWidget *FormWindowWidgetStack::defaultEditor() const
{
    if (m_tools.isEmpty())
        return 0;

    return m_tools.at(0)->editor();
}

QLayout *FormWindowWidgetStack::layout() const
{
    return m_layout;
}

QT_END_NAMESPACE
