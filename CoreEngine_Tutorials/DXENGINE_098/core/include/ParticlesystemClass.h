// --------------------------------------------------------------------------------------------
// Filename: ParticlesystemClass.h
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
// PURPOSE: Adopt the base idea of https://rastertek.com/tertut12.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"

struct ParticleType
{
	float positionX, positionY, positionZ;
	float red, green, blue;
	float velocity;
	bool active;
};

class ParticlesystemClass
{
public:
	ParticlesystemClass();
	~ParticlesystemClass();
	bool Initialize(void* ctx, void* Driver);
	void Update();
	void Render(void* pContext, float fadeLight);
	void Shutdown();

private:
	float m_particleDeviationX=0, m_particleDeviationY = 0, m_particleDeviationZ = 0;
	float m_particleVelocity = 0, m_particleVelocityVariation = 0;
	float m_particleSize = 0, m_particlesPerSecond = 0;
	int m_maxParticles = 0;
	int m_currentParticleCount=0;
	float m_accumulatedTime=0;
	int m_vertexCount = 0;

	ParticleType* m_particleList = NULL;

	std::vector<ModelTextureWaterfall> model_waterfall;

	ID3D11Buffer* m_vertexBuffer11 = NULL;
	ModelTextureWaterfall* m_vertices = NULL;
	
	bool InitializeParticleSystem(void* ctx, void* Driver);

	void KillParticles();
	void EmitParticles(float frameTime);
	void UpdateParticles(float frameTime);
	void UpdateBuffers(ID3D11DeviceContext* deviceContext);
	
};
