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

#ifndef QSCRIPTDEBUGGERSCRIPTSMODEL_P_H
#define QSCRIPTDEBUGGERSCRIPTSMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qpair.h>

#include "qscriptscriptdata_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerScriptsModelPrivate;
class QScriptDebuggerScriptsModel
    : public QAbstractItemModel
{
    CS_OBJECT(QScriptDebuggerScriptsModel)
public:
    QScriptDebuggerScriptsModel(QObject *parent = 0);
    ~QScriptDebuggerScriptsModel();

    void removeScript(qint64 id);
    void addScript(qint64 id, const QScriptScriptData &data);
    void addExtraScriptInfo(
        qint64 id, const QMap<QString, int> &functionsInfo,
        const QSet<int> &executableLineNumbers);
    void commit();

    QScriptScriptData scriptData(qint64 id) const;
    QScriptScriptMap scripts() const;
    qint64 resolveScript(const QString &fileName) const;
    QSet<int> executableLineNumbers(qint64 scriptId) const;

    QModelIndex indexFromScriptId(qint64 id) const;
    qint64 scriptIdFromIndex(const QModelIndex &index) const;
    QPair<QString, int> scriptFunctionInfoFromIndex(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerScriptsModel)
    Q_DISABLE_COPY(QScriptDebuggerScriptsModel)
};

QT_END_NAMESPACE

#endif
