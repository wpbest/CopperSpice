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

#ifndef DS9_AUDIOOUTPUT_H
#define DS9_AUDIOOUTPUT_H

#include "backendnode.h"
#include <phonon/audiooutputinterface.h>
#include "backend.h"

struct IBaseFilter;
struct IBasicAudio;

QT_BEGIN_NAMESPACE

namespace Phonon
{
    namespace DS9
    {
        class AudioOutput : public BackendNode, public Phonon::AudioOutputInterface
        {
            CS_OBJECT(AudioOutput)
            CS_INTERFACES(Phonon::AudioOutputInterface)

        public:
            AudioOutput(Backend *back, QObject *parent);
            ~AudioOutput();

            // Attributes Getters:
            qreal volume() const;
            int outputDevice() const;
            void setVolume(qreal newVolume);
            bool setOutputDevice(int newDevice);
            bool setOutputDevice(const AudioOutputDevice & newDevice);
            void setCrossFadingProgress(short currentIndex, qreal progress);
       
            DS9_CS_SIGNAL_1(Public, void volumeChanged(qreal newVolume))
            DS9_CS_SIGNAL_2(volumeChanged,newVolume) 

            DS9_CS_SIGNAL_1(Public, void audioDeviceFailed())
            DS9_CS_SIGNAL_2(audioDeviceFailed) 

        private:
            short m_currentIndex;
            qreal m_crossfadeProgress;

            int m_device;
            Backend *m_backend;
            qreal m_volume;
        };
    }
}

QT_END_NAMESPACE

#endif // PHONON_AUDIOOUTPUT_H
