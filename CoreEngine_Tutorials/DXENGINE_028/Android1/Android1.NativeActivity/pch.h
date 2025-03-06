//
// pch.h
// Header for standard system include files.
//
// Used by the build system to generate the precompiled header. Note that no
// pch.cpp is needed and the pch.h is automatically included in all cpp files
// that are part of the project
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#include "platform.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#if defined GLES2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#elif defined GLES3
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#endif

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <android/sensor.h>
#include <android/log.h>
#include "android_native_app_glue.h"

#include "AndroidEngine.h"

#define GL_BGRA                           0x80E1