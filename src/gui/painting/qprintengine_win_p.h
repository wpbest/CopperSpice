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

#ifndef QPRINTENGINE_WIN_P_H
#define QPRINTENGINE_WIN_P_H

#ifndef QT_NO_PRINTER

#include "QtGui/qprinter.h"
#include "QtGui/qprintengine.h"
#include "QtGui/qpaintengine.h"
#include "QtCore/qt_windows.h"
#include "qpaintengine_alpha_p.h"

QT_BEGIN_NAMESPACE

class QWin32PrintEnginePrivate;
class QPrinterPrivate;
class QPainterState;

class QWin32PrintEngine : public QAlphaPaintEngine, public QPrintEngine
{
    Q_DECLARE_PRIVATE(QWin32PrintEngine)
public:
    QWin32PrintEngine(QPrinter::PrinterMode mode);

    // override QWin32PaintEngine
    bool begin(QPaintDevice *dev);
    bool end();

    void updateState(const QPaintEngineState &state);

    void updateMatrix(const QTransform &matrix);
    void updateClipPath(const QPainterPath &clip, Qt::ClipOperation op);

    void drawPath(const QPainterPath &path);
    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    void drawTextItem(const QPointF &p, const QTextItem &textItem);

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &p);
    void setProperty(PrintEnginePropertyKey key, const QVariant &value);
    QVariant property(PrintEnginePropertyKey key) const;

    bool newPage();
    bool abort();
    int metric(QPaintDevice::PaintDeviceMetric) const;

    QPrinter::PrinterState printerState() const;

    QPaintEngine::Type type() const { return Windows; }

    HDC getDC() const;
    void releaseDC(HDC) const;

    HDC getPrinterDC() const { return getDC(); }
    void releasePrinterDC(HDC dc) const { releaseDC(dc); }

private:
    friend class QPrintDialog;
    friend class QPageSetupDialog;
};

class QWin32PrintEnginePrivate : public QAlphaPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QWin32PrintEngine)
public:
    QWin32PrintEnginePrivate() :
        hPrinter(0),
        globalDevMode(0),
        devMode(0),
        pInfo(0),
        hdc(0),
        mode(QPrinter::ScreenResolution),
        state(QPrinter::Idle),
        resolution(0),
        pageMarginsSet(false),
        num_copies(1),
        printToFile(false),
        fullPage(false),
        reinit(false),
        has_custom_paper_size(false)
    {
    }

    ~QWin32PrintEnginePrivate();


    /* Reads the default printer name and its driver (printerProgram) into
       the engines private data. */
    void queryDefault();

    /* Initializes the printer data based on the current printer name. This
       function creates a DEVMODE struct, HDC and a printer handle. If these
       structures are already in use, they are freed using release
    */
    void initialize();

    /* Initializes data in the print engine whenever the HDC has been renewed
    */
    void initHDC();

    /* Releases all the handles the printer currently holds, HDC, DEVMODE,
       etc and resets the corresponding members to 0. */
    void release();

    /* Queries the resolutions for the current printer, and returns them
       in a list. */
    QList<QVariant> queryResolutions() const;

    /* Resets the DC with changes in devmode. If the printer is active
       this function only sets the reinit variable to true so it
       is handled in the next begin or newpage. */
    void doReinit();

    /* Used by print/page setup dialogs */
    HGLOBAL *createDevNames();

    void readDevmode(HGLOBAL globalDevmode);
    void readDevnames(HGLOBAL globalDevnames);

    inline bool resetDC() {
        hdc = ResetDC(hdc, devMode);
        return hdc != 0;
    }

    void strokePath(const QPainterPath &path, const QColor &color);
    void fillPath(const QPainterPath &path, const QColor &color);

    void composeGdiPath(const QPainterPath &path);
    void fillPath_dev(const QPainterPath &path, const QColor &color);
    void strokePath_dev(const QPainterPath &path, const QColor &color, qreal width);

    void updateOrigin();

    void initDevRects();
    void setPageMargins(int margin_left, int margin_top, int margin_right, int margin_bottom);
    QRect getPageMargins() const;
    void updateCustomPaperSize();

    // Windows GDI printer references.
    HANDLE hPrinter;

    HGLOBAL globalDevMode;
    DEVMODE *devMode;
    PRINTER_INFO_2 *pInfo;
    HGLOBAL hMem;

    HDC hdc;

    QPrinter::PrinterMode mode;

    // Printer info
    QString name;
    QString program;
    QString port;

    // Document info
    QString docName;
    QString fileName;

    QPrinter::PrinterState state;
    int resolution;

    // This QRect is used to store the exact values
    // entered into the PageSetup Dialog because those are
    // entered in mm but are since converted to device coordinates.
    // If they were to be converted back when displaying the dialog
    // again, there would be inaccuracies so when the user entered 10
    // it may show up as 9.99 the next time the dialog is opened.
    // We don't want that confusion.
    QRect previousDialogMargins;

    bool pageMarginsSet;
    QRect devPageRect;
    QRect devPhysicalPageRect;
    QRect devPaperRect;
    qreal stretch_x;
    qreal stretch_y;
    int origin_x;
    int origin_y;

    int dpi_x;
    int dpi_y;
    int dpi_display;
    int num_copies;

    uint printToFile : 1;
    uint fullPage : 1;
    uint reinit : 1;

    uint complex_xform : 1;
    uint has_pen : 1;
    uint has_brush : 1;
    uint has_custom_paper_size : 1;

    uint txop;

    QColor brush_color;
    QPen pen;
    QColor pen_color;
    QSizeF paper_size;

    QTransform painterMatrix;
    QTransform matrix;
};

QT_END_NAMESPACE

#endif // QT_NO_PRINTER

#endif // QPRINTENGINE_WIN_P_H
