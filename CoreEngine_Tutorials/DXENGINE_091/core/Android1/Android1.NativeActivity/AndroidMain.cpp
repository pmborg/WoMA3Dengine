/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licenciado sob Apache License, Versão 2.0 (a "Licença");
* você não pode usar esse arquivo, exceto se estiver em conformidade com a Licença.
* Você pode obter uma cópia da Licença em
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* A menos que solicitado pela lei aplicável ou acordado por escrito, o software
* distribuído sob a Licença é distribuído em uma BASE ''NO ESTADO'',
* SEM GARANTIAS OU CONDIÇÕES DE NENHUM TIPO, expressas ou implícitas.
* Consulte o idioma específico governando as permissões e
* limitações sob a Licença.
*
*/

#include "Android.h"
#include "OSengine.h"


extern int Command;

static android_app* app = NULL;
struct engine engine = { 0 };

void process_events()
{
	int ident;
	int events;
	struct android_poll_source* source;

	// Se não estiver animando, bloquearemos indefinidamente, aguardando eventos.
	// Se estiver animado, faremos um loop até que todos os eventos sejam lidos e continuaremos
	// para desenhar o próximo quadro da animação.
	while ((ident = ALooper_pollAll(engine.has_focus_ ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {

		// Processar este evento.
		if (source != NULL) {
			source->process(app, source);
		}

		// Se um sensor tiver dados, processe-o agora.
		if (ident == LOOPER_ID_USER) {
			if (engine.accelerometerSensor != NULL) {
				ASensorEvent event;
				while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0) {
					//LOGI("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
				}
			}
		}
	}
}

bool android_APPLICATION_INIT_SYSTEM()
{
#if defined ANDROID_PLATFORM
	// Preparar para monitorar acelerômetro
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager, app->looper, LOOPER_ID_USER, NULL, NULL);

	if (app->savedState != NULL) {
		// Estamos começando com um estado anterior salvo; faça a restauração por meio dele.
		engine.state = *(struct saved_state*)app->savedState;
	}
#endif

	return true;
}

int android_APPLICATION_MAIN_LOOP() 
{
	//MANDATORY: Wait for window creation: (pre-req: ShowAlert used on WomaMessageBox)
	while (!engine.has_focus_)
		process_events();

	WomaMessageBox(WOMA::strConsoleTitle, DEMO_TITLE, MB_OK);
	Sleep(3000);	//wait 3 seconds

#if LEVEL >= 10
	engine.animating = 1;
#endif

	// loop aguardando itens.
	while (app->destroyRequested == 0)
	{
		// Leia todos os eventos pendentes.
		process_events();

		if (engine.has_focus_)
		{
			// Concluído com eventos; desenhe o próximo quadro de animação.
			engine.state.clean_color += .01f;
			if (engine.state.clean_color > 1)
				engine.state.clean_color = 0;

			// O desenho é acelerado para a taxa de atualização de tela, para que
			// não seja necessário fazer o intervalo aqui.
			engine_draw_frame(&engine);
		}

	}

	return Command;
}
/*
void android_APPLICATION_STOP() 
{
	// Verifique se estamos saindo.
	engine_term_display(&engine);
}
*/
/**
* Este é o ponto de entrada principal de um aplicativo nativo usando
 * android_native_app_glue.  Ele é executado em seu thread, com seu próprio loop de eventos
* para receber eventos de entrada e outros itens.
*/
void android_main(struct android_app* state) {
	app = state;

	_tprintf("[%d]: android_main()\n", gettid());
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	int argc = 0;
	char* argv[0];

	APPLICATION_MAIN(argc, argv);

	return;
}

