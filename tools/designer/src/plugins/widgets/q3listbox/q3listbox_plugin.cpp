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

#include "q3listbox_plugin.h"
#include "q3listbox_extrainfo.h"

#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>

#include <QtCore/qplugin.h>
#include <QtGui/QIcon>
#include <Qt3Support/Q3ListBox>

QT_BEGIN_NAMESPACE

Q3ListBoxPlugin::Q3ListBoxPlugin(const QIcon &icon, QObject *parent)
        : QObject(parent), m_initialized(false), m_icon(icon)
{}

QString Q3ListBoxPlugin::name() const
{ return QLatin1String("Q3ListBox"); }

QString Q3ListBoxPlugin::group() const
{ return QLatin1String("Qt 3 Support"); }

QString Q3ListBoxPlugin::toolTip() const
{ return QString(); }

QString Q3ListBoxPlugin::whatsThis() const
{ return QString(); }

QString Q3ListBoxPlugin::includeFile() const
{ return QLatin1String("q3listbox.h"); }

QIcon Q3ListBoxPlugin::icon() const
{ return m_icon; }

bool Q3ListBoxPlugin::isContainer() const
{ return false; }

QWidget *Q3ListBoxPlugin::createWidget(QWidget *parent)
{ return new Q3ListBox(parent); }

bool Q3ListBoxPlugin::isInitialized() const
{ return m_initialized; }

void Q3ListBoxPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);

    if (m_initialized)
        return;

    QExtensionManager *mgr = core->extensionManager();
    Q_ASSERT(mgr != 0);

    mgr->registerExtensions(new Q3ListBoxExtraInfoFactory(core, mgr), Q_TYPEID(QDesignerExtraInfoExtension));

    m_initialized = true;
}

QString Q3ListBoxPlugin::codeTemplate() const
{ return QString(); }

QString Q3ListBoxPlugin::domXml() const
{ return QLatin1String("\
<ui language=\"c++\">\
    <widget class=\"Q3ListBox\" name=\"listBox\">\
        <property name=\"geometry\">\
            <rect>\
                <x>0</x>\
                <y>0</y>\
                <width>100</width>\
                <height>80</height>\
            </rect>\
        </property>\
    </widget>\
</ui>");
}



QT_END_NAMESPACE
