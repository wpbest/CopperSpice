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

#ifndef QAUDIOOUTPUT_H
#define QAUDIOOUTPUT_H

#include <QtCore/qiodevice.h>
#include <QtCore/qglobal.h>
#include <QtMultimedia/qaudio.h>
#include <QtMultimedia/qaudioformat.h>
#include <QtMultimedia/qaudiodeviceinfo.h>

QT_BEGIN_NAMESPACE

class QAbstractAudioOutput;

class Q_MULTIMEDIA_EXPORT QAudioOutput : public QObject
{
    CS_OBJECT(QAudioOutput)

public:
    explicit QAudioOutput(const QAudioFormat &format = QAudioFormat(), QObject *parent = 0);
    explicit QAudioOutput(const QAudioDeviceInfo &audioDeviceInfo, const QAudioFormat &format = QAudioFormat(), QObject *parent = 0);
    ~QAudioOutput();

    QAudioFormat format() const;

    void start(QIODevice *device);
    QIODevice* start();

    void stop();
    void reset();
    void suspend();
    void resume();

    void setBufferSize(int bytes);
    int bufferSize() const;

    int bytesFree() const;
    int periodSize() const;

    void setNotifyInterval(int milliSeconds);
    int notifyInterval() const;

    qint64 processedUSecs() const;
    qint64 elapsedUSecs() const;

    QAudio::Error error() const;
    QAudio::State state() const;

    MULTI_CS_SIGNAL_1(Public, void stateChanged(QAudio::State un_named_arg1))
    MULTI_CS_SIGNAL_2(stateChanged,un_named_arg1) 
    MULTI_CS_SIGNAL_1(Public, void notify())
    MULTI_CS_SIGNAL_2(notify) 

private:
    Q_DISABLE_COPY(QAudioOutput)

    QAbstractAudioOutput* d;
};

QT_END_NAMESPACE

#endif // QAUDIOOUTPUT_H
