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

#include "deviceorientation.h"

#include <e32base.h>
#include <sensrvchannelfinder.h>
#include <sensrvdatalistener.h>
#include <sensrvchannel.h>
#include <sensrvorientationsensor.h>

class SymbianOrientation : public DeviceOrientation, public MSensrvDataListener
{
    Q_OBJECT
public:
    SymbianOrientation()
        : DeviceOrientation(), m_current(UnknownOrientation), m_sensorChannel(0), m_channelOpen(false)
    {
        TRAP_IGNORE(initL());
        if (!m_sensorChannel)
            qWarning("No valid sensors found.");
    }

    ~SymbianOrientation()
    {
        if (m_sensorChannel) {
            m_sensorChannel->StopDataListening();
            m_sensorChannel->CloseChannel();
            delete m_sensorChannel;
        }
    }

    void initL()
    {
        CSensrvChannelFinder *channelFinder = CSensrvChannelFinder::NewLC();
        RSensrvChannelInfoList channelInfoList;
        CleanupClosePushL(channelInfoList);

        TSensrvChannelInfo searchConditions;
        searchConditions.iChannelType = KSensrvChannelTypeIdOrientationData;
        channelFinder->FindChannelsL(channelInfoList, searchConditions);

        for (int i = 0; i < channelInfoList.Count(); ++i) {
            TRAPD(error, m_sensorChannel = CSensrvChannel::NewL(channelInfoList[i]));
            if (!error)
                TRAP(error, m_sensorChannel->OpenChannelL());
            if (!error) {
                TRAP(error, m_sensorChannel->StartDataListeningL(this, 1, 1, 0));
                m_channelOpen = true;
                break;
           }
            if (error) {
                delete m_sensorChannel;
                m_sensorChannel = 0;
            }
        }

        channelInfoList.Close();
        CleanupStack::Pop(&channelInfoList);
        CleanupStack::PopAndDestroy(channelFinder);
    }

    Orientation orientation() const
    {
        return m_current;
    }

   void setOrientation(Orientation) { }

private:
    DeviceOrientation::Orientation m_current;
    CSensrvChannel *m_sensorChannel;
    bool m_channelOpen;
    void pauseListening() {
        if (m_sensorChannel && m_channelOpen) {
            m_sensorChannel->StopDataListening();
            m_sensorChannel->CloseChannel();
            m_channelOpen = false;
        }
    }

    void resumeListening() {
        if (m_sensorChannel && !m_channelOpen) {
            TRAPD(error, m_sensorChannel->OpenChannelL());
            if (!error) {
                TRAP(error, m_sensorChannel->StartDataListeningL(this, 1, 1, 0));
                if (!error) {
                    m_channelOpen = true;
                }
            }
            if (error) {
                delete m_sensorChannel;
                m_sensorChannel = 0;
            }
        }
    }

    void DataReceived(CSensrvChannel &channel, TInt count, TInt dataLost)
    {
        Q_UNUSED(dataLost)
        if (channel.GetChannelInfo().iChannelType == KSensrvChannelTypeIdOrientationData) {
            TSensrvOrientationData data;
            for (int i = 0; i < count; ++i) {
                TPckgBuf<TSensrvOrientationData> dataBuf;
                channel.GetData(dataBuf);
                data = dataBuf();
                Orientation orientation = UnknownOrientation;
                switch (data.iDeviceOrientation) {
                case TSensrvOrientationData::EOrientationDisplayUp:
                    orientation = Portrait;
                    break;
                case TSensrvOrientationData::EOrientationDisplayRightUp:
                    orientation = Landscape;
                    break;
                case TSensrvOrientationData::EOrientationDisplayLeftUp:
                    orientation = LandscapeInverted;
                    break;
                case TSensrvOrientationData::EOrientationDisplayDown:
                    orientation = PortraitInverted;
                    break;
                case TSensrvOrientationData::EOrientationUndefined:
                case TSensrvOrientationData::EOrientationDisplayUpwards:
                case TSensrvOrientationData::EOrientationDisplayDownwards:
                default:
                    break;
                }

                if (m_current != orientation && orientation != UnknownOrientation) {
                    m_current = orientation;
                    emit orientationChanged();
                }
           }
        }
    }

   void DataError(CSensrvChannel& /* channel */, TSensrvErrorSeverity /* error */)
   {
   }

   void GetDataListenerInterfaceL(TUid /* interfaceUid */, TAny*& /* interface */)
   {
   }
};


DeviceOrientation* DeviceOrientation::instance()
{
    static SymbianOrientation *o = 0;
    if (!o)
        o = new SymbianOrientation;
    return o;
}

#include "deviceorientation_symbian.moc"
