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

#ifndef FILTERPAGE_H
#define FILTERPAGE_H

#include <QtGui/QWizardPage>
#include "ui_filterpage.h"

QT_BEGIN_NAMESPACE

struct CustomFilter
{
    QString name;
    QStringList filterAttributes;
};

class FilterPage : public QWizardPage
{
    Q_OBJECT

public:
    FilterPage(QWidget *parent = 0);
    QStringList filterAttributes() const;
    QList<CustomFilter> customFilters() const;

private slots:
    void addFilter();
    void removeFilter();

private:
    bool validatePage();

    Ui::FilterPage m_ui;
    QStringList m_filterAttributes;
    QList<CustomFilter> m_customFilters;
};

QT_END_NAMESPACE

#endif
