// --------------------------------------------------------------------------------------------
// Filename: ParticlesystemClass.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------

#include "platform.h"
#if defined MAIN_RENDER_WATER_FALL
#include "ParticlesystemClass.h"
#include "vertexTypes.h"
#include "mem_leak.h"
#include "DXmodelClass.h"

ParticlesystemClass particlesystemClass;

ParticlesystemClass::ParticlesystemClass()
{
	CLASSLOADER();
}

ParticlesystemClass::~ParticlesystemClass()
{
	Shutdown();
	CLASSDELETE();
}

void ParticlesystemClass::Shutdown()
{
	SAFE_DELETE_ARRAY(m_particleList);
	//SAFE_DELETE (m_vertexBuffer11); //deleted by DXmodelclass
	m_vertexBuffer11 = NULL;
	SAFE_DELETE_ARRAY(m_vertices);
}

bool ParticlesystemClass::Initialize(void* ctx, void* Driver)
{
	bool result = true;

	// Initialize the particle system.
	IF_NOT_RETURN_FALSE(InitializeParticleSystem(ctx, Driver));

	return result;
}

bool ParticlesystemClass::InitializeParticleSystem(void* ctx, void* Driver)
{
	DX11Class* m_driver11 = (DirectX::DX11Class*)Driver;
	ID3D11DeviceContext* pContext11 = (ID3D11DeviceContext*)ctx;

	// Set the random deviation of where the particles can be located when emitted.
	m_particleDeviationX = 0.30f; //0.30f 0.40f
	m_particleDeviationY = 0.2f;
	m_particleDeviationZ = 0.2f;

	// Set the speed and speed variation of particles.
	m_particleVelocity = 2.5f;
	m_particleVelocityVariation = 0.4;	//0.3f 0.5f

	// Set the physical size of the particles.
	m_particleSize = 0.15f;	 //0.2 0.4

	// Set the number of particles to emit per second.
	m_particlesPerSecond = 300.0f;	// 80-250

	// Set the maximum number of particles allowed in the particle system.
	m_maxParticles = 1500; // 500-5000

	// Initialize the current particle count to zero since none are emitted yet.
	m_currentParticleCount = 0;

	// Clear the initial accumulated time for the particle per second emission rate.
	m_accumulatedTime = 0.0f;

	// Create the particle list.
	m_particleList = NEW ParticleType[m_maxParticles];
	if (!m_particleList) { return false; }

	// Initialize the particle list.
	for (int i = 0; i < m_maxParticles; i++)
		m_particleList[i].active = false;

	//woma_waterfall
	ModelTextureWaterfall v;
	m_vertexCount = m_maxParticles * 6;
	for (size_t i = 0; i < m_vertexCount; i++)
		model_waterfall.push_back(v);

	std::vector<STRING> waterfall_textures;
	waterfall_textures.push_back(WATERFALLTEXTUREWATERFILENAME);
	waterfall_textures.push_back(WATERFALLTEXTURENOISEFILENAME);
	waterfall_textures.push_back(WATERFALLTEXTUREFOAMFILENAME);

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(SystemHandle->m_Application->model_waterfall, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION (SystemHandle->m_Application->model_waterfall, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
	ASSERT(SystemHandle->m_Application->model_waterfall->LoadWaterFall(pContext11, TEXT("waterfall"),
																	m_driver11, SHADER_USE_WATERFALL,
																	&waterfall_textures, &model_waterfall, NULL));

	m_vertexBuffer11 = ((DXmodelClass*)SystemHandle->m_Application->model_waterfall)->m_vertexBuffer11;

	// Create the vertex array for the particles that will be rendered.
	m_vertices = NEW ModelTextureWaterfall[m_vertexCount];
	IF_NOT_RETURN_FALSE(m_vertices);

	return true;
}

void ParticlesystemClass::KillParticles()
{
	// Kill all the particles that have gone below a certain height range.
	for (int i = 0; i < m_maxParticles; i++)
	{
		if ((m_particleList[i].active == true) && (m_particleList[i].positionY < -3.0f)) // END OF WATERFALL
		{
			m_particleList[i].active = false;
			m_currentParticleCount--;

			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
			for (int j = i; j < m_maxParticles - 1; j++)
			{
				m_particleList[j].positionX = m_particleList[j + 1].positionX;
				m_particleList[j].positionY = m_particleList[j + 1].positionY;
				m_particleList[j].positionZ = m_particleList[j + 1].positionZ;
				m_particleList[j].red = m_particleList[j + 1].red;
				m_particleList[j].green = m_particleList[j + 1].green;
				m_particleList[j].blue = m_particleList[j + 1].blue;
				m_particleList[j].velocity = m_particleList[j + 1].velocity;
				m_particleList[j].active = m_particleList[j + 1].active;
			}
		}
	}

	return;
}


void ParticlesystemClass::EmitParticles(float frameTime)
{
	bool emitParticle, found;
	float positionX, positionY, positionZ, velocity, red, green, blue;
	int index, i, j;

	// Increment the frame time.
	m_accumulatedTime += frameTime;

	// Set emit particle to false for now.
	emitParticle = false;

	// Check if it is time to emit a new particle or not.
	if (m_accumulatedTime > (1000.0f / m_particlesPerSecond))
	{
		m_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per frame.
	if ((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
	{
		m_currentParticleCount++;

		// Now generate the randomized particle properties.
		positionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationX;
		positionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationY;
		positionZ = (((float)rand() - (float)rand()) / RAND_MAX / 100) * m_particleDeviationZ;

		velocity = m_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_particleVelocityVariation;

		red = 107 / 256.0f;
		green = 210 / 256.0f;
		blue = 232 / 256.0f;

		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array.
		// We will sort using Z depth so we need to find where in the list the particle should be inserted.
		index = 0;
		found = false;
		while (!found)
		{
			if ((m_particleList[index].active == false) || (m_particleList[index].positionZ < positionZ))
				found = true;
			else
				index++;
		}

		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle.
		i = m_currentParticleCount;
		j = i - 1;

		while (i != index)
		{
			m_particleList[i].positionX = m_particleList[j].positionX;
			m_particleList[i].positionY = m_particleList[j].positionY;
			m_particleList[i].positionZ = m_particleList[j].positionZ;
			m_particleList[i].red = m_particleList[j].red;
			m_particleList[i].green = m_particleList[j].green;
			m_particleList[i].blue = m_particleList[j].blue;
			m_particleList[i].velocity = m_particleList[j].velocity;
			m_particleList[i].active = m_particleList[j].active;
			i--;
			j--;
		}

		// Now insert it into the particle array in the correct depth order.
		m_particleList[index].positionX = positionX;
		m_particleList[index].positionY = positionY;
		m_particleList[index].positionZ = positionZ;
		m_particleList[index].red = red;
		m_particleList[index].green = green;
		m_particleList[index].blue = blue;
		m_particleList[index].velocity = velocity;
		m_particleList[index].active = true;
	}

	return;
}

void ParticlesystemClass::UpdateParticles(float frameTime)
{
	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
	for (int i = 0; i < m_currentParticleCount; i++)
		m_particleList[i].positionY = m_particleList[i].positionY - (m_particleList[i].velocity * frameTime * 0.001f);

	return;
}

void ParticlesystemClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	int index, i;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DXTextureTextureWaterfallType* verticesPtr = NULL;

	// Initialize vertex array to zeros at first.
	memset(m_vertices, 0, (sizeof(DXTextureTextureWaterfallType) * m_vertexCount));

	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.
	index = 0;

	for (i = 0; i < m_currentParticleCount; i++)
	{
		// Bottom left.
		m_vertices[index].x = m_particleList[i].positionX - m_particleSize;
		m_vertices[index].y = m_particleList[i].positionY - m_particleSize;
		m_vertices[index].z = m_particleList[i].positionZ;
		m_vertices[index].tu = 0.0f;
		m_vertices[index].tv = 1.0f;
		m_vertices[index].r = m_particleList[i].red;
		m_vertices[index].g = m_particleList[i].green;
		m_vertices[index].b = m_particleList[i].blue;
		m_vertices[index].a = 1.0f;
		index++;

		// Top left.
		m_vertices[index].x = m_particleList[i].positionX - m_particleSize;
		m_vertices[index].y = m_particleList[i].positionY + m_particleSize;
		m_vertices[index].z = m_particleList[i].positionZ;
		m_vertices[index].tu = 0.0f;
		m_vertices[index].tv = 0.0f;
		m_vertices[index].r = m_particleList[i].red;
		m_vertices[index].g = m_particleList[i].green;
		m_vertices[index].b = m_particleList[i].blue;
		m_vertices[index].a = 1.0f;
		index++;

		// Bottom right.
		m_vertices[index].x = m_particleList[i].positionX + m_particleSize;
		m_vertices[index].y = m_particleList[i].positionY - m_particleSize;
		m_vertices[index].z = m_particleList[i].positionZ;
		m_vertices[index].tu = 1.0f;
		m_vertices[index].tv = 1.0f;
		m_vertices[index].r = m_particleList[i].red;
		m_vertices[index].g = m_particleList[i].green;
		m_vertices[index].b = m_particleList[i].blue;
		m_vertices[index].a = 1.0f;
		index++;

		// Bottom right.
		m_vertices[index].x = m_particleList[i].positionX + m_particleSize;
		m_vertices[index].y = m_particleList[i].positionY - m_particleSize;
		m_vertices[index].z = m_particleList[i].positionZ;
		m_vertices[index].tu = 1.0f;
		m_vertices[index].tv = 1.0f;
		m_vertices[index].r = m_particleList[i].red;
		m_vertices[index].g = m_particleList[i].green;
		m_vertices[index].b = m_particleList[i].blue;
		m_vertices[index].a = 1.0f;
		index++;

		// Top left.
		m_vertices[index].x = m_particleList[i].positionX - m_particleSize;
		m_vertices[index].y = m_particleList[i].positionY + m_particleSize;
		m_vertices[index].z = m_particleList[i].positionZ;
		m_vertices[index].tu = 0.0f;
		m_vertices[index].tv = 0.0f;
		m_vertices[index].r = m_particleList[i].red;
		m_vertices[index].g = m_particleList[i].green;
		m_vertices[index].b = m_particleList[i].blue;
		m_vertices[index].a = 1.0f;
		index++;

		// Top right.
		m_vertices[index].x = m_particleList[i].positionX + m_particleSize;
		m_vertices[index].y = m_particleList[i].positionY + m_particleSize;
		m_vertices[index].z = m_particleList[i].positionZ;
		m_vertices[index].tu = 1.0f;
		m_vertices[index].tv = 0.0f;
		m_vertices[index].r = m_particleList[i].red;
		m_vertices[index].g = m_particleList[i].green;
		m_vertices[index].b = m_particleList[i].blue;
		m_vertices[index].a = 1.0f;
		index++;
	}

	// Lock the vertex buffer.
	IF_FAILED_THROW_EXCEPTION(deviceContext->Map(m_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (DXTextureTextureWaterfallType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)m_vertices, (sizeof(DXTextureTextureWaterfallType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer11, 0);
}

void ParticlesystemClass::Update()
{
	float frameTime = SystemHandle->m_Application->dt / 2.5f;

	// Release old particles.
	KillParticles();

	// Emit new particles.
	EmitParticles(frameTime);

	// Update the position of the particles.
	UpdateParticles(frameTime);
}

void ParticlesystemClass::Render(void* pContext, float fadeLight)
{
	static VirtualModelClass* model_waterfall=NULL;

	if (model_waterfall == NULL)
	{
		model_waterfall = SystemHandle->m_Application->model_waterfall;
		model_waterfall->scale(6, 6, 6);
		model_waterfall->rotateY(PI / 2);
		model_waterfall->translation(138, 15, 57);
	}

	// Update the dynamic vertex buffer with the new position of each particle
	UpdateBuffers((ID3D11DeviceContext*)pContext);

	// Render the model (which will use the waterfall SHADER)
	((DXmodelClass*)model_waterfall)->model_fade = fadeLight;
	((DXmodelClass*)model_waterfall)->m_Shader11->PSfade = fadeLight;
	model_waterfall->Render(pContext);
}
#endif
