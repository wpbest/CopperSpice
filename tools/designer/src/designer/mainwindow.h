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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QList>
#include <QtGui/QMdiArea>

QT_BEGIN_NAMESPACE

class QDesignerActions;
class QDesignerWorkbench;
class QDesignerToolWindow;
class QDesignerFormWindow;
class QDesignerSettings;

class QtToolBarManager;
class QToolBar;
class QMdiArea;
class QMenu;
class QByteArray;
class QMimeData;

/* A main window that has a configureable policy on handling close events. If
 * enabled, it can forward the close event to external handlers.
 * Base class for windows that can switch roles between tool windows
 * and main windows. */

class MainWindowBase: public QMainWindow
{
    Q_DISABLE_COPY(MainWindowBase)
    Q_OBJECT
protected:
    explicit MainWindowBase(QWidget *parent = 0, Qt::WindowFlags flags = Qt::Window);

public:
    enum CloseEventPolicy {
        /* Always accept close events */
        AcceptCloseEvents,
        /* Emit a signal with the event, have it handled elsewhere */
        EmitCloseEventSignal };

    CloseEventPolicy closeEventPolicy() const { return m_policy; }
    void setCloseEventPolicy(CloseEventPolicy pol) { m_policy = pol; }

    static QList<QToolBar *> createToolBars(const QDesignerActions *actions, bool singleToolBar);
    static QString mainWindowTitle();

    // Use the minor Qt version as settings versions to avoid conflicts
    static int settingsVersion();

signals:
    void closeEventReceived(QCloseEvent *e);

protected:
    virtual void closeEvent(QCloseEvent *e);
private:
    CloseEventPolicy m_policy;
};

/* An MdiArea that listens for desktop file manager file drop events and emits
 * a signal to open a dropped file. */
class DockedMdiArea : public QMdiArea
{
    Q_DISABLE_COPY(DockedMdiArea)
    Q_OBJECT
public:
    explicit DockedMdiArea(const QString &extension, QWidget *parent = 0);

signals:
    void fileDropped(const QString &);

protected:
    bool event (QEvent *event);

private:
    QStringList uiFiles(const QMimeData *d) const;

    const QString m_extension;
};

// Convenience class that manages a QtToolBarManager and an action to trigger
// it on a mainwindow.
class ToolBarManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ToolBarManager)
public:
    explicit ToolBarManager(QMainWindow *configureableMainWindow,
                            QWidget *parent,
                            QMenu *toolBarMenu,
                            const QDesignerActions *actions,
                            const QList<QToolBar *> &toolbars,
                            const QList<QDesignerToolWindow*> &toolWindows);

    QByteArray saveState(int version = 0) const;
    bool restoreState(const QByteArray &state, int version = 0);

private slots:
    void configureToolBars();
    void updateToolBarMenu();

private:
    QMainWindow *m_configureableMainWindow;
    QWidget *m_parent;
    QMenu *m_toolBarMenu;
    QtToolBarManager *m_manager;
    QAction *m_configureAction;
    QList<QToolBar *> m_toolbars;
};

/* Main window to be used for docked mode */
class DockedMainWindow : public MainWindowBase {
    Q_OBJECT
    Q_DISABLE_COPY(DockedMainWindow)
public:
    typedef QList<QDesignerToolWindow*> DesignerToolWindowList;
    typedef QList<QDockWidget *> DockWidgetList;

    explicit DockedMainWindow(QDesignerWorkbench *wb,
                              QMenu *toolBarMenu,
                              const DesignerToolWindowList &toolWindows);

    // Create a MDI subwindow for the form.
    QMdiSubWindow *createMdiSubWindow(QWidget *fw, Qt::WindowFlags f, const QKeySequence &designerCloseActionShortCut);

    QMdiArea *mdiArea() const;

    DockWidgetList addToolWindows(const DesignerToolWindowList &toolWindows);

    void restoreSettings(const QDesignerSettings &s, const DockWidgetList &dws, const QRect &desktopArea);
    void saveSettings(QDesignerSettings &) const;

signals:
    void fileDropped(const QString &);
    void formWindowActivated(QDesignerFormWindow *);

private slots:
    void slotSubWindowActivated(QMdiSubWindow*);

private:
    ToolBarManager *m_toolBarManager;
};

QT_END_NAMESPACE

#endif // MAINWINDOW_H
