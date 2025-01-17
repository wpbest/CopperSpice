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

#ifndef MEXTENSIONS_H
#define MEXTENSIONS_H

#include <private/qgl_p.h>
#include <private/qeglcontext_p.h>
#include <private/qpixmapdata_gl_p.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/* Extensions decls */

#ifndef EGL_SHARED_IMAGE_NOK
#define EGL_SHARED_IMAGE_NOK 0x30DA
typedef void* EGLNativeSharedImageTypeNOK;
#endif

#ifndef EGL_GL_TEXTURE_2D_KHR
#define EGL_GL_TEXTURE_2D_KHR 0x30B1
#endif

#ifndef EGL_FIXED_WIDTH_NOK
#define EGL_FIXED_WIDTH_NOK 0x30DB
#define EGL_FIXED_HEIGHT_NOK 0x30DC
#endif

#ifndef EGL_BITMAP_POINTER_KHR
#define EGL_BITMAP_POINTER_KHR 0x30C6
#define EGL_BITMAP_PITCH_KHR 0x30C7
#endif

#ifndef EGL_MAP_PRESERVE_PIXELS_KHR
#define EGL_MAP_PRESERVE_PIXELS_KHR 0x30C4
#define EGL_LOCK_USAGE_HINT_KHR 0x30C5
#define EGL_READ_SURFACE_BIT_KHR 0x0001
#define EGL_WRITE_SURFACE_BIT_KHR 0x0002
#endif

#ifndef EGL_SYNC_FENCE_KHR
#define EGL_SYNC_FENCE_KHR 0x30F9
#define EGL_SYNC_TYPE_KHR 0x30F7
#define EGL_SYNC_STATUS_KHR 0x30F1
#define EGL_SYNC_CONDITION_KHR 0x30F8
#define EGL_SIGNALED_KHR 0x30F2
#define EGL_UNSIGNALED_KHR 0x30F3
#define EGL_SYNC_PRIOR_COMMANDS_COMPLETE_KHR 0x30F0
#define EGL_SYNC_FLUSH_COMMANDS_BIT_KHR 0x0001
#define EGL_FOREVER_KHR 0xFFFFFFFFFFFFFFFFull
#define EGL_TIMEOUT_EXPIRED_KHR 0x30F5
#define EGL_CONDITION_SATISFIED_KHR 0x30F6
#define EGL_NO_SYNC_KHR ((EGLSyncKHR)0)
typedef void* EGLSyncKHR;
typedef khronos_utime_nanoseconds_t EGLTimeKHR;
#endif

/* Class */

class QMeeGoExtensions
{
public:
    static void ensureInitialized();

    static EGLNativeSharedImageTypeNOK eglCreateSharedImageNOK(EGLDisplay dpy, EGLImageKHR image, EGLint *props);
    static bool eglQueryImageNOK(EGLDisplay dpy, EGLImageKHR image, EGLint prop, EGLint *v);
    static bool eglDestroySharedImageNOK(EGLDisplay dpy, EGLNativeSharedImageTypeNOK img);
    static bool eglSetSurfaceScalingNOK(EGLDisplay dpy, EGLSurface surface, int x, int y, int width, int height);
    static bool eglLockSurfaceKHR(EGLDisplay display, EGLSurface surface, const EGLint *attrib_list);
    static bool eglUnlockSurfaceKHR(EGLDisplay display, EGLSurface surface);
    static EGLSyncKHR eglCreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list);
    static bool eglDestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync);
    static EGLint eglClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout);
    static EGLBoolean eglGetSyncAttribKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint *value);

private:
    static void initialize();

    static bool initialized;
    static bool hasImageShared;
    static bool hasSurfaceScaling;
    static bool hasLockSurface;
    static bool hasFenceSync;
};

#endif
