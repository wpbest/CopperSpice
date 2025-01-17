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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//


#ifndef ICONSELECTOR_H
#define ICONSELECTOR_H

#include "shared_global_p.h"

#include <QtGui/QWidget>
#include <QtGui/QDialog>

#include <QtCore/QScopedPointer>

QT_BEGIN_NAMESPACE

class QtResourceModel;
class QDesignerFormEditorInterface;
class QDesignerDialogGuiInterface;
class QDesignerResourceBrowserInterface;

namespace qdesigner_internal {

class DesignerIconCache;
class DesignerPixmapCache;
class PropertySheetIconValue;
struct IconThemeEditorPrivate;

// Resource Dialog that embeds the language-dependent resource widget as returned by the language extension
class QDESIGNER_SHARED_EXPORT LanguageResourceDialog : public QDialog
{
    Q_OBJECT

    explicit LanguageResourceDialog(QDesignerResourceBrowserInterface *rb, QWidget *parent = 0);

public:
    virtual ~LanguageResourceDialog();
    // Factory: Returns 0 if the language extension does not provide a resource browser.
    static LanguageResourceDialog* create(QDesignerFormEditorInterface *core, QWidget *parent);

    void setCurrentPath(const QString &filePath);
    QString currentPath() const;

private:
    QScopedPointer<class LanguageResourceDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(LanguageResourceDialog)
    Q_DISABLE_COPY(LanguageResourceDialog)
    Q_PRIVATE_SLOT(d_func(), void slotAccepted())
    Q_PRIVATE_SLOT(d_func(), void slotPathChanged(QString))

};

class QDESIGNER_SHARED_EXPORT IconSelector: public QWidget
{
    Q_OBJECT
public:
    IconSelector(QWidget *parent = 0);
    virtual ~IconSelector();

    void setFormEditor(QDesignerFormEditorInterface *core); // required for dialog gui.
    void setIconCache(DesignerIconCache *iconCache);
    void setPixmapCache(DesignerPixmapCache *pixmapCache);

    void setIcon(const PropertySheetIconValue &icon);
    PropertySheetIconValue icon() const;

    // Check whether a pixmap may be read
    enum CheckMode { CheckFast, CheckFully };
    static bool checkPixmap(const QString &fileName, CheckMode cm = CheckFully, QString *errorMessage = 0);
    // Choose a pixmap from file
    static QString choosePixmapFile(const QString &directory, QDesignerDialogGuiInterface *dlgGui, QWidget *parent);
    // Choose a pixmap from resource; use language-dependent resource browser if present
    static QString choosePixmapResource(QDesignerFormEditorInterface *core, QtResourceModel *resourceModel, const QString &oldPath, QWidget *parent);

signals:
    void iconChanged(const PropertySheetIconValue &icon);
private:
    QScopedPointer<class IconSelectorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IconSelector)
    Q_DISABLE_COPY(IconSelector)

    Q_PRIVATE_SLOT(d_func(), void slotStateActivated())
    Q_PRIVATE_SLOT(d_func(), void slotSetActivated())
    Q_PRIVATE_SLOT(d_func(), void slotSetResourceActivated())
    Q_PRIVATE_SLOT(d_func(), void slotSetFileActivated())
    Q_PRIVATE_SLOT(d_func(), void slotResetActivated())
    Q_PRIVATE_SLOT(d_func(), void slotResetAllActivated())
    Q_PRIVATE_SLOT(d_func(), void slotUpdate())
};

// IconThemeEditor: Let's the user input theme icon names and shows a preview label.
class QDESIGNER_SHARED_EXPORT IconThemeEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme DESIGNABLE true)
public:
    explicit IconThemeEditor(QWidget *parent = 0, bool wantResetButton = true);
    virtual ~IconThemeEditor();

    QString theme() const;
    void setTheme(const QString &theme);

signals:
    void edited(const QString &);

public slots:
    void reset();

private slots:
    void slotChanged(const QString &);

private:
    void updatePreview(const QString &);

    QScopedPointer<IconThemeEditorPrivate> d;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // ICONSELECTOR_H

