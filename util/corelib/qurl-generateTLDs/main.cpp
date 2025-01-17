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

#include <QtCore>

static QString utf8encode(const QByteArray &array) // turns e.g. tranøy.no to tran\xc3\xb8y.no
{
    QString result;
    result.reserve(array.length() + array.length() / 3);
    for (int i = 0; i < array.length(); ++i) {
        char c = array.at(i);
        // if char is non-ascii, escape it
        if (c < 0x20 || uchar(c) >= 0x7f) {
            result += "\\x" + QString::number(uchar(c), 16);
        } else {
            // if previous char was escaped, we need to make sure the next char is not
            // interpreted as part of the hex value, e.g. "äc.com" -> "\xabc.com"; this
            // should be "\xab""c.com"
            QRegExp hexEscape("\\\\x[a-fA-F0-9][a-fA-F0-9]$");
            bool isHexChar = ((c >= '0' && c <= '9') ||
                             (c >= 'a' && c <= 'f') ||
                             (c >= 'A' && c <= 'F'));
            if (result.contains(hexEscape) && isHexChar)
                result += "\"\"";
            result += c;
        }
    }
    return result;
}

int main(int argc, char **argv) {

    QCoreApplication app(argc, argv);
    if (argc < 3) {
        printf("\nusage: %s inputFile outputFile\n\n", argv[0]);
        printf("'inputFile' should be a list of effective TLDs, one per line,\n");
        printf("as obtained from http://publicsuffix.org . To create indices and data file\n");
        printf("file, do the following:\n\n");
        printf("       wget http://mxr.mozilla.org/mozilla-central/source/netwerk/dns/effective_tld_names.dat?raw=1 -O effective_tld_names.dat\n");
        printf("       grep '^[^\\/\\/]' effective_tld_names.dat > effective_tld_names.dat.trimmed\n");
        printf("       %s effective_tld_names.dat.trimmed effective_tld_names.dat.qt\n\n", argv[0]);
        printf("Now copy the data from effective_tld_names.dat.qt to the file src/corelib/io/qurltlds_p.h in your Qt repo\n\n");
        exit(1);
    }
    QFile file(argv[1]);
    QFile outFile(argv[2]);
    file.open(QIODevice::ReadOnly);
    outFile.open(QIODevice::WriteOnly);

    QByteArray outIndicesBufferBA;
    QBuffer outIndicesBuffer(&outIndicesBufferBA);
    outIndicesBuffer.open(QIODevice::WriteOnly);

    QByteArray outDataBufferBA;
    QBuffer outDataBuffer(&outDataBufferBA);
    outDataBuffer.open(QIODevice::WriteOnly);

    int lineCount = 0;
    while (!file.atEnd()) {
        file.readLine();
        lineCount++;
    }
    file.reset();
    QVector<QString> strings(lineCount);
    while (!file.atEnd()) {
        QString s = QString::fromUtf8(file.readLine());
        QString st = s.trimmed();
        int num = qHash(st) % lineCount;

        QString utf8String = utf8encode(st.toUtf8());

        // for domain 1.com, we could get something like
        // a.com\01.com, which would be interpreted as octal 01,
        // so we need to separate those strings with quotes
        QRegExp regexpOctalEscape(QLatin1String("^[0-9]"));
        if (!strings.at(num).isEmpty() && st.contains(regexpOctalEscape))
            strings[num].append("\"\"");

        strings[num].append(utf8String);
        strings[num].append("\\0");
    }

    outIndicesBuffer.write("static const quint16 tldCount = ");
    outIndicesBuffer.write(QByteArray::number(lineCount));
    outIndicesBuffer.write(";\n");
    outIndicesBuffer.write("static const quint16 tldIndices[");
//    outIndicesBuffer.write(QByteArray::number(lineCount+1)); // not needed
    outIndicesBuffer.write("] = {\n");

    int utf8Size = 0;
//    int charSize = 0;
    for (int a = 0; a < lineCount; a++) {
        bool lineIsEmpty = strings.at(a).isEmpty();
        if (!lineIsEmpty) {
            strings[a].prepend("\"");
            strings[a].append("\"");
        }
        int zeroCount = strings.at(a).count(QLatin1String("\\0"));
        int utf8CharsCount = strings.at(a).count(QLatin1String("\\x"));
        int quoteCount = strings.at(a).count('"');
        outDataBuffer.write(strings.at(a).toUtf8());
        if (!lineIsEmpty)
            outDataBuffer.write("\n");
        outIndicesBuffer.write(QByteArray::number(utf8Size));
        outIndicesBuffer.write(",\n");
        utf8Size += strings.at(a).count() - (zeroCount + quoteCount + utf8CharsCount * 3);
//        charSize += strings.at(a).count();
    }
    outIndicesBuffer.write(QByteArray::number(utf8Size));
    outIndicesBuffer.write("};\n");
    outIndicesBuffer.close();
    outFile.write(outIndicesBufferBA);

    outDataBuffer.close();
    outFile.write("\nstatic const char tldData[");
//    outFile.write(QByteArray::number(charSize)); // not needed
    outFile.write("] = {\n");
    outFile.write(outDataBufferBA);
    outFile.write("};\n");
    outFile.close();
    printf("data generated to %s . Now copy the data from this file to src/corelib/io/qurltlds_p.h in your Qt repo\n", argv[2]);
    exit(0);
}
