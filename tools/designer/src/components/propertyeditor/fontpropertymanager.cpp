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

#include "fontpropertymanager.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtpropertybrowserutils_p.h"

#include <qdesigner_utils_p.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtXml/QXmlStreamReader>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

    static const char *aliasingC[] = {
        QT_TRANSLATE_NOOP("FontPropertyManager", "PreferDefault"),
        QT_TRANSLATE_NOOP("FontPropertyManager", "NoAntialias"),
        QT_TRANSLATE_NOOP("FontPropertyManager", "PreferAntialias")
    };

    FontPropertyManager::FontPropertyManager() :
        m_createdFontProperty(0)
    {
        const int nameCount = sizeof(aliasingC)/sizeof(const char *);
        for (int  i = 0; i < nameCount; i++)
            m_aliasingEnumNames.push_back(QCoreApplication::translate("FontPropertyManager", aliasingC[i]));

        QString errorMessage;
        if (!readFamilyMapping(&m_familyMappings, &errorMessage)) {
            designerWarning(errorMessage);
        }

    }

    void FontPropertyManager::preInitializeProperty(QtProperty *property,
                                                    int type,
                                                    ResetMap &resetMap)
    {
        if (m_createdFontProperty) {
            PropertyToSubPropertiesMap::iterator it = m_propertyToFontSubProperties.find(m_createdFontProperty);
            if (it == m_propertyToFontSubProperties.end())
                it = m_propertyToFontSubProperties.insert(m_createdFontProperty, PropertyList());
            const int index = it.value().size();
            m_fontSubPropertyToFlag.insert(property, index);
            it.value().push_back(property);
            m_fontSubPropertyToProperty[property] = m_createdFontProperty;
            resetMap[property] = true;
        }

        if (type == QVariant::Font)
            m_createdFontProperty = property;
    }

    // Map the font family names to display names retrieved from the XML configuration
    static QStringList designerFamilyNames(QStringList families, const FontPropertyManager::NameMap &nm)
    {
        if (nm.empty())
            return families;

        const FontPropertyManager::NameMap::const_iterator ncend = nm.constEnd();
        const QStringList::iterator end = families.end();
        for (QStringList::iterator it = families.begin(); it != end; ++it) {
            const FontPropertyManager::NameMap::const_iterator nit = nm.constFind(*it);
            if (nit != ncend)
                *it = nit.value();
        }
        return families;
    }

    void FontPropertyManager::postInitializeProperty(QtVariantPropertyManager *vm,
                                                     QtProperty *property,
                                                     int type,
                                                     int enumTypeId)
    {
        if (type != QVariant::Font)
            return;

        // This will cause a recursion
        QtVariantProperty *antialiasing = vm->addProperty(enumTypeId, QCoreApplication::translate("FontPropertyManager", "Antialiasing"));
        const QFont font = qvariant_cast<QFont>(vm->variantProperty(property)->value());

        antialiasing->setAttribute(QLatin1String("enumNames"), m_aliasingEnumNames);
        antialiasing->setValue(antialiasingToIndex(font.styleStrategy()));
        property->addSubProperty(antialiasing);

        m_propertyToAntialiasing[property] = antialiasing;
        m_antialiasingToProperty[antialiasing] = property;
        // Fiddle family names
        if (!m_familyMappings.empty()) {
            const PropertyToSubPropertiesMap::iterator it = m_propertyToFontSubProperties.find(m_createdFontProperty);
            QtVariantProperty *familyProperty = vm->variantProperty(it.value().front());
            const QString enumNamesAttribute = QLatin1String("enumNames");
            QStringList plainFamilyNames = familyProperty->attributeValue(enumNamesAttribute).toStringList();
            // Did someone load fonts or something?
            if (m_designerFamilyNames.size() != plainFamilyNames.size())
                m_designerFamilyNames = designerFamilyNames(plainFamilyNames, m_familyMappings);
            familyProperty->setAttribute(enumNamesAttribute, m_designerFamilyNames);
        }
        // Next
        m_createdFontProperty = 0;
    }

    bool FontPropertyManager::uninitializeProperty(QtProperty *property)
    {
        const PropertyToPropertyMap::iterator ait =  m_propertyToAntialiasing.find(property);
        if (ait != m_propertyToAntialiasing.end()) {
            QtProperty *antialiasing = ait.value();
            m_antialiasingToProperty.remove(antialiasing);
            m_propertyToAntialiasing.erase(ait);
            delete antialiasing;
        }

        PropertyToSubPropertiesMap::iterator sit = m_propertyToFontSubProperties.find(property);
        if (sit == m_propertyToFontSubProperties.end())
            return false;

        m_propertyToFontSubProperties.erase(sit);
        m_fontSubPropertyToFlag.remove(property);
        m_fontSubPropertyToProperty.remove(property);

        return true;
    }

    void FontPropertyManager::slotPropertyDestroyed(QtProperty *property)
    {
        removeAntialiasingProperty(property);
    }

    void FontPropertyManager::removeAntialiasingProperty(QtProperty *property)
    {
        const PropertyToPropertyMap::iterator ait =  m_antialiasingToProperty.find(property);
        if (ait == m_antialiasingToProperty.end())
            return;
        m_propertyToAntialiasing[ait.value()] = 0;
        m_antialiasingToProperty.erase(ait);
    }

    bool FontPropertyManager::resetFontSubProperty(QtVariantPropertyManager *vm, QtProperty *property)
    {
        const PropertyToPropertyMap::iterator it = m_fontSubPropertyToProperty.find(property);
        if (it == m_fontSubPropertyToProperty.end())
            return false;

        QtVariantProperty *fontProperty = vm->variantProperty(it.value());

        QVariant v = fontProperty->value();
        QFont font = qvariant_cast<QFont>(v);
        unsigned mask = font.resolve();
        const unsigned flag = fontFlag(m_fontSubPropertyToFlag.value(property));

        mask &= ~flag;
        font.resolve(mask);
        v.setValue(font);
        fontProperty->setValue(v);
        return true;
    }

    int FontPropertyManager::antialiasingToIndex(QFont::StyleStrategy antialias)
    {
        switch (antialias) {
        case QFont::PreferDefault:   return 0;
        case QFont::NoAntialias:     return 1;
        case QFont::PreferAntialias: return 2;
        default: break;
        }
        return 0;
    }

    QFont::StyleStrategy FontPropertyManager::indexToAntialiasing(int idx)
    {
        switch (idx) {
        case 0: return QFont::PreferDefault;
        case 1: return QFont::NoAntialias;
        case 2: return QFont::PreferAntialias;
        }
        return QFont::PreferDefault;
    }

    unsigned FontPropertyManager::fontFlag(int idx)
    {
        switch (idx) {
        case 0: return QFont::FamilyResolved;
        case 1: return QFont::SizeResolved;
        case 2: return QFont::WeightResolved;
        case 3: return QFont::StyleResolved;
        case 4: return QFont::UnderlineResolved;
        case 5: return QFont::StrikeOutResolved;
        case 6: return QFont::KerningResolved;
        case 7: return QFont::StyleStrategyResolved;
        }
        return 0;
    }

    FontPropertyManager::ValueChangedResult FontPropertyManager::valueChanged(QtVariantPropertyManager *vm, QtProperty *property, const QVariant &value)
    {
        QtProperty *antialiasingProperty = m_antialiasingToProperty.value(property, 0);
        if (!antialiasingProperty) {
            if (m_propertyToFontSubProperties.contains(property)) {
                updateModifiedState(property, value);
            }
            return NoMatch;
        }

        QtVariantProperty *fontProperty = vm->variantProperty(antialiasingProperty);
        const QFont::StyleStrategy newValue = indexToAntialiasing(value.toInt());

        QFont font = qvariant_cast<QFont>(fontProperty->value());
        const QFont::StyleStrategy oldValue = font.styleStrategy();
        if (newValue == oldValue)
            return Unchanged;

        font.setStyleStrategy(newValue);
        fontProperty->setValue(QVariant::fromValue(font));
        return Changed;
    }

    void FontPropertyManager::updateModifiedState(QtProperty *property, const QVariant &value)
    {
        const PropertyToSubPropertiesMap::iterator it = m_propertyToFontSubProperties.find(property);
        if (it == m_propertyToFontSubProperties.end())
            return;

        const PropertyList &subProperties = it.value();

        QFont font = qvariant_cast<QFont>(value);
        const unsigned mask = font.resolve();

        const int count = subProperties.size();
        for (int index = 0; index < count; index++) {
             const unsigned flag = fontFlag(index);
             subProperties.at(index)->setModified(mask & flag);
        }
    }

    void FontPropertyManager::setValue(QtVariantPropertyManager *vm, QtProperty *property, const QVariant &value)
    {
        updateModifiedState(property, value);

        if (QtProperty *antialiasingProperty = m_propertyToAntialiasing.value(property, 0)) {
            QtVariantProperty *antialiasing = vm->variantProperty(antialiasingProperty);
            if (antialiasing) {
                QFont font = qvariant_cast<QFont>(value);
                antialiasing->setValue(antialiasingToIndex(font.styleStrategy()));
            }
        }
    }

    /* Parse a mappings file of the form:
     * <fontmappings>
     * <mapping><family>DejaVu Sans</family><display>DejaVu Sans [CE]</display></mapping>
     * ... which is used to display on which platforms fonts are available.*/

    static const char *rootTagC = "fontmappings";
    static const char *mappingTagC = "mapping";
    static const char *familyTagC = "family";
    static const char *displayTagC = "display";

    static QString msgXmlError(const QXmlStreamReader &r, const QString& fileName)
    {
        return QString::fromUtf8("An error has been encountered at line %1 of %2: %3:").arg(r.lineNumber()).arg(fileName, r.errorString());
    }

    /* Switch stages when encountering a start element (state table) */
    enum ParseStage { ParseBeginning, ParseWithinRoot, ParseWithinMapping, ParseWithinFamily,
                      ParseWithinDisplay, ParseError };

    static ParseStage nextStage(ParseStage currentStage, const QStringRef &startElement)
    {
        switch (currentStage) {
        case ParseBeginning:
            return startElement == QLatin1String(rootTagC) ? ParseWithinRoot : ParseError;
        case ParseWithinRoot:
        case ParseWithinDisplay: // Next mapping, was in <display>
            return startElement == QLatin1String(mappingTagC) ? ParseWithinMapping : ParseError;
        case ParseWithinMapping:
            return startElement == QLatin1String(familyTagC) ? ParseWithinFamily : ParseError;
        case ParseWithinFamily:
            return startElement == QLatin1String(displayTagC) ? ParseWithinDisplay : ParseError;
        case ParseError:
            break;
        }
        return  ParseError;
    }

    bool FontPropertyManager::readFamilyMapping(NameMap *rc, QString *errorMessage)
    {
        rc->clear();
        const QString fileName = QLatin1String(":/trolltech/propertyeditor/fontmapping.xml");
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            *errorMessage = QString::fromUtf8("Unable to open %1: %2").arg(fileName, file.errorString());
            return false;
        }

        QXmlStreamReader reader(&file);
        QXmlStreamReader::TokenType token;

        QString family;
        ParseStage stage = ParseBeginning;
        do {
            token = reader.readNext();
            switch (token) {
            case QXmlStreamReader::Invalid:
                *errorMessage = msgXmlError(reader, fileName);
                 return false;
            case QXmlStreamReader::StartElement:
                stage = nextStage(stage, reader.name());
                switch (stage) {
                case ParseError:
                    reader.raiseError(QString::fromUtf8("Unexpected element <%1>.").arg(reader.name().toString()));
                    *errorMessage = msgXmlError(reader, fileName);
                    return false;
                case ParseWithinFamily:
                    family = reader.readElementText();
                    break;
                case ParseWithinDisplay:
                    rc->insert(family, reader.readElementText());
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        } while (token != QXmlStreamReader::EndDocument);
        return true;
    }

}

QT_END_NAMESPACE
