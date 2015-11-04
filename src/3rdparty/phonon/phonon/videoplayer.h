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

#ifndef Phonon_VIDEOPLAYER_H
#define Phonon_VIDEOPLAYER_H

#include "phonon_export.h"
#include "phononnamespace.h"
#include "mediasource.h"
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PHONON_VIDEOPLAYER

namespace Phonon
{
class VideoPlayerPrivate;
class MediaObject;
class AudioOutput;
class VideoWidget;

/** \class VideoPlayer videoplayer.h Phonon/VideoPlayer
 * \short Playback class for simple tasks.
 *
 * With %VideoPlayer you can get results quickly and easily. You can do the standard
 * playback tasks like play, pause and stop, but also set a playback volume and
 * seek (there's no guarantee that the seek will work, though).
 *
 * Keep in mind that when the %VideoPlayer instance is deleted the playback will
 * stop.
 *
 * A play and forget code example:
 * \code
 * VideoPlayer *player = new VideoPlayer(parentWidget);
 * connect(player, SIGNAL(finished()), player, SLOT(deleteLater()));
 * player->play(url);
 * \endcode
 *
 * \ingroup Playback
 * \ingroup PhononVideo
 * \author Matthias Kretz <kretz@kde.org>
 */
class PHONON_EXPORT VideoPlayer : public QWidget
{
    CS_OBJECT(VideoPlayer)

    public:      
        explicit VideoPlayer(Phonon::Category category, QWidget *parent = 0);
     
        VideoPlayer(QWidget *parent = 0);      
        ~VideoPlayer();
      
        qint64 totalTime() const;        
        qint64 currentTime() const;
        
        float volume() const;       
        bool isPlaying() const;       
        bool isPaused() const;
      
        MediaObject *mediaObject() const;        
        AudioOutput *audioOutput() const;
        VideoWidget *videoWidget() const;

    public :
        
        PHN_CS_SLOT_1(Public, void load(const Phonon::MediaSource & source))
        PHN_CS_SLOT_2(load) 
       
        PHN_CS_SLOT_1(Public, void play(const Phonon::MediaSource & source))
        PHN_CS_SLOT_OVERLOAD(play,(const Phonon::MediaSource &)) 
        
        PHN_CS_SLOT_1(Public, void play())
        PHN_CS_SLOT_OVERLOAD(play,()) 
        
        PHN_CS_SLOT_1(Public, void pause())
        PHN_CS_SLOT_2(pause) 
        
        PHN_CS_SLOT_1(Public, void stop())
        PHN_CS_SLOT_2(stop) 
       
        PHN_CS_SLOT_1(Public, void seek(qint64 ms))
        PHN_CS_SLOT_2(seek) 
       
        PHN_CS_SLOT_1(Public, void setVolume(float volume))
        PHN_CS_SLOT_2(setVolume) 
   
        PHN_CS_SIGNAL_1(Public, void finished())
        PHN_CS_SIGNAL_2(finished) 

    protected:
        VideoPlayerPrivate *const d;
};

} //namespace Phonon

#endif //QT_NO_PHONON_VIDEOPLAYER

QT_END_NAMESPACE

#endif