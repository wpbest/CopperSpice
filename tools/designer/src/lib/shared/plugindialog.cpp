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


#include "plugindialog_p.h"
#include "pluginmanager_p.h"
#include "qdesigner_integration_p.h"

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>
#include <QtDesigner/QDesignerWidgetDataBaseInterface>

#include <QtGui/QStyle>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtCore/QFileInfo>
#include <QtCore/QPluginLoader>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

PluginDialog::PluginDialog(QDesignerFormEditorInterface *core, QWidget *parent)
    : QDialog(parent
#ifdef Q_WS_MAC
            , Qt::Tool
#endif
            ), m_core(core)
{
    ui.setupUi(this);

    ui.message->hide();

    const QStringList headerLabels(tr("Components"));

    ui.treeWidget->setAlternatingRowColors(false);
    ui.treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui.treeWidget->setHeaderLabels(headerLabels);
    ui.treeWidget->header()->hide();

    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                            QIcon::Normal, QIcon::On);
    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                            QIcon::Normal, QIcon::Off);
    featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    setWindowTitle(tr("Plugin Information"));
    populateTreeWidget();

    if (qobject_cast<qdesigner_internal::QDesignerIntegration *>(m_core->integration())) {
        QPushButton *updateButton = new QPushButton(tr("Refresh"));
        const QString tooltip = tr("Scan for newly installed custom widget plugins.");
        updateButton->setToolTip(tooltip);
        updateButton->setWhatsThis(tooltip);
        connect(updateButton, SIGNAL(clicked()), this, SLOT(updateCustomWidgetPlugins()));
        ui.buttonBox->addButton(updateButton, QDialogButtonBox::ActionRole);
    }
}

void PluginDialog::populateTreeWidget()
{
    ui.treeWidget->clear();
    QDesignerPluginManager *pluginManager = m_core->pluginManager();
    const QStringList fileNames = pluginManager->registeredPlugins();

    if (!fileNames.isEmpty()) {
        QTreeWidgetItem *topLevelItem = setTopLevelItem(tr("Loaded Plugins"));
        QFont boldFont = topLevelItem->font(0);

        foreach (const QString &fileName, fileNames) {
            QPluginLoader loader(fileName);
            const QFileInfo fileInfo(fileName);

            QTreeWidgetItem *pluginItem = setPluginItem(topLevelItem, fileInfo.fileName(), boldFont);

            if (QObject *plugin = loader.instance()) {
                if (const QDesignerCustomWidgetCollectionInterface *c = qobject_cast<QDesignerCustomWidgetCollectionInterface*>(plugin)) {
                    foreach (const QDesignerCustomWidgetInterface *p, c->customWidgets())
                        setItem(pluginItem, p->name(), p->toolTip(), p->whatsThis(), p->icon());
                } else {
                    if (const QDesignerCustomWidgetInterface *p = qobject_cast<QDesignerCustomWidgetInterface*>(plugin))
                        setItem(pluginItem, p->name(), p->toolTip(), p->whatsThis(), p->icon());
                }
            }
        }
    }

    const QStringList notLoadedPlugins = pluginManager->failedPlugins();
    if (!notLoadedPlugins.isEmpty()) {
        QTreeWidgetItem *topLevelItem = setTopLevelItem(tr("Failed Plugins"));
        const QFont boldFont = topLevelItem->font(0);
        foreach (const QString &plugin, notLoadedPlugins) {
            const QString failureReason = pluginManager->failureReason(plugin);
            QTreeWidgetItem *pluginItem = setPluginItem(topLevelItem, plugin, boldFont);
            setItem(pluginItem, failureReason, failureReason, QString(), QIcon());
        }
    }

    if (ui.treeWidget->topLevelItemCount() == 0) {
        ui.label->setText(tr("Qt Designer couldn't find any plugins"));
        ui.treeWidget->hide();
    } else {
        ui.label->setText(tr("Qt Designer found the following plugins"));
    }
}

QIcon PluginDialog::pluginIcon(const QIcon &icon)
{
    if (icon.isNull())
        return QIcon(QLatin1String(":/trolltech/formeditor/images/qtlogo.png"));

    return icon;
}

QTreeWidgetItem* PluginDialog::setTopLevelItem(const QString &itemName)
{
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(ui.treeWidget);
    topLevelItem->setText(0, itemName);
    ui.treeWidget->setItemExpanded(topLevelItem, true);
    topLevelItem->setIcon(0, style()->standardPixmap(QStyle::SP_DirOpenIcon));

    QFont boldFont = topLevelItem->font(0);
    boldFont.setBold(true);
    topLevelItem->setFont(0, boldFont);

    return topLevelItem;
}

QTreeWidgetItem* PluginDialog::setPluginItem(QTreeWidgetItem *topLevelItem,
                                             const QString &itemName, const QFont &font)
{
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(topLevelItem);
    pluginItem->setFont(0, font);
    pluginItem->setText(0, itemName);
    ui.treeWidget->setItemExpanded(pluginItem, true);
    pluginItem->setIcon(0, style()->standardPixmap(QStyle::SP_DirOpenIcon));

    return pluginItem;
}

void PluginDialog::setItem(QTreeWidgetItem *pluginItem, const QString &name,
                           const QString &toolTip, const QString &whatsThis, const QIcon &icon)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(pluginItem);
    item->setText(0, name);
    item->setToolTip(0, toolTip);
    item->setWhatsThis(0, whatsThis);
    item->setIcon(0, pluginIcon(icon));
}

void  PluginDialog::updateCustomWidgetPlugins()
{
    if (qdesigner_internal::QDesignerIntegration *integration = qobject_cast<qdesigner_internal::QDesignerIntegration *>(m_core->integration())) {
        const int before = m_core->widgetDataBase()->count();
        integration->updateCustomWidgetPlugins();
        const int after = m_core->widgetDataBase()->count();
        if (after >  before) {
            ui.message->setText(tr("New custom widget plugins have been found."));
            ui.message->show();
        } else {
            ui.message->setText(QString());
        }
        populateTreeWidget();
    }
}

}

QT_END_NAMESPACE
