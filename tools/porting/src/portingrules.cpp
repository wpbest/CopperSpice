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

#include "portingrules.h"
#include "logger.h"
#include "qtsimplexml.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>

QT_BEGIN_NAMESPACE

PortingRules *PortingRules::theInstance  = 0;

void PortingRules::createInstance(QString xmlFilePath)
{
    deleteInstance();
    theInstance  = new PortingRules(xmlFilePath);
}

PortingRules *PortingRules::instance()
{
    if(theInstance) {
        return theInstance;
    } else {
        qWarning("Error: must create a PortingRules instance with createInstance() before calling instance()\n");
        return 0;
    }
}

void PortingRules::deleteInstance()
{
    if(theInstance) {
        delete theInstance;
        theInstance = 0;
    }
}

PortingRules::PortingRules(QString xmlFilePath)
{
    parseXml(xmlFilePath);
}

QList<TokenReplacement*> PortingRules::getTokenReplacementRules()
{
    if(tokenRules.isEmpty())
         addLogWarning(QLatin1String("Warning: token rules list is empty"));
    return tokenRules;
}

QStringList PortingRules::getHeaderList(QtVersion qtVersion)
{
    if(qt3Headers.isEmpty() || qt4Headers.isEmpty())
         addLogWarning(QLatin1String("Warning: headers list is empty"));

    if (qtVersion==Qt3)
        return qt3Headers;
    else //Qt4
        return qt4Headers;
}

QHash<QByteArray, QByteArray> PortingRules::getNeededHeaders()
{
    if(neededHeaders.isEmpty())
         addLogWarning(QLatin1String("Warning: needed headers list is empty"));
    return neededHeaders;
}

QStringList PortingRules::getInheritsQt()
{
    if(tokenRules.isEmpty())
        addLogWarning(QLatin1String("Warning: inheritsQtClass list is empty"));
    return inheritsQtClass;
}

QHash<QByteArray, QByteArray> PortingRules::getClassLibraryList()
{
    if(classLibraryList.isEmpty())
        addLogWarning(QLatin1String("Warning: classLibraryList list is empty"));
    return classLibraryList;
}

QHash<QByteArray, QByteArray> PortingRules::getHeaderReplacements()
{
    return headerReplacements;
}

/*
    Loads rule xml file given by fileName, and sets up data structures.
    The rules can generally be divided into to types, replacement rules and
    info rules.

    Replacement rules has the form Qt3Symobl -> Qt4Symbol
    Info rules includes the NeedHeader, Qt3Header, Qt4Header, InhertitsQt
    rule types.
*/
void PortingRules::parseXml(QString fileName)
{
    QtSimpleXml *xmlPointer = loadXml(fileName);
    QtSimpleXml &xml = *xmlPointer;

    int ruleCount = xml[QLatin1String("Rules")].numChildren();
    ++ruleCount;

    for(int rule=0; rule<ruleCount; ++rule) {
        QtSimpleXml &currentRule = xml[QLatin1String("Rules")][rule];
        QString ruleType = currentRule.attribute(QLatin1String("Type"));

        if(isReplacementRule(ruleType)) {
            QString qt3Symbol = currentRule[QLatin1String("Qt3")].text();
            QString qt4Symbol = currentRule[QLatin1String("Qt4")].text();

            QString disable = currentRule.attribute(QLatin1String("Disable"));
            if(disable == QLatin1String("True") || disable == QLatin1String("true")) {
                disableRule(currentRule);
                continue;
            }

            if (isRuleDisabled(currentRule))
                continue;

            if(ruleType == QLatin1String("RenamedHeader")) {
              headerReplacements.insert(qt3Symbol.toLatin1(), qt4Symbol.toLatin1());
            } else if(ruleType == QLatin1String("RenamedClass") || ruleType == QLatin1String("RenamedToken") ) {
                tokenRules.append(new ClassNameReplacement(
                        qt3Symbol.toLatin1(), qt4Symbol.toLatin1()));
            } else if(ruleType == QLatin1String("RenamedEnumvalue") || ruleType == QLatin1String("RenamedType") ||
                    ruleType == QLatin1String("RenamedQtSymbol") ) {
                checkScopeAddRule(currentRule);
            }
        } else if(ruleType == QLatin1String("NeedHeader")) {
            const QByteArray className = currentRule[QLatin1String("Class")].text().toLatin1();
            const QByteArray headerName = currentRule[QLatin1String("Header")].text().toLatin1();
            neededHeaders.insert(className, headerName);
        }
        else if(ruleType == QLatin1String("qt3Header")) {
            qt3Headers += currentRule.text();
        }
        else if(ruleType == QLatin1String("qt4Header")) {
            qt4Headers += currentRule.text();
        }
        else if(ruleType == QLatin1String("InheritsQt")) {
            inheritsQtClass += currentRule.text();
        }
        else if(ruleType == QLatin1String("Qt4Class")) {
            // Get library name, make it lowercase and chop of the "Qt" prefix.
            const QByteArray libraryName = currentRule[QLatin1String("Library")].text().toLatin1().toLower().mid(2);
            classLibraryList.insert(currentRule[QLatin1String("Name")].text().toLatin1(), libraryName);
        }
    }

    QString includeFile = xml[QLatin1String("Rules")][QLatin1String("Include")].text();

    if(!includeFile.isNull()) {
        QString resolvedIncludeFile = resolveFileName(fileName, includeFile);
        if (!resolvedIncludeFile.isEmpty())
            parseXml(resolvedIncludeFile);
    }

    delete xmlPointer;
}

/*
    Check if the rule in currentRule describes a qualified name
    (like QButton::ToggleState). If so, create a scoped ScopedTokenReplacement,
    else create a GenericTokenReplacement
*/
void PortingRules::checkScopeAddRule(/*const */QtSimpleXml &currentRule)
{
    QByteArray oldToken = currentRule[QLatin1String("Qt3")].text().toLatin1();
    QByteArray newToken = currentRule[QLatin1String("Qt4")].text().toLatin1();

    if (oldToken.contains(QByteArray("::")))
        tokenRules.append(new ScopedTokenReplacement(oldToken, newToken));
    else
        tokenRules.append(new GenericTokenReplacement(oldToken, newToken));
}

/*
    Loads the xml-file given by fileName into a new'ed QtSimpleXml, which is
    returned by pointer.
*/
QtSimpleXml *PortingRules::loadXml(const QString fileName) const
{
    QFile f(fileName);
    if(!f.open(QIODevice::ReadOnly)) {
        qFatal("Could not find rule file %s", fileName.toLatin1().constData());
    }
    QtSimpleXml *xml = new QtSimpleXml();
    if(!xml->setContent(&f))
        addLogError(QLatin1String("Xml parsing failed: ") + xml->errorString());

    return xml;
}

/*
    Resolves includeFilePath against currentFilePath. If currentFilePath
    contains foo/bar.xml, and includeFilePath contains bar2.xml, the returned
    result will be foo/bar2.xml. If includeFilePath is absolute, it is returned
    unmodified.
*/
QString PortingRules::resolveFileName(const QString currentFilePath,
                                      const QString includeFilePath) const
{
    if(QFileInfo(includeFilePath).isAbsolute())
        return includeFilePath;
    QString relativeDirectory = QFileInfo(currentFilePath).dir().dirName();
    QString testFileName = relativeDirectory + QLatin1String("/") + includeFilePath;
    if (QFile::exists(testFileName))
        return testFileName;

    return QString();
}
/*
    Checks if a rule is a replacement rule.
*/
bool PortingRules::isReplacementRule(const QString ruleType) const
{
    return (ruleType == QLatin1String("RenamedHeader") || ruleType == QLatin1String("RenamedClass") ||
            ruleType == QLatin1String("RenamedToken") || ruleType == QLatin1String("RenamedEnumvalue") ||
            ruleType == QLatin1String("RenamedType") || ruleType == QLatin1String("RenamedQtSymbol") );
}

/*
    Disables a replacement rule given by the replacementRule parameter
*/
void PortingRules::disableRule(QtSimpleXml &replacementRule)
{
    RuleDescription ruleDescription(replacementRule);
    disabledRules.append(ruleDescription);
}

/*
    Checks if a replacement rule is disabled or not
*/
bool PortingRules::isRuleDisabled(QtSimpleXml &replacementRule) const
{
    RuleDescription ruleDescription(replacementRule);
    return disabledRules.contains(ruleDescription);
}

/*
    Adds a warning to the global logger.
*/
void PortingRules::addLogWarning(const QString text) const
{
    Logger::instance()->addEntry(new PlainLogEntry(QLatin1String("Warning"), QLatin1String("Porting"), text));
}

/*
    Adds an error to the global logger.
*/
void PortingRules::addLogError(const QString text) const
{
    Logger::instance()->addEntry(new PlainLogEntry(QLatin1String("Error"), QLatin1String("Porting"), text));
}

QT_END_NAMESPACE
