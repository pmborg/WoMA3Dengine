// --------------------------------------------------------------------------------------------
// Filename: SmokeEmitterClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// NOTE: With a help from CHATGPT :)
// PURPOSE:
// --------------------------------------------------------------------------------------------

#pragma once
#include "platform.h"
#include "OSengine.h"
#include "VirtualModelClass.h"
#include "DX11Class.h"

class SmokeEmitterClass
{
public:
    SmokeEmitterClass();
    ~SmokeEmitterClass();

    void Initialize(void* ctx, void* driver, const char* textureFile, Vector3 worldPos, float radius);
    void Update(float dt);
    void Render(void* pContext, float fadeLight);
	void Shutdown();

private:
    DirectX::DX11Class* m_Driver_ = nullptr;
    //VirtualModelClass*  m_smokeModel = nullptr;

    struct Particle {
		Vector3 pos;
		Vector3 vel;
        float  life;     // 0..1
        float  size;
    };

    std::vector<Particle> m_particles;

	Vector3 m_origin;
    float  m_radius= 0.f;
    float  m_spawnTimer = 0.f;
};
