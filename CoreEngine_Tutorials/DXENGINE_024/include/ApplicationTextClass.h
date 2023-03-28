// --------------------------------------------------------------------------------------------
// Filename: ApplicationTextClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2014
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// PURPOSE:
//
// ********************************************************************************************
#pragma once

//028
#include "main.h"
#include "VirtualTextClass.h"			//[ch8]

// Index position in the ARRAY:
#define TEXT_FPS			0
#define TEXT_CPU			1
#define TEXT_POS			2
#define TEXT_ROT			3
#define TEXT_TIME			4
#define TEXT_FADE			5
#define TEXT_ScreenToShow	6
#define TEXT_CLOCK			7
#define TEXT_LIGHT_DIR		8
#define TEXT_TERRAINRENDERCOUNT 9	// 60
#define TEXT_LATENCY			10	// 140

#define N_TEXT_MAX_SENTENCE 11 // LAST+1

namespace DirectX {

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationTextClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationTextClass
{
public:
	ApplicationTextClass();
	~ApplicationTextClass();

	bool Initialize(void* Driver);
	void Shutdown();
	void Render();

	void SetFps(int);
	void SetCpu(int);

	void SetCameraPosition(float, float, float);
	void SetCameraRotation(float, float, float);
	void SetClockTime(UINT, UINT);
	void SetLightDirection(float rotX, float rotY, float rotZ);

	std::vector<SentenceType*>	m_sentence;	//SentenceType* m_sentence[N_TEXT_MAX_SENTENCE];
	VirtualTextClass*			m_Text;
};

}

//extern DirectX::ApplicationTextClass* AppTextClass;