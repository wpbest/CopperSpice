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

/********************************************************
**  Copyright (C) 2005-2006 Matthias Kretz <kretz@kde.org
********************************************************/

#ifndef PHONON_VOLUMESLIDER_H
#define PHONON_VOLUMESLIDER_H

#include "phonon_export.h"
#include "phonondefs.h"
#include <QtGui/QWidget>
QT_BEGIN_NAMESPACE

#ifndef QT_NO_PHONON_VOLUMESLIDER

namespace Phonon
{
class AudioOutput;
class VolumeSliderPrivate;

/** \class VolumeSlider volumeslider.h Phonon/VolumeSlider
 * \short Widget providing a slider to control the volume of an AudioOutput.
 *
 * \ingroup PhononWidgets
 * \author Matthias Kretz <kretz@kde.org>
 */
class PHONON_EXPORT VolumeSlider : public QWidget
{
    CS_OBJECT(VolumeSlider)
    K_DECLARE_PRIVATE(VolumeSlider)
   
    PHN_CS_PROPERTY_READ(maximumVolume, maximumVolume)
    PHN_CS_PROPERTY_WRITE(maximumVolume, setMaximumVolume)   
   
    PHN_CS_PROPERTY_READ(orientation, orientation)
    PHN_CS_PROPERTY_WRITE(orientation, setOrientation)

    PHN_CS_PROPERTY_READ(tracking, hasTracking)
    PHN_CS_PROPERTY_WRITE(tracking, setTracking)
   
    PHN_CS_PROPERTY_READ(pageStep, pageStep)
    PHN_CS_PROPERTY_WRITE(pageStep, setPageStep)
   
    PHN_CS_PROPERTY_READ(singleStep, singleStep)
    PHN_CS_PROPERTY_WRITE(singleStep, setSingleStep)

    PHN_CS_PROPERTY_READ(muteVisible, isMuteVisible)
    PHN_CS_PROPERTY_WRITE(muteVisible, setMuteVisible)
    
    PHN_CS_PROPERTY_READ(iconSize, iconSize)
    PHN_CS_PROPERTY_WRITE(iconSize, setIconSize)

    public:
     
        explicit VolumeSlider(QWidget *parent = 0);
        explicit VolumeSlider(AudioOutput *, QWidget *parent = 0);
        ~VolumeSlider();

        bool hasTracking() const;
        void setTracking(bool tracking);
        int pageStep() const;
        void setPageStep(int milliseconds);
        int singleStep() const;
        void setSingleStep(int milliseconds);
        bool isMuteVisible() const;
        QSize iconSize() const;
        qreal maximumVolume() const;
        Qt::Orientation orientation() const;
        AudioOutput *audioOutput() const;

        PHN_CS_SLOT_1(Public, void setMaximumVolume(qreal un_named_arg1))
        PHN_CS_SLOT_2(setMaximumVolume) 
        
        PHN_CS_SLOT_1(Public, void setOrientation(Qt::Orientation un_named_arg1))
        PHN_CS_SLOT_2(setOrientation) 

        PHN_CS_SLOT_1(Public, void setMuteVisible(bool un_named_arg1))
        PHN_CS_SLOT_2(setMuteVisible) 

        PHN_CS_SLOT_1(Public, void setIconSize(const QSize & size))
        PHN_CS_SLOT_2(setIconSize) 
      
        PHN_CS_SLOT_1(Public, void setAudioOutput(Phonon::AudioOutput * un_named_arg1))
        PHN_CS_SLOT_2(setAudioOutput) 

    protected:
        VolumeSliderPrivate *const k_ptr;

    private:
        PHN_CS_SLOT_1(Private, void _k_sliderChanged(int un_named_arg1))
        PHN_CS_SLOT_2(_k_sliderChanged)

        PHN_CS_SLOT_1(Private, void _k_volumeChanged(qreal un_named_arg1))
        PHN_CS_SLOT_2(_k_volumeChanged)

        PHN_CS_SLOT_1(Private, void _k_mutedChanged(bool un_named_arg1))
        PHN_CS_SLOT_2(_k_mutedChanged)

        PHN_CS_SLOT_1(Private, void _k_buttonClicked())
        PHN_CS_SLOT_2(_k_buttonClicked)
};

} // namespace Phonon

#endif //QT_NO_PHONON_VOLUMESLIDER

QT_END_NAMESPACE

#endif
