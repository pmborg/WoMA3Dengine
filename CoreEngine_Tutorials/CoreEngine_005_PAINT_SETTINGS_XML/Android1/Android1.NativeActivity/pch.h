//
// pch.h
// Cabeçalho para os arquivos de sistema padrão.
//
// Usado pelo sistema de compilação para gerar o cabeçalho pré-compilado. Observe que nenhum
// pch.cpp necessário e pch.h incluído automaticamente em todos os arquivos cpp
// que fazem parte do objeto
//

#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>

#include <android/log.h>
#include "android_native_app_glue.h"

#include "platform.h"
#include "default_settings_xml.h"
#include "woma_macros.h"