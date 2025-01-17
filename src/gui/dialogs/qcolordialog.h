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

#ifndef QCOLORDIALOG_H
#define QCOLORDIALOG_H

#include <QtGui/qdialog.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_COLORDIALOG

class QColorDialogPrivate;

class Q_GUI_EXPORT QColorDialog : public QDialog
{
    CS_OBJECT(QColorDialog)
    Q_DECLARE_PRIVATE(QColorDialog)

    GUI_CS_ENUM(ColorDialogOption)
    GUI_CS_PROPERTY_READ(currentColor, currentColor)
    GUI_CS_PROPERTY_WRITE(currentColor, setCurrentColor)
    GUI_CS_PROPERTY_NOTIFY(currentColor, currentColorChanged)
    GUI_CS_PROPERTY_READ(options, options)
    GUI_CS_PROPERTY_WRITE(options, setOptions)

public:
    enum ColorDialogOption {
        ShowAlphaChannel    = 0x00000001,
        NoButtons           = 0x00000002,
        DontUseNativeDialog = 0x00000004
    };

    using ColorDialogOptions = QFlags<ColorDialogOption>;

    explicit QColorDialog(QWidget *parent = 0);
    explicit QColorDialog(const QColor &initial, QWidget *parent = 0);
    ~QColorDialog();

    void setCurrentColor(const QColor &color);
    QColor currentColor() const;

    QColor selectedColor() const;

    void setOption(ColorDialogOption option, bool on = true);
    bool testOption(ColorDialogOption option) const;
    void setOptions(ColorDialogOptions options);
    ColorDialogOptions options() const;

    using QDialog::open;

    void open(QObject *receiver, const char *member);

    void setVisible(bool visible);

    // ### Qt5/merge overloads with title = QString()
    static QColor getColor(const QColor &initial, QWidget *parent, const QString &title,
                           ColorDialogOptions options = 0);
    static QColor getColor(const QColor &initial = Qt::white, QWidget *parent = 0);

    // obsolete
    static QRgb getRgba(QRgb rgba = 0xffffffff, bool *ok = 0, QWidget *parent = 0);

    // ### Qt5/use QColor in signatures
    static int customCount();
    static QRgb customColor(int index);
    static void setCustomColor(int index, QRgb color);
    static void setStandardColor(int index, QRgb color);

    GUI_CS_SIGNAL_1(Public, void currentColorChanged(const QColor & color))
    GUI_CS_SIGNAL_2(currentColorChanged,color) 
   
    GUI_CS_SIGNAL_1(Public, void colorSelected(const QColor & color))
    GUI_CS_SIGNAL_2(colorSelected,color) 

protected:
    void changeEvent(QEvent *event);
    void done(int result);

private:
    Q_DISABLE_COPY(QColorDialog)

    GUI_CS_SLOT_1(Private, void _q_addCustom())
    GUI_CS_SLOT_2(_q_addCustom)

    GUI_CS_SLOT_1(Private, void _q_newHsv(int h,int s,int v))
    GUI_CS_SLOT_2(_q_newHsv)

    GUI_CS_SLOT_1(Private, void _q_newColorTypedIn(const QRgb &rgb))
    GUI_CS_SLOT_2(_q_newColorTypedIn)

    GUI_CS_SLOT_1(Private, void _q_newCustom(int un_named_arg1,int un_named_arg2))
    GUI_CS_SLOT_2(_q_newCustom)

    GUI_CS_SLOT_1(Private, void _q_newStandard(int un_named_arg1,int un_named_arg2))
    GUI_CS_SLOT_2(_q_newStandard)

#if defined(Q_WS_MAC)
    GUI_CS_SLOT_1(Private, void _q_macRunNativeAppModalPanel())
    GUI_CS_SLOT_2(_q_macRunNativeAppModalPanel)
#endif

    friend class QColorShower;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QColorDialog::ColorDialogOptions)

#endif // QT_NO_COLORDIALOG

QT_END_NAMESPACE

#endif // QCOLORDIALOG_H
