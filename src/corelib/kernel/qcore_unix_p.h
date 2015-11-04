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

#ifndef QCORE_UNIX_P_H
#define QCORE_UNIX_P_H

#include "qplatformdefs.h"
#include "qatomic.h"

#ifndef Q_OS_UNIX
# error "qcore_unix_p.h included on a non-Unix system"
#endif

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

struct sockaddr;

#if defined(Q_OS_LINUX) && defined(O_CLOEXEC)
# define QT_UNIX_SUPPORTS_THREADSAFE_CLOEXEC 1
QT_BEGIN_NAMESPACE
namespace QtLibcSupplement {
    inline int accept4(int, sockaddr *, QT_SOCKLEN_T *, int)
    { errno = ENOSYS; return -1; }
    inline int dup3(int, int, int)
    { errno = ENOSYS; return -1; }
    inline int pipe2(int [], int )
    { errno = ENOSYS; return -1; }
}
QT_END_NAMESPACE
using namespace QT_PREPEND_NAMESPACE(QtLibcSupplement);

#else
# define QT_UNIX_SUPPORTS_THREADSAFE_CLOEXEC 0
#endif

#define EINTR_LOOP(var, cmd)                    \
    do {                                        \
        var = cmd;                              \
    } while (var == -1 && errno == EINTR)

QT_BEGIN_NAMESPACE

// Internal operator functions for timevals
inline timeval &normalizedTimeval(timeval &t)
{
    while (t.tv_usec > 1000000l) {
        ++t.tv_sec;
        t.tv_usec -= 1000000l;
    }
    while (t.tv_usec < 0l) {
        --t.tv_sec;
        t.tv_usec += 1000000l;
    }
    return t;
}
inline bool operator<(const timeval &t1, const timeval &t2)
{ return t1.tv_sec < t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_usec < t2.tv_usec); }
inline bool operator==(const timeval &t1, const timeval &t2)
{ return t1.tv_sec == t2.tv_sec && t1.tv_usec == t2.tv_usec; }
inline timeval &operator+=(timeval &t1, const timeval &t2)
{
    t1.tv_sec += t2.tv_sec;
    t1.tv_usec += t2.tv_usec;
    return normalizedTimeval(t1);
}
inline timeval operator+(const timeval &t1, const timeval &t2)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec + t2.tv_sec;
    tmp.tv_usec = t1.tv_usec + t2.tv_usec;
    return normalizedTimeval(tmp);
}
inline timeval operator-(const timeval &t1, const timeval &t2)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec - (t2.tv_sec - 1);
    tmp.tv_usec = t1.tv_usec - (t2.tv_usec + 1000000);
    return normalizedTimeval(tmp);
}
inline timeval operator*(const timeval &t1, int mul)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec * mul;
    tmp.tv_usec = t1.tv_usec * mul;
    return normalizedTimeval(tmp);
}

inline void qt_ignore_sigpipe()
{
#ifndef Q_NO_POSIX_SIGNALS
    // Set to ignore SIGPIPE once only.
    static QBasicAtomicInt atom = QBasicAtomicInt{Q_BASIC_ATOMIC_INITIALIZER(0)};
    if (!atom.load()) {
        // More than one thread could turn off SIGPIPE at the same time
        // But that's acceptable because they all would be doing the same
        // action
        struct sigaction noaction;
        memset(&noaction, 0, sizeof(noaction));
        noaction.sa_handler = SIG_IGN;
        ::sigaction(SIGPIPE, &noaction, 0);
        atom.store(1);
    }
#else
    // Posix signals are not supported by the underlying platform
    // so we don't need to ignore sigpipe signal explicitly
#endif
}

// don't call QT_OPEN or ::open
// call qt_safe_open
static inline int qt_safe_open(const char *pathname, int flags, mode_t mode = 0777)
{
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
    int fd;
    EINTR_LOOP(fd, QT_OPEN(pathname, flags, mode));

    // unknown flags are ignored, so we have no way of verifying if
    // O_CLOEXEC was accepted
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
    return fd;
}
#undef QT_OPEN
#define QT_OPEN         qt_safe_open


// don't call ::pipe, call qt_safe_pipe
static inline int qt_safe_pipe(int pipefd[2], int flags = 0)
{
#ifdef O_CLOEXEC
    Q_ASSERT((flags & ~(O_CLOEXEC | O_NONBLOCK)) == 0);
#else
    Q_ASSERT((flags & ~O_NONBLOCK) == 0);
#endif

    int ret;
#if QT_UNIX_SUPPORTS_THREADSAFE_CLOEXEC && defined(O_CLOEXEC)
    // use pipe2
    flags |= O_CLOEXEC;
    ret = ::pipe2(pipefd, flags); // pipe2 is Linux-specific and is documented not to return EINTR
    if (ret == 0 || errno != ENOSYS)
        return ret;
#endif

    ret = ::pipe(pipefd);
    if (ret == -1)
        return -1;

    ::fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
    ::fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

    // set non-block too?
    if (flags & O_NONBLOCK) {
        ::fcntl(pipefd[0], F_SETFL, ::fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
        ::fcntl(pipefd[1], F_SETFL, ::fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
    }

    return 0;
}

// don't call dup or fcntl(F_DUPFD)
static inline int qt_safe_dup(int oldfd, int atleast = 0, int flags = FD_CLOEXEC)
{
    Q_ASSERT(flags == FD_CLOEXEC || flags == 0);

    int ret;
#ifdef F_DUPFD_CLOEXEC
    // use this fcntl
    if (flags & FD_CLOEXEC) {
        ret = ::fcntl(oldfd, F_DUPFD_CLOEXEC, atleast);
        if (ret != -1 || errno != EINVAL)
            return ret;
    }
#endif

    // use F_DUPFD
    ret = ::fcntl(oldfd, F_DUPFD, atleast);

    if (flags && ret != -1)
        ::fcntl(ret, F_SETFD, flags);
    return ret;
}

// don't call dup2
// call qt_safe_dup2
static inline int qt_safe_dup2(int oldfd, int newfd, int flags = FD_CLOEXEC)
{
    Q_ASSERT(flags == FD_CLOEXEC || flags == 0);

    int ret;
#if QT_UNIX_SUPPORTS_THREADSAFE_CLOEXEC && defined(O_CLOEXEC)
    // use dup3
    if (flags & FD_CLOEXEC) {
        EINTR_LOOP(ret, ::dup3(oldfd, newfd, O_CLOEXEC));
        if (ret == 0 || errno != ENOSYS)
            return ret;
    }
#endif
    EINTR_LOOP(ret, ::dup2(oldfd, newfd));
    if (ret == -1)
        return -1;

    if (flags)
        ::fcntl(newfd, F_SETFD, flags);
    return 0;
}

static inline qint64 qt_safe_read(int fd, void *data, qint64 maxlen)
{
    qint64 ret = 0;
    EINTR_LOOP(ret, QT_READ(fd, data, maxlen));
    return ret;
}
#undef QT_READ
#define QT_READ qt_safe_read

static inline qint64 qt_safe_write(int fd, const void *data, qint64 len)
{
    qint64 ret = 0;
    EINTR_LOOP(ret, QT_WRITE(fd, data, len));
    return ret;
}
#undef QT_WRITE
#define QT_WRITE qt_safe_write

static inline qint64 qt_safe_write_nosignal(int fd, const void *data, qint64 len)
{
    qt_ignore_sigpipe();
    return qt_safe_write(fd, data, len);
}

static inline int qt_safe_close(int fd)
{
    int ret;
    EINTR_LOOP(ret, QT_CLOSE(fd));
    return ret;
}
#undef QT_CLOSE
#define QT_CLOSE qt_safe_close

static inline int qt_safe_execve(const char *filename, char *const argv[],
                                 char *const envp[])
{
    int ret;
    EINTR_LOOP(ret, ::execve(filename, argv, envp));
    return ret;
}

static inline int qt_safe_execv(const char *path, char *const argv[])
{
    int ret;
    EINTR_LOOP(ret, ::execv(path, argv));
    return ret;
}

static inline int qt_safe_execvp(const char *file, char *const argv[])
{
    int ret;
    EINTR_LOOP(ret, ::execvp(file, argv));
    return ret;
}

static inline pid_t qt_safe_waitpid(pid_t pid, int *status, int options)
{
    int ret;
    EINTR_LOOP(ret, ::waitpid(pid, status, options));
    return ret;
}

#if !defined(_POSIX_MONOTONIC_CLOCK)
#  define _POSIX_MONOTONIC_CLOCK -1
#endif

timeval qt_gettime(); // in qelapsedtimer_mac.cpp or qtimestamp_unix.cpp

Q_CORE_EXPORT int qt_safe_select(int nfds, fd_set *fdread, fd_set *fdwrite, fd_set *fdexcept,
                                 const struct timeval *tv);

// according to X/OPEN we have to define semun ourselves
// we use prefix as on some systems sem.h will have it
struct semid_ds;
union qt_semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;      /* array for GETALL, SETALL */
};

QT_END_NAMESPACE

#endif
