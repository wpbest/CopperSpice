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

#ifndef ASSISTANTCLIENT_H
#define ASSISTANTCLIENT_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

class QProcess;
class QString;

class AssistantClient
{
    AssistantClient(const AssistantClient &);
    AssistantClient &operator=(const AssistantClient &);

public:
    AssistantClient();
    ~AssistantClient();

    bool showPage(const QString &path, QString *errorMessage);
    bool activateIdentifier(const QString &identifier, QString *errorMessage);
    bool activateKeyword(const QString &keyword, QString *errorMessage);

    bool isRunning() const;

    static QString documentUrl(const QString &prefix, int qtVersion = 0);
    // Root of the Qt Designer documentation
    static QString designerManualUrl(int qtVersion = 0);
    // Root of the Qt Reference documentation
    static QString qtReferenceManualUrl(int qtVersion = 0);

private:
    static QString binary();
    bool sendCommand(const QString &cmd, QString *errorMessage);
    bool ensureRunning(QString *errorMessage);

    QProcess *m_process;
};

QT_END_NAMESPACE

#endif // ASSISTANTCLIENT_H
