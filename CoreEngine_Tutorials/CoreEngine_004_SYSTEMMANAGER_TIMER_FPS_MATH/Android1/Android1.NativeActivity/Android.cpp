// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: Android.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2024
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include <malloc.h>
#include "Android.h"

/**
* Inicializar um contexto EGL para a exibição atual.
*/
static int engine_init_display(struct engine* engine) {
	// inicializar OpenGL ES e EGL

	/*
	* Aqui, especifica os atributos da configuração desejada.
	* Abaixo, selecionamos um EGLConfig com pelo menos 8 bits por componente de cor
	* compatível com janelas na tela
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Aqui, o aplicativo escolhe a configuração que desejar. Neste
	 * modelo, temos um processo de seleção muito simplificado, onde podemos escolher
	* o primeiro EGLConfig que atenda aos nossos critérios */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID é um atributo de EGLConfig que está
	* garantido para ser aceito por ANativeWindow_setBuffersGeometry().
	* Assim que escolhermos um EGLConfig, será possível reconfigurar os buffers de
	* ANativeWindow para corresponder, usando EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.clean_color = 0;

	// Inicializar estado GL.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	return 0;
}

/**
* Somente o quadro atual na tela.
*/
void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// Sem tela.
		return;
	}

	// Basta preencher a tela com uma cor.
	glClearColor(engine->state.clean_color, engine->state.clean_color, engine->state.clean_color, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(engine->display, engine->surface);
}

/**
* Desative o contexto de EGL atualmente associado à tela.
*/
void engine_term_display(struct engine* engine) {
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

	engine->animating = 0;
	engine->has_focus_ = false;

	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
* Processe o próximo evento de entrada.
*/
int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);
		return 1;
	}
	return 0;
}

/**
* Processe o próximo comando principal.
*/
void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	//RUN ORDER:
	//case APP_CMD_INIT_WINDOW
	//case APP_CMD_GAINED_FOCUS
	//...
	//case APP_CMD_LOST_FOCUS
	//case APP_CMD_TERM_WINDOW
	//case APP_CMD_SAVE_STATE
	//case APP_CMD_STOP

	switch (cmd) 
	{
		//START:1
		case APP_CMD_INIT_WINDOW:
			//LOGE("case APP_CMD_INIT_WINDOW");
			// A janela está sendo exibida, prepare-a.
			if (engine->app->window != NULL) {
				engine_init_display(engine);
				//engine_draw_frame(engine);
			}
		break;

		//START:2
		case APP_CMD_GAINED_FOCUS:
			// Quando nosso aplicativo obtiver foco, começaremos a monitorar o acelerômetro.
			//LOGE("case APP_CMD_GAINED_FOCUS");
			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_enableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
				// Desejamos obter 60 eventos por segundo (em microssegundos).
				ASensorEventQueue_setEventRate(engine->sensorEventQueue, engine->accelerometerSensor, (1000L / 60) * 1000);
			}
			engine->has_focus_ = true;
			break;

		//STOP:1
		case APP_CMD_LOST_FOCUS:
			//LOGE("case APP_CMD_LOST_FOCUS");
			// Quando nosso aplicativo perder foco, paramos de monitorar o acelerômetro.
			// Isso é para evitar o consumo da bateria enquanto ela não está sendo usada.
			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_disableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
			}
			// Também interromper animação.
			engine->animating = 0;
			engine->has_focus_ = false;
			engine_draw_frame(engine);
			break;

		//STOP:2
		case APP_CMD_TERM_WINDOW:
			//LOGE("case APP_CMD_TERM_WINDOW");
			// A janela está sendo ocultada ou fechada, limpe-a.
			engine_term_display(engine);
			engine->has_focus_ = false;
			break;

		//STOP:3
		case APP_CMD_SAVE_STATE:
			//LOGE("case APP_CMD_SAVE_STATE");
			// O sistema pediu para salvar nosso estado atual.  Faça-o.
			engine->app->savedState = malloc(sizeof(struct saved_state));
			*((struct saved_state*)engine->app->savedState) = engine->state;
			engine->app->savedStateSize = sizeof(struct saved_state);
			break;

		//STOP:4
		case APP_CMD_STOP:
			//LOGE("case APP_CMD_STOP");
			break;

		case APP_CMD_DESTROY:
			//LOGE("APP_CMD_DESTROY\n");
			engine->app->destroyRequested = 1;
			break;
	}
}
