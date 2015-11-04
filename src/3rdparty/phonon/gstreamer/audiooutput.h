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

#ifndef GSTREAMER_AUDIOOUTPUT_H
#define GSTREAMER_AUDIOOUTPUT_H

#include "common.h"
#include "medianode.h"
#include <phonon/audiooutputinterface.h>
#include <phonon/phononnamespace.h>
#include <QtCore/QFile>
#include <gst/gst.h>

QT_BEGIN_NAMESPACE

namespace Phonon
{
namespace Gstreamer
{
class AudioOutput : public QObject, public AudioOutputInterface, public MediaNode
{
    CS_OBJECT_MULTIPLE(AudioOutput, QObject)

    CS_INTERFACES(Phonon::AudioOutputInterface, Phonon::Gstreamer::MediaNode)  

public:
    AudioOutput(Backend *backend, QObject *parent);
    ~AudioOutput();

    qreal volume() const;
    int outputDevice() const;
    void setVolume(qreal newVolume);
    bool setOutputDevice(int newDevice);

#if (PHONON_VERSION >= PHONON_VERSION_CHECK(4, 2, 0))
    bool setOutputDevice(const AudioOutputDevice &newDevice);
#endif


    GstElement *audioElement()
    {
        Q_ASSERT(m_audioBin);
        return m_audioBin;
    }

    void mediaNodeEvent(const MediaNodeEvent *event);

    GSTRM_CS_SIGNAL_1(Public, void volumeChanged(qreal newVolume))
    GSTRM_CS_SIGNAL_2(volumeChanged,newVolume) 
    
    GSTRM_CS_SIGNAL_1(Public, void audioDeviceFailed())
    GSTRM_CS_SIGNAL_2(audioDeviceFailed) 

private:

    qreal m_volumeLevel;
    int m_device;

    GstElement *m_volumeElement;
    GstElement *m_audioBin;
    GstElement *m_audioSink;
    GstElement *m_conv;
};
}
} //namespace Phonon::Gstreamer

QT_END_NAMESPACE

#endif // Phonon_GSTREAMER_AUDIOOUTPUT_H
