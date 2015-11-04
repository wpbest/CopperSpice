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

#ifndef DS9_BACKEND_H
#define DS9_BACKEND_H

#include "phononds9_namespace.h"
#include <phonon/backendinterface.h>
#include <phonon/phononnamespace.h>

#include <QtCore/QList>
#include <QtCore/QMutex>

#include "compointer.h"
#include "backendnode.h"

QT_BEGIN_NAMESPACE

namespace Phonon
{
    namespace DS9
    {
        class AudioOutput;
        class MediaObject;

        typedef Phonon::ObjectDescriptionType ObjectDescriptionType;

        class Backend : public QObject, public Phonon::BackendInterface
        {
            CS_OBJECT(Backend)
            CS_INTERFACES(Phonon::BackendInterface)

        public:
            Backend(QObject *parent = 0, const QVariantList & = QVariantList());
            virtual ~Backend();

            QObject *createObject(Phonon::BackendInterface::Class, QObject *parent, const QList<QVariant> &args);

            bool supportsVideo() const;
            QStringList availableMimeTypes() const;

            QList<int> objectDescriptionIndexes(Phonon::ObjectDescriptionType type) const;
            QHash<QByteArray, QVariant> objectDescriptionProperties(Phonon::ObjectDescriptionType type, int index) const;

            bool connectNodes(QObject *, QObject *);
            bool disconnectNodes(QObject *, QObject *);

            //transaction management
            bool startConnectionChange(QSet<QObject *>);
            bool endConnectionChange(QSet<QObject *>);

            Filter getAudioOutputFilter(int index) const;

            static QMutex *directShowMutex;

            DS9_CS_SIGNAL_1(Public, void objectDescriptionChanged(ObjectDescriptionType un_named_arg1))
            DS9_CS_SIGNAL_2(objectDescriptionChanged,un_named_arg1) 

        private:
            class AudioMoniker : public ComPointer<IMoniker>
            {
            public:
                bool operator==(const AudioMoniker &other);
            };
            mutable QVector<AudioMoniker> m_audioOutputs;
            mutable QVector<CLSID> m_audioEffects;
            mutable QMutex m_directShowMutex;
        };
    }
}

QT_END_NAMESPACE

#endif // PHONON_BACKEND_H
