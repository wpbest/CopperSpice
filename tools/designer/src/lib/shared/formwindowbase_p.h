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

#ifndef FORMWINDOWBASE_H
#define FORMWINDOWBASE_H

#include "shared_global_p.h"

#include <QtDesigner/QDesignerFormWindowInterface>

#include <QtCore/QVariantMap>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE

class QDesignerDnDItemInterface;
class QMenu;
class QtResourceSet;
class QDesignerPropertySheet;

namespace qdesigner_internal {

class QEditorFormBuilder;
class DeviceProfile;
class Grid;

class DesignerPixmapCache;
class DesignerIconCache;
class FormWindowBasePrivate;

class QDESIGNER_SHARED_EXPORT FormWindowBase: public QDesignerFormWindowInterface
{
    Q_OBJECT
public:
    enum HighlightMode  { Restore, Highlight };
    enum SaveResourcesBehaviour  { SaveAll, SaveOnlyUsedQrcFiles, DontSaveQrcFiles };

    explicit FormWindowBase(QDesignerFormEditorInterface *core, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~FormWindowBase();

    QVariantMap formData();
    void setFormData(const QVariantMap &vm);

    // Return contents without warnings. Should be 'contents(bool quiet)'
    QString fileContents() const;

    // Return the widget containing the form. This is used to
    // apply embedded design settings to that are inherited (for example font).
    // These are meant to be applied to the form only and not to the other editors
    // in the widget stack.
    virtual QWidget *formContainer() const = 0;

    // Deprecated
    virtual QPoint grid() const;

    // Deprecated
    virtual void setGrid(const QPoint &grid);

    virtual bool hasFeature(Feature f) const;
    virtual Feature features() const;
    virtual void setFeatures(Feature f);

    const Grid &designerGrid() const;
    void setDesignerGrid(const  Grid& grid);

    bool hasFormGrid() const;
    void setHasFormGrid(bool b);

    bool gridVisible() const;

    SaveResourcesBehaviour saveResourcesBehaviour() const;
    void setSaveResourcesBehaviour(SaveResourcesBehaviour behaviour);

    static const Grid &defaultDesignerGrid();
    static void setDefaultDesignerGrid(const Grid& grid);

    // Overwrite to initialize and return a full popup menu for a managed widget
    virtual QMenu *initializePopupMenu(QWidget *managedWidget);
    // Helper to create a basic popup menu from task menu extensions (internal/public)
    static QMenu *createExtensionTaskMenu(QDesignerFormWindowInterface *fw, QObject *o, bool trailingSeparator = true);

    virtual bool dropWidgets(const QList<QDesignerDnDItemInterface*> &item_list, QWidget *target,
                             const QPoint &global_mouse_pos) = 0;

    // Helper to find the widget at the mouse position with some flags.
    enum WidgetUnderMouseMode { FindSingleSelectionDropTarget, FindMultiSelectionDropTarget };
    QWidget *widgetUnderMouse(const QPoint &formPos, WidgetUnderMouseMode m);

    virtual QWidget *widgetAt(const QPoint &pos) = 0;
    virtual QWidget *findContainer(QWidget *w, bool excludeLayout) const = 0;

    void deleteWidgetList(const QWidgetList &widget_list);

    virtual void highlightWidget(QWidget *w, const QPoint &pos, HighlightMode mode = Highlight) = 0;

    enum PasteMode { PasteAll, PasteActionsOnly };
    virtual void paste(PasteMode pasteMode) = 0;

    // Factory method to create a form builder
    virtual QEditorFormBuilder *createFormBuilder() = 0;

    virtual bool blockSelectionChanged(bool blocked) = 0;
    virtual void emitSelectionChanged() = 0;

    DesignerPixmapCache *pixmapCache() const;
    DesignerIconCache *iconCache() const;
    QtResourceSet *resourceSet() const;
    void setResourceSet(QtResourceSet *resourceSet);
    void addReloadableProperty(QDesignerPropertySheet *sheet, int index);
    void removeReloadableProperty(QDesignerPropertySheet *sheet, int index);
    void addReloadablePropertySheet(QDesignerPropertySheet *sheet, QObject *object);
    void removeReloadablePropertySheet(QDesignerPropertySheet *sheet);
    void reloadProperties();

    void emitWidgetRemoved(QWidget *w);
    void emitObjectRemoved(QObject *o);

    DeviceProfile deviceProfile() const;
    QString styleName() const;
    QString deviceProfileName() const;

    enum LineTerminatorMode {
        LFLineTerminator,
        CRLFLineTerminator,
        NativeLineTerminator =
#if defined (Q_OS_WIN)
            CRLFLineTerminator
#else
            LFLineTerminator
#endif
    };

    void setLineTerminatorMode(LineTerminatorMode mode);
    LineTerminatorMode lineTerminatorMode() const;

    // Connect the 'activated' (doubleclicked) signal of the form window to a
    // slot triggering the default action (of the task menu)
    static void setupDefaultAction(QDesignerFormWindowInterface *fw);

public slots:
    void resourceSetActivated(QtResourceSet *resourceSet, bool resourceSetChanged);

private slots:
    void triggerDefaultAction(QWidget *w);

private:
    void syncGridFeature();

    FormWindowBasePrivate *m_d;    
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // FORMWINDOWBASE_H
