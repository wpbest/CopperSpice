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

#include "newdynamicpropertydialog.h"
#include "ui_newdynamicpropertydialog.h"
#include <abstractdialoggui_p.h>
#include <qdesigner_propertysheet_p.h>

#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

NewDynamicPropertyDialog::NewDynamicPropertyDialog(QDesignerDialogGuiInterface *dialogGui,
                                                       QWidget *parent)   :
    QDialog(parent),
    m_dialogGui(dialogGui),
    m_ui(new Ui::NewDynamicPropertyDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->m_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_ui->m_comboBox->addItem(QLatin1String("String"),      QVariant(QVariant::String));
    m_ui->m_comboBox->addItem(QLatin1String("StringList"),  QVariant(QVariant::StringList));
    m_ui->m_comboBox->addItem(QLatin1String("Char"),        QVariant(QVariant::Char));
    m_ui->m_comboBox->addItem(QLatin1String("ByteArray"),   QVariant(QVariant::ByteArray));
    m_ui->m_comboBox->addItem(QLatin1String("Url"),         QVariant(QVariant::Url));
    m_ui->m_comboBox->addItem(QLatin1String("Bool"),        QVariant(QVariant::Bool));
    m_ui->m_comboBox->addItem(QLatin1String("Int"),         QVariant(QVariant::Int));
    m_ui->m_comboBox->addItem(QLatin1String("UInt"),        QVariant(QVariant::UInt));
    m_ui->m_comboBox->addItem(QLatin1String("LongLong"),    QVariant(QVariant::LongLong));
    m_ui->m_comboBox->addItem(QLatin1String("ULongLong"),   QVariant(QVariant::ULongLong));
    m_ui->m_comboBox->addItem(QLatin1String("Double"),      QVariant(QVariant::Double));
    m_ui->m_comboBox->addItem(QLatin1String("Size"),        QVariant(QVariant::Size));
    m_ui->m_comboBox->addItem(QLatin1String("SizeF"),       QVariant(QVariant::SizeF));
    m_ui->m_comboBox->addItem(QLatin1String("Point"),       QVariant(QVariant::Point));
    m_ui->m_comboBox->addItem(QLatin1String("PointF"),      QVariant(QVariant::PointF));
    m_ui->m_comboBox->addItem(QLatin1String("Rect"),        QVariant(QVariant::Rect));
    m_ui->m_comboBox->addItem(QLatin1String("RectF"),       QVariant(QVariant::RectF));
    m_ui->m_comboBox->addItem(QLatin1String("Date"),        QVariant(QVariant::Date));
    m_ui->m_comboBox->addItem(QLatin1String("Time"),        QVariant(QVariant::Time));
    m_ui->m_comboBox->addItem(QLatin1String("DateTime"),    QVariant(QVariant::DateTime));
    m_ui->m_comboBox->addItem(QLatin1String("Font"),        QVariant(QVariant::Font));
    m_ui->m_comboBox->addItem(QLatin1String("Palette"),     QVariant(QVariant::Palette));
    m_ui->m_comboBox->addItem(QLatin1String("Color"),       QVariant(QVariant::Color));
    m_ui->m_comboBox->addItem(QLatin1String("Pixmap"),      QVariant(QVariant::Pixmap));
    m_ui->m_comboBox->addItem(QLatin1String("Icon"),        QVariant(QVariant::Icon));
    m_ui->m_comboBox->addItem(QLatin1String("Cursor"),      QVariant(QVariant::Cursor));
    m_ui->m_comboBox->addItem(QLatin1String("SizePolicy"),  QVariant(QVariant::SizePolicy));
    m_ui->m_comboBox->addItem(QLatin1String("KeySequence"), QVariant(QVariant::KeySequence));

    m_ui->m_comboBox->setCurrentIndex(0); // String
    setOkButtonEnabled(false);
}

void NewDynamicPropertyDialog::setOkButtonEnabled(bool e)
{
    m_ui->m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(e);
}

NewDynamicPropertyDialog::~NewDynamicPropertyDialog()
{
    delete m_ui;
}

void NewDynamicPropertyDialog::setReservedNames(const QStringList &names)
{
    m_reservedNames = names;
}

void NewDynamicPropertyDialog::setPropertyType(QVariant::Type t)
{
    const int index = m_ui->m_comboBox->findData(QVariant(t));
    if (index != -1)
        m_ui->m_comboBox->setCurrentIndex(index);
}

QString NewDynamicPropertyDialog::propertyName() const
{
    return m_ui->m_lineEdit->text();
}

QVariant NewDynamicPropertyDialog::propertyValue() const
{
    const int index = m_ui->m_comboBox->currentIndex();
    if (index == -1)
        return QVariant();
    return m_ui->m_comboBox->itemData(index);
}

void NewDynamicPropertyDialog::information(const QString &message)
{
    m_dialogGui->message(this, QDesignerDialogGuiInterface::PropertyEditorMessage, QMessageBox::Information, tr("Set Property Name"), message);
}

void NewDynamicPropertyDialog::nameChanged(const QString &s)
{
    setOkButtonEnabled(!s.isEmpty());
}

bool NewDynamicPropertyDialog::validatePropertyName(const QString& name)
{
    if (m_reservedNames.contains(name)) {
        information(tr("The current object already has a property named '%1'.\nPlease select another, unique one.").arg(name));
        return false;
    }
    if (!QDesignerPropertySheet::internalDynamicPropertiesEnabled() && name.startsWith(QLatin1String("_q_"))) {
        information(tr("The '_q_' prefix is reserved for the Qt library.\nPlease select another name."));
        return false;
    }
    return true;
}

void NewDynamicPropertyDialog::on_m_buttonBox_clicked(QAbstractButton *btn)
{
    const int role = m_ui->m_buttonBox->buttonRole(btn);
    switch (role) {
        case QDialogButtonBox::RejectRole:
            reject();
            break;
        case QDialogButtonBox::AcceptRole:
            if (validatePropertyName(propertyName()))
                accept();
            break;
    }
}
}

QT_END_NAMESPACE
