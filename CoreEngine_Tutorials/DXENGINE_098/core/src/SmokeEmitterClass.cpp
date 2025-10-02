// --------------------------------------------------------------------------------------------
// Filename: SmokeEmitterClass.cpp
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

#include "platform.h"
#include "SmokeEmitterClass.h"
#include "SystemClass.h"
#include "vertexTypes.h"
#include "mem_leak.h" // NEW
#include "DXmodelClass.h"

SmokeEmitterClass smokeEmitterClass;

SmokeEmitterClass::SmokeEmitterClass() 
{
	CLASSLOADER();
}

SmokeEmitterClass::~SmokeEmitterClass()
{
	Shutdown();
	CLASSDELETE();
}

void SmokeEmitterClass::Shutdown()
{
	if (SystemHandle && SystemHandle->m_Application)
		SAFE_DELETE(SystemHandle->m_Application->m_smokeModel);
}

void SmokeEmitterClass::Initialize(void* ctx, void* driver, const char* textureFile, Vector3 worldPos, float radius)
{
    m_origin = worldPos;
    m_radius = radius;

	ID3D11DeviceContext* pContext11 = (ID3D11DeviceContext*)ctx;
	m_Driver_ = (DirectX::DX11Class*)driver;

	//VirtualModelClass* m_smokeModel = SystemHandle->m_Application->m_smokeModel;
	std::vector<SmokeVertexType> SmokeVertexModel;
	SmokeVertexType vertex;

	SmokeVertexModel.emplace_back(Vector3(-0.5f, +0.5f, 0.0f), Vector2(0.0f, 0.0f)); // top-left
	SmokeVertexModel.emplace_back(Vector3(+0.5f, +0.5f, 0.0f), Vector2(1.0f, 0.0f)); // top-right
	SmokeVertexModel.emplace_back(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f)); // bottom-left
	SmokeVertexModel.emplace_back(Vector3(+0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)); // bottom-right

	std::vector<STRING> smoke_textures;
	smoke_textures.push_back(textureFile);

	std::vector<UINT> indexList = { 0,2,1,  1,2,3 };

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(SystemHandle->m_Application->m_smokeModel, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(SystemHandle->m_Application->m_smokeModel, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
	ASSERT(SystemHandle->m_Application->m_smokeModel->LoadSmoke(pContext11, TEXT("Smoke"),
		m_Driver_, SHADER_USE_SMOKE,
		&smoke_textures, &SmokeVertexModel, &indexList));

	SystemHandle->m_Application->m_smokeModel->translation(worldPos.x, worldPos.y, worldPos.z);
	SystemHandle->m_Application->m_smokeModel->scale(3,3,3);
}

void SmokeEmitterClass::Update(float dt)
{
    // Spawn new particles
    m_spawnTimer += dt;
    while (m_spawnTimer > 0.05f) {      // every 0.05s
        m_spawnTimer -= 0.05f;
        Particle p;

		// good waterfall-like spawn
		p.pos = m_origin
			+ Vector3((rand() % 100 / 100.f - 0.5f) * m_radius,     // sideways
				(rand() % 100 / 200.f),                      // start at slightly different Y
				(rand() % 100 / 100.f - 0.5f) * m_radius);

		p.vel = Vector3((rand() % 100 / 500.f - 0.1f),                // small sideways drift
			0.8f + (rand() % 100 / 150.f),                // strong upward
			(rand() % 100 / 500.f - 0.1f));              // small sideways drift
		p.life = 1.0f;
		p.size = 0.8f + (rand() % 100 / 200.f);                        // varied size


        m_particles.push_back(p);
    }

    // Update existing
    for (auto& p : m_particles) {
        p.pos = p.pos + p.vel * dt;
        p.life -= dt * 0.25f;    // fade over 4s
        p.size += dt * 0.2f;     // expand slowly
    }

    // Remove dead
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& p){ return p.life <= 0.f; }),
        m_particles.end()
    );

	//womalog("Smoke particles: %zu  dt=%f  spawnTimer=%f\n", m_particles.size(), dt, m_spawnTimer);
}

void SmokeEmitterClass::Render(void* pContext, float fadeLight)
{
	static VirtualModelClass* m_smokeModel = nullptr;
	if (!m_smokeModel)
		m_smokeModel = SystemHandle->m_Application->m_smokeModel;

	Vector3 camPos(sort_cameraX, sort_cameraY, sort_cameraZ);

	// --- sort back-to-front for correct alpha blending ---
	std::sort(m_particles.begin(), m_particles.end(),
		[&](const Particle& a, const Particle& b)
		{
			float da = (a.pos.x - camPos.x) * (a.pos.x - camPos.x)
				+ (a.pos.y - camPos.y) * (a.pos.y - camPos.y)
				+ (a.pos.z - camPos.z) * (a.pos.z - camPos.z);

			float db = (b.pos.x - camPos.x) * (b.pos.x - camPos.x)
				+ (b.pos.y - camPos.y) * (b.pos.y - camPos.y)
				+ (b.pos.z - camPos.z) * (b.pos.z - camPos.z);

			// sort farthest → nearest for alpha blending
			return da > db;
		});

	// --- render ---
	((DXmodelClass*)m_smokeModel)->model_fade = fadeLight;
	for (auto& p : m_particles)
	{
		((DXmodelClass*)m_smokeModel)->m_worldMatrix = XMMatrixIdentity();
		m_smokeModel->scale(p.size, p.size, p.size);

		Vector3 toCam = camPos - p.pos;
		float yaw = atan2(toCam.x, toCam.z);
		m_smokeModel->rotateY(yaw);

		m_smokeModel->translation(p.pos.x, p.pos.y, p.pos.z);
		m_smokeModel->SetAlpha(p.life);
		m_smokeModel->Render(pContext);
	}
}


