#if !defined NewWomaEngine
#include "AndroidEngine.h"
//#include "pch.h"
#include "OSengine.h"
//#include <EGL/eglext.h>

/**
* Initialize an EGL context for the current display.
*/

//[2º] ~EQ~ Render.cpp:: void Renderer::initRenderer()
bool engine_init_display(struct womaengine* engine)
{
	// initialize OpenGL ES and EGL
	LOGE("[%d]: engine_init_display()", gettid());

	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/
	EGLint w, h;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	//STEP1
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (EGL_FALSE == eglInitialize(display, NULL, NULL)) {
		LOGE("NativeEngine: failed to init display, error %d", eglGetError()); 
		return false;
	}

	//[WOMA] egl_vendor: Android
	engine->egl_vendor = eglQueryString(display,EGL_VENDOR);
	LOGE("egl_vendor: %s", engine->egl_vendor);

	//[WOMA] egl_version: 1.5 Android META - EGL
	engine->egl_render = eglQueryString(display, GL_RENDERER);
	LOGE("egl_render: %s", engine->egl_render);
	//LOGE("egl_render: %s", glGetString(GL_RENDERER));

	//[WOMA] egl_version: 1.5 Android META - EGL
	engine->egl_version = eglQueryString(display, EGL_VERSION);
	LOGE("egl_version: %s", engine->egl_version);

	//[WOMA] egl_extension: EGL_ANDROID_front_buffer_auto_refresh EGL_ANDROID_get_native_client_buffer EGL_ANDROID_presentation_time EGL_EXT_surface_CTA861_3_metadata EGL_EXT_surface_SMPTE2086_metadata EGL_KHR_get_all_proc_addresses EGL_KHR_swap_buffers_with_damage EGL_ANDROID_get_frame_timestamps EGL_EXT_gl_colorspace_scrgb EGL_EXT_gl_colorspace_scrgb_linear EGL_EXT_gl_colorspace_display_p3_linear EGL_EXT_gl_colorspace_display_p3 EGL_EXT_gl_colorspace_display_p3_passthrough EGL_EXT_gl_colorspace_bt2020_hlg EGL_EXT_gl_colorspace_bt2020_linear EGL_EXT_gl_colorspace_bt2020_pq EGL_ANDROID_image_native_buffer EGL_ANDROID_native_fence_sync EGL_ANDROID_recordable EGL_EXT_create_context_robustness EGL_EXT_image_gl_colorspace EGL_EXT_pixel_format_float EGL_EXT_protected_content EGL_EXT_yuv_surface EGL_IMG_context_priority EGL_KHR_create_context EGL_KHR_create_context_no_error EGL_KHR_fence_sync EGL_KHR_gl_colorspace EGL_KHR_gl_renderbuffer_image EGL_KHR_gl_texture_2D_image EGL_KHR_gl_texture_3D_image EGL_KHR_gl_texture_cu
	engine->egl_extension = eglQueryString(display, EGL_EXTENSIONS);
	LOGE("egl_extension: %s", engine->egl_extension);

	//[WOMA] egl_client_apis: OpenGL_ES
	engine->egl_client_apis = eglQueryString(display, EGL_CLIENT_APIS);
	LOGE("egl_client_apis: %s", engine->egl_client_apis);

	//eglInitialize(display, 0, 0);
	ASSERT(display != EGL_NO_DISPLAY);

	//=============================================================================
	const EGLint attribs[] = {
#if defined GLES2
	  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // request OpenGL ES 2.0	-- C:\Program Files (x86)\Android\AndroidNDK64\android-ndk-r15c\platforms\android-26\arch-arm64\usr\include\EGL
#endif
#if defined GLES3
	  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR, //, // EGL_OPENGL_ES3_BIT EGL 1.4 - ES 3.x				-- C:\Program Files (x86)\Android\AndroidNDK\android-ndk-r23c\toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\include\EGL
#endif
	  EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	  EGL_BLUE_SIZE, 8,
	  EGL_GREEN_SIZE, 8,
	  EGL_RED_SIZE, 8,
	  EGL_DEPTH_SIZE, 24,
	  EGL_NONE
	};

#if defined GLES3
	/*
	//define _BASETSD_H_

	// get the list of configurations
	std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
	eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

	// Find a config we like.
	// Could likely just grab the first if we don't care about anything else in the config.
	// Otherwise hook in your own heuristic
	auto newconfig = *std::find_if(
		supportedConfigs.get(),
		supportedConfigs.get() + numConfigs,
		[&display](const EGLConfig &config) {
		EGLint red, green, blue, depth;
		if (eglGetConfigAttrib(display, config, EGL_RED_SIZE, &red)
			&& eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &green)
			&& eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &blue)
			&& eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &depth)) {

			//aout << "Found config with " << red << ", " << green << ", " << blue << ", " << depth << std::endl;
			return red == 8 && green == 8 && blue == 8 && depth == 24;
		}
		return false;
	});
	*/
	//aout << "Found " << numConfigs << " configs" << std::endl;
	//aout << "Chose " << config << std::endl;

#endif
	//=============================================================================

	// figure out how many configs there are
	eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
	LOGE("numConfigs: %d", numConfigs);
	//EGLConfig* c= supportedConfigs(new EGLConfig[numConfigs]);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	//STEP2
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);
	LOGE("numConfigs: %d", numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	//STEP3
	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	//STEP4 neee?
	//ANativeWindow_setBuffersGeometry(ANativeWindow* window, int32_t width, int32_t height, int32_t format);
	//ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	//STEP5
	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	if (surface == EGL_NO_SURFACE) {
		LOGE("Failed to create EGL surface, EGL error %d", eglGetError());
		return false;
	}

	//=============================================================================
	//STEP6:
#if defined GLES2
	// OpenGL ES 2.0
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	context = eglCreateContext(display, config, NULL, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		LOGE("Failed to create EGL context, EGL error %d", eglGetError());
		return false;
	}
#endif
#if defined GLES3
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
	context = eglCreateContext(display, config, NULL, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		LOGE("Failed to create EGL context, EGL error %d", eglGetError());
		return false;
	}
#endif
	//=============================================================================
	//STEP7:
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
	{
		LOGW("Unable to eglMakeCurrent");
		return false;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;

	// Initialize GL state.
#if _NOT //defined GLES3
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
#endif

	return 0;
}

/**
* Just the current frame in the display.
*/
void updateRenderArea(struct womaengine* engine)
{
	EGLint width, height;

	//if (engine->display == NULL) return; // No display.

	eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &width);
	eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &height);

	// Handle dimension changes.
	if (width != engine->width || height != engine->height) 
	{
		LOGE("width: %d height: %d", width, height);
		engine->width = width;
		engine->height = height;
		glViewport(0, 0, engine->width, engine->height);

		// make sure that we lazily recreate the projection matrix before we render
		engine->shaderNeedsNewProjectionMatrix = true;
	}

	if (SystemHandle) {
		SystemHandle->AppSettings->WINDOW_WIDTH = engine->width;
		SystemHandle->AppSettings->WINDOW_HEIGHT = engine->height;
	}
}

// ~EQ~: Render.cpp::void Renderer::render() 
void engine_render(struct womaengine* engine)
{
	updateRenderArea(engine);
}


/**
* Tear down the EGL context currently associated with the display.
*/
void engine_term_display(struct womaengine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->has_focus_ = false;
	engine->GLES = false;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}


void process_events(struct womaengine *engine, struct android_app* app)
{
	// Read all pending events.
	static int ident;
	static int events;
	struct android_poll_source* source = NULL;

	// If not animating, we will block forever waiting for events.
	// If animating, we loop until all events are read, then continue
	// to draw the next frame of animation.
	while ((ident = ALooper_pollAll(engine->has_focus_ ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
	{
		// Process this event.
		if (source != NULL) {
			source->process(app, source);
		}

		// If a sensor has data, process it now.
		if (ident == LOOPER_ID_USER) {
			if (engine->accelerometerSensor != NULL) {
				ASensorEvent event;
				while (ASensorEventQueue_getEvents(engine->sensorEventQueue, &event, 1) > 0) {
				//	LOGE("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
				}
			}
		}

		// Check if we are exiting.
		if (app->destroyRequested != 0) 
		{
			LOGE("app->destroyRequested");
			engine_term_display(engine);
			return;
		}
	}
}

#endif