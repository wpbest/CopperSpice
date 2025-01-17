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

#include "qjsonwriter_p.h"
#include "qjson_p.h"

QT_BEGIN_NAMESPACE

using namespace QJsonPrivate;

static void objectContentToJson(const QJsonPrivate::Object *o, QByteArray &json, int indent, bool compact);
static void arrayContentToJson(const QJsonPrivate::Array *a, QByteArray &json, int indent, bool compact);

static inline uchar hexdig(uint u)
{
    return (u < 0xa ? '0' + u : 'a' + u - 0xa);
}

static QByteArray escapedString(const QString &s)
{
    const uchar replacement = '?';
    QByteArray ba(s.length(), Qt::Uninitialized);

    uchar *cursor = (uchar *)ba.data();
    const uchar *ba_end = cursor + ba.length();

    const QChar *ch = (const QChar *)s.constData();
    const QChar *end = ch + s.length();

    int surrogate_high = -1;

    while (ch < end) {
        if (cursor >= ba_end - 6) {
            // ensure we have enough space
            int pos = cursor - (const uchar *)ba.constData();
            ba.resize(ba.size()*2);
            cursor = (uchar *)ba.data() + pos;
            ba_end = (const uchar *)ba.constData() + ba.length();
        }

        uint u = ch->unicode();
        if (surrogate_high >= 0) {
            if (ch->isLowSurrogate()) {
                u = QChar::surrogateToUcs4(surrogate_high, u);
                surrogate_high = -1;
            } else {
                // high surrogate without low
                *cursor = replacement;
                ++ch;
                surrogate_high = -1;
                continue;
            }
        } else if (ch->isLowSurrogate()) {
            // low surrogate without high
            *cursor = replacement;
            ++ch;
            continue;
        } else if (ch->isHighSurrogate()) {
            surrogate_high = u;
            ++ch;
            continue;
        }

        if (u < 0x80) {
            if (u < 0x20 || u == 0x22 || u == 0x5c) {
                *cursor++ = '\\';
                switch (u) {
                case 0x22:
                    *cursor++ = '"';
                    break;
                case 0x5c:
                    *cursor++ = '\\';
                    break;
                case 0x8:
                    *cursor++ = 'b';
                    break;
                case 0xc:
                    *cursor++ = 'f';
                    break;
                case 0xa:
                    *cursor++ = 'n';
                    break;
                case 0xd:
                    *cursor++ = 'r';
                    break;
                case 0x9:
                    *cursor++ = 't';
                    break;
                default:
                    *cursor++ = 'u';
                    *cursor++ = '0';
                    *cursor++ = '0';
                    *cursor++ = hexdig(u>>4);
                    *cursor++ = hexdig(u & 0xf);
               }
            } else {
                *cursor++ = (uchar)u;
            }
        } else {
            if (u < 0x0800) {
                *cursor++ = 0xc0 | ((uchar) (u >> 6));
            } else {
                /* Qt 5 Beta 1
                // is it one of the Unicode non-characters?
                if (QChar::isNonCharacter(u)) {
                    *cursor++ = replacement;
                    ++ch;
                    continue;
                }
                */

                if (QChar::requiresSurrogates(u)) {
                    *cursor++ = 0xf0 | ((uchar) (u >> 18));
                    *cursor++ = 0x80 | (((uchar) (u >> 12)) & 0x3f);
                } else {
                    *cursor++ = 0xe0 | (((uchar) (u >> 12)) & 0x3f);
                }
                *cursor++ = 0x80 | (((uchar) (u >> 6)) & 0x3f);
            }
            *cursor++ = 0x80 | ((uchar) (u&0x3f));
        }
        ++ch;
    }

    ba.resize(cursor - (const uchar *)ba.constData());
    return ba;
}

static void valueToJson(const QJsonPrivate::Base *b, const QJsonPrivate::Value &v, QByteArray &json, int indent, bool compact)
{
    QJsonValue::Type type = (QJsonValue::Type)(uint)v.type;
    switch (type) {
    case QJsonValue::Bool:
        json += v.toBoolean() ? "true" : "false";
        break;
    case QJsonValue::Double:
        json += QByteArray::number(v.toDouble(b), 'g', 17);
        break;
    case QJsonValue::String:
        json += '"';
        json += escapedString(v.toString(b));
        json += '"';
        break;
    case QJsonValue::Array:
        json += compact ? "[" : "[\n";
        arrayContentToJson(static_cast<QJsonPrivate::Array *>(v.base(b)), json, indent + (compact ? 0 : 1), compact);
        json += QByteArray(4*indent, ' ');
        json += "]";
        break;
    case QJsonValue::Object:
        json += compact ? "{" : "{\n";
        objectContentToJson(static_cast<QJsonPrivate::Object *>(v.base(b)), json, indent + (compact ? 0 : 1), compact);
        json += QByteArray(4*indent, ' ');
        json += "}";
        break;
    case QJsonValue::Null:
    default:
        json += "null";
    }
}

static void arrayContentToJson(const QJsonPrivate::Array *a, QByteArray &json, int indent, bool compact)
{
    if (!a || !a->length)
        return;

    QByteArray indentString(4*indent, ' ');

    uint i = 0;
    while (1) {
        json += indentString;
        valueToJson(a, a->at(i), json, indent, compact);

        if (++i == a->length) {
            if (!compact)
                json += '\n';
            break;
        }

        json += compact ? "," : ",\n";
    }
}


static void objectContentToJson(const QJsonPrivate::Object *o, QByteArray &json, int indent, bool compact)
{
    if (!o || !o->length)
        return;

    QByteArray indentString(4*indent, ' ');

    uint i = 0;
    while (1) {
        QJsonPrivate::Entry *e = o->entryAt(i);
        json += indentString;
        json += '"';
        json += escapedString(e->key());
        json += "\": ";
        valueToJson(o, e->value, json, indent, compact);

        if (++i == o->length) {
            if (!compact)
                json += '\n';
            break;
        }

        json += compact ? "," : ",\n";
    }
}

void Writer::objectToJson(const QJsonPrivate::Object *o, QByteArray &json, int indent, bool compact)
{
    json.reserve(json.size() + (o ? (int)o->size : 16));
    json += compact ? "{" : "{\n";
    objectContentToJson(o, json, indent + (compact ? 0 : 1), compact);
    json += QByteArray(4*indent, ' ');
    json += compact ? "}" : "}\n";
}

void Writer::arrayToJson(const QJsonPrivate::Array *a, QByteArray &json, int indent, bool compact)
{
    json.reserve(json.size() + (a ? (int)a->size : 16));
    json += compact ? "[" : "[\n";
    arrayContentToJson(a, json, indent + (compact ? 0 : 1), compact);
    json += QByteArray(4*indent, ' ');
    json += compact ? "]" : "]\n";
}

QT_END_NAMESPACE
