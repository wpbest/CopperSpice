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

#ifndef QGUIPLATFORMPlugin_P_H
#define QGUIPLATFORMPlugin_P_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>
#include <QtGui/qdialog.h>

QT_BEGIN_NAMESPACE

class QStyle;
class QPalette;
class QIcon;
class QFileDialog;
class QColorDialog;
class QFileInfo;

struct Q_GUI_EXPORT QGuiPlatformPluginInterface  : public QFactoryInterface
{
};

#define QGuiPlatformPluginInterface_iid "com.copperspice.QGuiPlatformPluginInterface"

CS_DECLARE_INTERFACE(QGuiPlatformPluginInterface, QGuiPlatformPluginInterface_iid)

class Q_GUI_EXPORT QGuiPlatformPlugin : public QObject, public QGuiPlatformPluginInterface
{
    CS_OBJECT(QGuiPlatformPlugin)
    CS_INTERFACES(QGuiPlatformPluginInterface, QFactoryInterface)

    public:
        explicit QGuiPlatformPlugin(QObject *parent = 0);
        ~QGuiPlatformPlugin();

        virtual QStringList keys() const {  return QStringList() << QLatin1String("default");  };

        virtual QString styleName();
        virtual QPalette palette();
        virtual QString systemIconThemeName();
        virtual QStringList iconThemeSearchPaths();
        virtual QIcon fileSystemIcon(const QFileInfo &);

        enum PlatformHint { PH_ToolButtonStyle, PH_ToolBarIconSize, PH_ItemView_ActivateItemOnSingleClick };
        virtual int platformHint(PlatformHint hint);

        virtual void fileDialogDelete(QFileDialog *) {}
        virtual bool fileDialogSetVisible(QFileDialog *, bool) { return false; }
        virtual QDialog::DialogCode fileDialogResultCode(QFileDialog *) { return QDialog::Rejected; }
        virtual void fileDialogSetDirectory(QFileDialog *, const QString &) {}
        virtual QString fileDialogDirectory(const QFileDialog *) const { return QString(); }
        virtual void fileDialogSelectFile(QFileDialog *, const QString &) {}
        virtual QStringList fileDialogSelectedFiles(const QFileDialog *) const { return QStringList(); }
        virtual void fileDialogSetFilter(QFileDialog *) {}
        virtual void fileDialogSetNameFilters(QFileDialog *, const QStringList &) {}
        virtual void fileDialogSelectNameFilter(QFileDialog *, const QString &) {}
        virtual QString fileDialogSelectedNameFilter(const QFileDialog *) const { return QString(); }

        virtual void colorDialogDelete(QColorDialog *) {}
        virtual bool colorDialogSetVisible(QColorDialog *, bool) { return false; }
        virtual void colorDialogSetCurrentColor(QColorDialog *, const QColor &) {}
};

//internal
QGuiPlatformPlugin *qt_guiPlatformPlugin();

QT_END_NAMESPACE

#endif // QGUIPLATFORMPLUGIN_H
