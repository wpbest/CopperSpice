/*
 * Copyright (C) 2006 George Staikos <staikos@kde.org>
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2008 Holger Hans Peter Freyther
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <QBasicTimer>
#include <QCoreApplication>
#include <QDebug>
#include <QPointer>
#include <wtf/CurrentTime.h>

namespace WebCore {

class SharedTimerQt : public QObject {
    CS_OBJECT(SharedTimerQt)

    friend void setSharedTimerFiredFunction(void (*f)());

public:
    static SharedTimerQt* inst();

    void start(double);
    void stop();

protected:
    void timerEvent(QTimerEvent* ev);

private:
    WEB_CS_SLOT_1(Private,void destroy())
    WEB_CS_SLOT_2(destroy)
    
    SharedTimerQt();
    ~SharedTimerQt();
    QBasicTimer m_timer;
    void (*m_timerFunction)();
};

SharedTimerQt::SharedTimerQt()
    : QObject()
    , m_timerFunction(0)
{}

SharedTimerQt::~SharedTimerQt()
{
    if (m_timer.isActive()) {
        if (m_timerFunction) {
            (m_timerFunction)();
            m_timerFunction = 0;
        }
    }
}

void SharedTimerQt::destroy()
{
    delete this;
}

SharedTimerQt* SharedTimerQt::inst()
{
    static QPointer<SharedTimerQt> timer;
    if (!timer) {
        timer = new SharedTimerQt();
        QObject::connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), timer, SLOT(destroy()));
    }

    return timer;
}

void SharedTimerQt::start(double fireTime)
{
    double interval = fireTime - currentTime();
    unsigned int intervalInMS;
    if (interval < 0)
        intervalInMS = 0;
    else {
        interval *= 1000;
        intervalInMS = (unsigned int)interval;
    }

    m_timer.start(intervalInMS, this);
}

void SharedTimerQt::stop()
{
    m_timer.stop();
}

void SharedTimerQt::timerEvent(QTimerEvent* ev)
{
    if (!m_timerFunction || ev->timerId() != m_timer.timerId())
        return;

    m_timer.stop();
    (m_timerFunction)();
}

void setSharedTimerFiredFunction(void (*f)())
{
    if (!QCoreApplication::instance())
        return;

    SharedTimerQt::inst()->m_timerFunction = f;
}

void setSharedTimerFireTime(double fireTime)
{
    if (!QCoreApplication::instance())
        return;

    SharedTimerQt::inst()->start(fireTime);
}

void stopSharedTimer()
{
    if (!QCoreApplication::instance())
        return;

    SharedTimerQt::inst()->stop();
}


}

