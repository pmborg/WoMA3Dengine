// --------------------------------------------------------------------------------------------
// Filename: AndroidEngine.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2024
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2024 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// --------------------------------------------------------------------------------------------
#if !defined NewWomaEngine
#pragma once
#include "platform.h"
#include <android\input.h>
#include "android_native_app_glue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "[WOMA]", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "[WOMA]", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"[WOMA]", __VA_ARGS__))

#include <assert.h>
#define ASSERT(cond) assert(cond)

/**
* Our saved state data.
*/

struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* Shared state for our app.
*/
struct womaengine {
	struct android_app* app;

	const char* egl_vendor;
	const char* egl_render;
	const char* egl_version;
	const char* egl_extension;
	const char* egl_client_apis;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	bool has_focus_;
	bool GLES;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	bool shaderNeedsNewProjectionMatrix;
	struct saved_state state;
};

extern int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
extern void engine_handle_cmd(struct android_app* app, int32_t cmd);
extern bool engine_init_display(struct womaengine* engine);
extern int engine_create_context(struct womaengine* engine);
extern void updateRenderArea(struct womaengine* engine);
extern void engine_render(struct womaengine* engine);
extern void engine_term_display(struct womaengine* engine);

void init_engine(struct android_app* app, struct womaengine* engine);
void loadAll(struct android_app* app);

void process_events(struct womaengine* engine, struct android_app* app);

extern struct womaengine engine;
#endif