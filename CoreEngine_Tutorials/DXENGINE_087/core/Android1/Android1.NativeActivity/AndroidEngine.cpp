#if !defined NewWomaEngine
// --------------------------------------------------------------------------------------------
// Filename: AndroidEngine.cpp
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

#include "AndroidEngine.h"
#include "mem_leak.h"
#include "defs.h"
#include <android\input.h>

#if CORE_ENGINE_LEVEL >= 10 && !RUN_ASMAIN
extern void ImGui_Init(struct android_app* app);
extern void ImGui_Stop();
#endif



// Process the next main command.
void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
	struct womaengine* engine = (struct womaengine*)app->userData;
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
			LOGE("case APP_CMD_INIT_WINDOW");
			//TO_CLONE:
			//pApp->userData = new Renderer(pApp);

			// The window is being shown, get it ready.
			if (engine->app->window != NULL) 
			{
				engine_init_display(engine);
				updateRenderArea(engine);
			#if CORE_ENGINE_LEVEL >= 10 && defined USE_IMGUI && !RUN_ASMAIN
				ImGui_Init(app);
			#endif
				//engine->has_focus_ = true;
				//engine_render(engine);
			}
			break;
		//START:2
		case APP_CMD_GAINED_FOCUS:
			// When our app gains focus, we start monitoring the accelerometer.
			LOGE("case APP_CMD_GAINED_FOCUS");

			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_enableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
				// We'd like to get 60 events per second (in us).
				ASensorEventQueue_setEventRate(engine->sensorEventQueue, engine->accelerometerSensor, (1000L / 60) * 1000);
			}
			engine->has_focus_ = true;
			break;

		//STOP:1
		case APP_CMD_LOST_FOCUS:
			// When our app loses focus, we stop monitoring the accelerometer.
			// This is to avoid consuming battery while not being used.
			LOGE("case APP_CMD_LOST_FOCUS");

			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_disableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
			}
			// Also stop animating.
			engine->has_focus_ = false;
			updateRenderArea(engine);
			//engine_render(engine);
			break;
		//STOP:2
		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			LOGE("case APP_CMD_TERM_WINDOW");
		#if CORE_ENGINE_LEVEL >= 10 && defined USE_IMGUI && !RUN_ASMAIN
			ImGui_Stop();
		#endif
			engine_term_display(engine);
			engine->has_focus_ = false;
			break;
		//STOP:3
		case APP_CMD_SAVE_STATE:
			// The system has asked us to save our current state.  Do so.
			LOGE("case APP_CMD_SAVE_STATE");
			//engine->app->savedState = malloc(sizeof(struct saved_state));
			//*((struct saved_state*)engine->app->savedState) = engine->state;
			//engine->app->savedStateSize = sizeof(struct saved_state);
			break;
		//STOP:4
		case APP_CMD_STOP:
			LOGE("case APP_CMD_STOP");
			break;
	}
}


// Process the next input event.
#if defined USE_IMGUI
#include "imgui.h"
IMGUI_IMPL_API int32_t  ImGui_ImplAndroid_HandleInputEvent(const AInputEvent* input_event);
#endif

int32_t engine_handle_input(struct android_app* app, AInputEvent* inputEvent) {
	struct womaengine* engine = (struct womaengine*)app->userData;

	if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_MOTION)
	{
		engine->state.x = AMotionEvent_getX(inputEvent, 0);
		engine->state.y = AMotionEvent_getY(inputEvent, 0);
		//return 1;
	}

	/*
	int32_t event_action = AMotionEvent_getAction(input_event);
	int32_t event_pointer_index = (event_action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
	if (ident == AMOTION_EVENT_TOOL_TYPE_FINGER || ident == AMOTION_EVENT_TOOL_TYPE_UNKNOWN)
	{
		AMotionEvent_getX(input_event, event_pointer_index);
		AMotionEvent_getY(input_event, event_pointer_index);
	}
	*/
#if defined USE_IMGUI
	return ImGui_ImplAndroid_HandleInputEvent(inputEvent); //
#else
	return 0;
#endif
}


void init_engine(struct android_app* app, struct womaengine* engine)
{
	LOGE("init_engine()");

	// Prepare to monitor accelerometer
	engine->sensorManager = ASensorManager_getInstance();
	engine->accelerometerSensor = ASensorManager_getDefaultSensor(engine->sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	engine->sensorEventQueue = ASensorManager_createEventQueue(engine->sensorManager, app->looper, LOOPER_ID_USER, NULL, NULL);

	if (app->savedState != NULL) {
		// We are starting with a previous saved state; restore from it.
		engine->state = *(struct saved_state*)app->savedState;
	}
}
#endif