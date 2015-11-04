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

#ifndef QVFB_H
#define QVFB_H

#include <QMainWindow>
#include <QStringList>
#include "qvfbview.h"

QT_BEGIN_NAMESPACE

class QVFbAbstractView;
class QVFbRateDialog;
class QPopupMenu;
class QMenuData;
class QAction;
class Config;
class DeviceSkin;
class QVFb;
class QLabel;
class QMenu;
class QScrollArea;

class Zoomer : public QWidget {
    Q_OBJECT
public:
    Zoomer(QVFb* target);

private slots:
    void zoom(int);

private:
    QVFb *qvfb;
    QLabel *label;
};

class QVFb: public QMainWindow
{
    Q_OBJECT
public:
    enum DisplayType { QWS, X11 };

    QVFb( int display_id, int w, int h, int d, int r, const QString &skin, DisplayType displayType, QWidget *parent = 0, Qt::WindowFlags wflags = 0 );
    ~QVFb();

    void enableCursor( bool e );

    QSize sizeHint() const;

public slots:
     void popupMenu();

protected slots:
    void saveImage();
    void toggleAnimation();
    void toggleCursor();
    void changeRate();
    void setRate(int);
    void about();

    void configure();
    void skinConfigChosen(int i);
    void chooseSize(const QSize& sz);
    void chooseDepth(int depth, QVFbView::PixelFormat displayFormat);

    void setZoom1();
    void setZoom2();
    void setZoom3();
    void setZoom4();
    void setZoomHalf();
    void setZoom075();

    void setZoom();

    void setRot0();
    void setRot90();
    void setRot180();
    void setRot270();

public slots:
    void setZoom(double);
    void setRotation(QVFbView::Rotation);

protected:
    template <typename T>
    void createMenu(T *menu);
    QMenu* createFileMenu();
    QMenu* createViewMenu();
    QMenu* createHelpMenu();

private:
    void findSkins(const QString &currentSkin);
    void init( int display_id, int w, int h, int d, int r, const QString& skin );
    DeviceSkin *skin;
    double skinscaleH,skinscaleV;
    QVFbAbstractView *view;
    QVFbAbstractView *secondaryView;
    QVFbRateDialog *rateDlg;
    QMenu *viewMenu;
    QAction *cursorAction;
    Config* config;
    QStringList skinnames;
    QStringList skinfiles;
    int currentSkinIndex;
    Zoomer *zoomer;
    QScrollArea* scroller;
    DisplayType displayType;

    int refreshRate;
private slots:
    void setGamma400(int n);
    void setR400(int n);
    void setG400(int n);
    void setB400(int n);
    void updateGammaLabels();
};

QT_END_NAMESPACE

#endif
