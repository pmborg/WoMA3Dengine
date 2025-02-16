#pragma once

#include "android_native_app_glue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "[WOMA]", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "[WOMA]", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"[WOMA]", __VA_ARGS__))

extern void engine_handle_cmd(struct android_app* app, int32_t cmd);
extern int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
extern void engine_draw_frame(struct engine* engine);
extern void engine_term_display(struct engine* engine);

/**
* Nossos dados de estado salvos.
*/
struct saved_state {
	float clean_color;
	int32_t x;
	int32_t y;
};

/**
* Estado compartilhado do nosso aplicativo.
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	bool has_focus_;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};