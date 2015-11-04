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

#ifndef QT7_AUDIOMIXER_H
#define QT7_AUDIOMIXER_H

#include <QtCore/QObject>
#include <QtCore/QTime>
#include <QtCore/QEvent>
#include <phonon/effectinterface.h>
#include <phonon/effectparameter.h>
#include <phonon/volumefaderinterface.h>
#include "medianode.h"
#include "audionode.h"

QT_BEGIN_NAMESPACE

namespace Phonon
{
namespace QT7
{
    class AudioMixerAudioNode : public AudioNode
    {
        public:
            AudioMixerAudioNode();
            void setVolume(float volume, int bus = 0);
            float volume(int bus = 0);

        protected:
            ComponentDescription getAudioNodeDescription() const;
            void initializeAudioUnit();

        private:
            friend class AudioMixer;
            int m_numberOfBusses;
            float m_volume;
    };

    class AudioMixer : public MediaNode, Phonon::EffectInterface, Phonon::VolumeFaderInterface
    {
        CS_OBJECT(AudioMixer)

        CS_INTERFACES(Phonon::EffectInterface, Phonon::VolumeFaderInterface)
  
        public:
            AudioMixer(QObject *parent = 0);
            ~AudioMixer();
            AudioMixerAudioNode *m_audioNode;
            Phonon::VolumeFaderEffect::FadeCurve m_fadeCurve;

            int m_fadeTimer;
            int m_fadeDuration;
            float m_fadeToVolume;
            float m_fadeFromVolume;
            QTime m_fadeStartTime;

            // EffectInterface:
            QList<Phonon::EffectParameter> parameters() const;
            QVariant parameterValue(const Phonon::EffectParameter &parameter) const;
            void setParameterValue(const Phonon::EffectParameter &parameter, const QVariant &newValue);

            // VolumeFaderInterface:
            float volume() const;
            void setVolume(float volume);
            Phonon::VolumeFaderEffect::FadeCurve fadeCurve() const;
            void setFadeCurve(Phonon::VolumeFaderEffect::FadeCurve fadeCurve);
            void fadeTo(float volume, int fadeTime);
            void updateFade();

        protected:
            bool event(QEvent *event);
    };

}} //namespace Phonon::QT7

QT_END_NAMESPACE

#endif // Phonon_QT7_AUDIOMIXER_H
