// --------------------------------------------------------------------------------------------
// Filename: vkModelClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234526920;

#include "platform.h"

#if DX_ENGINE_LEVEL >= 21 && defined VULKAN
#include "OSengine.h"
#if defined VULKAN
#include "vkModelClass.h"
#endif

#include "GLmathClass.h" // Shared OPENGL / Vulkan

extern GLtextureClass* GetOrCreateTexture(WomaDriverClass* driver, const STRING& filename, void* pContext, bool Model3D);

#if DX_ENGINE_LEVEL >= 36 && defined USE_RENDER_TO_TEXTURE
#include "VKrenderTextureClass.h"
#endif

vkModelClass::vkModelClass(bool model3d, PRIMITIVE_TOPOLOGY primitive, bool computeNormals, bool modelHASshadow, bool modelRENDERshadow)
{
	CLASSLOADER();

	// SUPER: ----------------------------------------------------------------------
	m_ObjId = -1;
	ModelShaderType = SHADER_AUTO;

	Model3D = model3d;
	ModelHASfog = false;
	ModelHASlight = true; // Have to be true!
	ModelHASColorMap = false;
	PrimitiveTopology = primitive;

	PosX = PosY = PosZ = 0;

	SpriteTextureWidth = NULL;
	SpriteTextureHeight = NULL;

#if defined USE_VIEW2D_SPRITES
	m_xTexture = 1.0f;
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	ModelCastShadow = modelHASshadow;
	ModelRenderShadow = modelRENDERshadow;
#endif

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
	m_instanceCount = 0;
	m_instanceBuffer = NULL;
#endif

	ModelHASNormals = false;
	ModelcomputeNormals = computeNormals;

	// Initialize the world/model matrix to the identity matrix:
	m_worldMatrix = m_worldMatrix.mat4identity();  //Identity();
	m_viewMatrix = m_viewMatrix.mat4identity();
}

vkModelClass::~vkModelClass() 
{ 
	//Shutdown();
	CLASSDELETE(); 
}

void vkModelClass::Shutdown()
{
	VkDevice vkDevice = ((vkClass*)m_Driver)->m_device;

	SAFE_SHUTDOWN(m_Shader);

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	SAFE_SHUTDOWN(m_ShaderShadowMap);
	if (m_vertexBuffer_4Shadows != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(vkDevice, m_vertexBuffer_4Shadows, nullptr);
		m_vertexBuffer_4Shadows = VK_NULL_HANDLE;
	}

	if (m_vertexBufferMemory_4Shadows != VK_NULL_HANDLE)
	{
		vkFreeMemory(vkDevice, m_vertexBufferMemory_4Shadows, nullptr);
		m_vertexBufferMemory_4Shadows = VK_NULL_HANDLE;
	}
	SAFE_DELETE(modelShadowMapVertex);
#endif

	if (m_vertexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(vkDevice, m_vertexBuffer, nullptr);
		m_vertexBuffer = VK_NULL_HANDLE;
	}

	if (m_vertexBufferMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(vkDevice, m_vertexBufferMemory, nullptr);
		m_vertexBufferMemory = VK_NULL_HANDLE;
	}

	if (m_indexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(vkDevice, m_indexBuffer, nullptr);
		m_indexBuffer = VK_NULL_HANDLE;
	}

	if (m_indexBufferMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(vkDevice, m_indexBufferMemory, nullptr);
		m_indexBufferMemory = VK_NULL_HANDLE;
	}

	for (UINT i = 0; i < vertexBuffers.size(); i++)
	{
		if (vertexBuffers[i] != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(vkDevice, vertexBuffers[i], nullptr);
			vertexBuffers[i] = VK_NULL_HANDLE;
		}
	}

	for (UINT i = 0; i < vertexMemories.size(); i++)
	{
		if (vertexMemories[i] != VK_NULL_HANDLE)
		{
			vkFreeMemory(vkDevice, vertexMemories[i], nullptr);
			vertexMemories[i] = VK_NULL_HANDLE;
		}
	}

	vertexBuffers.clear();
	vertexMemories.clear();

	for (UINT i = 0; i < meshSRV.size(); i++)
		meshSRV[i] = NULL;

	meshSRV.clear();
	m_Texture = NULL;

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
	if (m_instanceBuffer)
	{
		vkDestroyBuffer(vkDevice, m_instanceBuffer, nullptr);
		m_instanceBuffer = VK_NULL_HANDLE;
	}

	if (m_instanceBufferMemory)
	{
		vkFreeMemory(vkDevice, m_instanceBufferMemory, nullptr);
		m_instanceBufferMemory = VK_NULL_HANDLE;
	}
#endif
}

void vkModelClass::RenderWithFade(void* pContext, float fadeLight, bool FOG, UINT pass, void* lightViewMatrix, void* lightProjectionMatrix)
{
	m_Shader->PSfade = fadeLight;

	model_fade = fadeLight;

	if (RENDER_PAGE >= 51) {
		if (WOMA::AppSettings->START_FOG > 0 && WOMA::AppSettings->END_FOG > 0)
			m_Shader->hasFog = FOG;
	}

	Render(pContext, 0, 0, 0, pass, lightViewMatrix, lightProjectionMatrix);
}

// ----------------------------------------------------------------------------------------
void vkModelClass::SetGeometryBuffers(void* pContext, UINT pass)
// ----------------------------------------------------------------------------------------
{
	VkCommandBuffer commandBuffer = (VkCommandBuffer)pContext;
	VkBuffer		vertexBuffer = m_vertexBuffer;

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	if (pass == PASS_SHADOWS && ModelCastShadow)
	{
		ASSERT_DEBUG(m_vertexBuffer_4Shadows);
		vertexBuffer = m_vertexBuffer_4Shadows;
	}
#endif

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES
	// --------------------------------------------------------------------
	// RENDER: 1 - Vertex Buffer (instanced)
	// --------------------------------------------------------------------
	if (ModelShaderType == SHADER_TEXTURE_LIGHT_INSTANCED ||
		ModelShaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED ||
		ModelShaderType == SHADER_NORMAL_BUMP_INSTANCED ||
		ModelShaderType == SHADER_TEXTURE_GS_INSTANCED ||
		ModelShaderType == SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED)
	{
		VkDeviceSize offsets[2] = { 0, 0 };
		VkBuffer buffers[2] = { vertexBuffer, m_instanceBuffer };
		vkCmdBindVertexBuffers(commandBuffer, 0, 2, buffers, offsets);
	}
	else
#endif
	{
		// --------------------------------------------------------------------
		// RENDER: 1 - Vertex Buffer
		// --------------------------------------------------------------------
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);
	}

	// --------------------------------------------------------------------
	// RENDER: 2 Index Buffer
	// --------------------------------------------------------------------
	vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void vkModelClass::UpdateAlphaMapDescriptor(vkShaderClass* useShader, UINT frame, UINT descriptorIndex)
{
	GLtextureClass* alphaTextureMap;
	if (useShader->hasAlfaMap)
		alphaTextureMap = ((GLtextureClass*)obj3d.material[obj3d.subsetMaterialArray[descriptorIndex]].alfaMap);
	else
		alphaTextureMap = ((GLtextureClass*)m_driver->g_allTexturePointerArray[0]); //dummy texture

	//if (currentAlphaMap == alphaTextureMap)
	//	return;

	//currentAlphaMap = alphaTextureMap;

	VkDescriptorImageInfo info{};
	info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	info.imageView = alphaTextureMap->m_vkImageView;
	info.sampler = m_driver->m_vkSampler[alphaTextureMap->m_wrap];

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = useShader->GetDescriptorSet(frame, descriptorIndex);
	write.dstBinding = 3;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.pImageInfo = &info;

	//womalog("AlphaMap: frame=%u set=%p binding=3 SRV=%p\n",
	//	frame,
	//	write.dstSet,
	//	info.imageView);

	vkUpdateDescriptorSets(m_driver->m_device, 1, &write, 0, nullptr);
}

void vkModelClass::RenderSubMesh(void* pContext, WomaDriverClass* driver, mat4* m_world, mat4* viewRender, mat4* projectionMatrix, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	VkDevice vkDevice = m_driver->m_device;
	VkCommandBuffer cmd = (VkCommandBuffer)pContext;
	vkShaderClass* useShader= m_Shader;
	UINT frame = m_driver->VKwindowsToRenderArray[0].m_currentFrame; //m_driver->m_frameIndex; ////monitor

	// Get shader to Use:
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	if (pass == PASS_SHADOWS && m_ShaderShadowMap)
	{
		useShader = m_ShaderShadowMap;
		if (pass == PASS_SHADOWS && m_ShaderShadowMap)
		{
			useShader = m_ShaderShadowMap;
		}
	}
#endif

	if (!useShader)
		return;

	useShader->hasTexture = false;

	UINT texture_index = 0;

	for (UINT i = 0; i < obj3d.meshSubsets; i++)
	{
		useShader->currentDescriptorSubset = i; // Set current subset to use on shader

		//ALFA-COLOR:
		if ((UINT)obj3d.material[obj3d.subsetMaterialArray[i]].transparent == pass || pass == PASS_SHADOWS)
		{
			useShader->hasTexture = obj3d.material[obj3d.subsetMaterialArray[i]].hasTexture;

			if (!useShader->hasTexture)
			{
				// Material WITH COLOR ONLY
				// ------------------------
				useShader->pixelColor = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor;		// Set the new Pixel Color!)

				if (!FRAME[frame])
				{
					FRAME[frame] = true;

					constexpr UINT fallbackSlot = 0; //dummy texture
					GLtextureClass* texture = static_cast<GLtextureClass*>(m_driver->g_allTexturePointerArray[fallbackSlot]);
					if (!textureWrite[fallbackSlot].sType)
					{
						textureDescriptor[fallbackSlot].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						textureDescriptor[fallbackSlot].imageView = texture->m_vkImageView;
						textureDescriptor[fallbackSlot].sampler = m_driver->m_vkSampler[texture->m_wrap];

						textureWrite[fallbackSlot].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						textureWrite[fallbackSlot].dstBinding = 1;
						textureWrite[fallbackSlot].descriptorCount = 1;
						textureWrite[fallbackSlot].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						textureWrite[fallbackSlot].pImageInfo = &textureDescriptor[fallbackSlot];
					}

					textureWrite[fallbackSlot].dstSet = useShader->GetDescriptorSet(frame, i);
					vkUpdateDescriptorSets(vkDevice, 1, &textureWrite[fallbackSlot], 0, nullptr);
				}
				useShader->SetShaderParameters(pass, pContext, m_world, viewRender, projectionMatrix);	// UPDATE CONST. BUFFER for COLOR
			}
			else
			{
					// Material - WITH TEXTURE:
					// ---------------------------------
					if (pass < PASS_SHADOWS) // if shadow pass skip this
					{
						useShader->diffuseColor = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor;
						useShader->ambientColor = obj3d.material[obj3d.subsetMaterialArray[i]].ambientColor;
						useShader->emissiveColor = obj3d.material[obj3d.subsetMaterialArray[i]].emissiveColor;

				#if defined USE_ALFA_MAP // Alfa Map
						useShader->hasAlfaMap = (obj3d.material[obj3d.subsetMaterialArray[i]].alfaMap) ? true : false;
				#endif

						// Specular Light:
				#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
						useShader->hasSpecular = obj3d.material[obj3d.subsetMaterialArray[i]].bSpecular;
						if (useShader->hasSpecular) {
							useShader->specularColor = obj3d.material[obj3d.subsetMaterialArray[i]].specularColor;	// NEW Specular
							useShader->nShininess = (float)obj3d.material[obj3d.subsetMaterialArray[i]].nShininess;	// NEW Shininess
						}
				#endif
						if (pass < PASS_SHADOWS)
						{
							texture_index = obj3d.material[obj3d.subsetMaterialArray[i]].texArrayIndex;
							ASSERT_DEBUG(texture_index < MAX_VULKAN_TEXTURES_MODEL); // Each vkModelClass supports up to (MAX_VULKAN_TEXTURES_MODEL)24 texture slots (texArrayIndex 0..23).

							auto* texture = meshSRV[texture_index];
							if (!textureWrite[texture_index].sType)
							{
								// [0] Set: shaderTexture - Vulkan: Descriptor Set 0 / Binding 1 (t0)
								//pContext->PSSetShaderResources(0, 1, &meshSRV11[texture_index]);// set current texture

								textureDescriptor[texture_index].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
								textureDescriptor[texture_index].imageView = meshSRV[texture_index]->m_vkImageView;
								textureDescriptor[texture_index].sampler = m_driver->m_vkSampler[texture->m_wrap];; //meshSRV[texture_index]->m_vkSampler

								textureWrite[texture_index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
								textureWrite[texture_index].dstBinding = 1;
								textureWrite[texture_index].descriptorCount = 1;
								textureWrite[texture_index].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
								textureWrite[texture_index].pImageInfo = &textureDescriptor[texture_index];
							}

							textureWrite[texture_index].dstSet = useShader->GetDescriptorSet(frame, i);
							vkUpdateDescriptorSets(vkDevice, 1, &textureWrite[texture_index], 0, nullptr);

							// [1] Set: AlfaMapTexture - Vulkan: Descriptor Set 0 / Binding 2 (t1)
				#if defined USE_ALFA_MAP //RENDER_OBJ_WITH_ALFA

							switch (useShader->m_shaderType)
							{
							case SHADER_TEXTURE_LIGHT_ALPHAMAP:		//light34.frag
							case SHADER_NORMAL_BUMP:				//bump.frag
							case SHADER_TEXTURE_LIGHT_INSTANCED:	//lightInstance.frag
							case SHADER_TEXTURE_LIGHT_RENDERSHADOW:			//36 Draw Shadows               (for chess/board) lightShadow.frag
							case SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED:	//41 036LightRenderShadow.hlsl  (for chess/board) lightInstance.frag		
							case SHADER_Double_Color_Terrain:				//053Double_Color_Terrain.frag

								//pContext->PSSetShaderResources(1, 1, &obj3d.material[obj3d.subsetMaterialArray[i]].alfaMap);		// set current alfaMap

								UpdateAlphaMapDescriptor(useShader, frame, i);
								break;
							}
				#endif

							// [2] Set: ShadowMapTexture - Vulkan: Descriptor Set 0 / Binding 3 (t2)
				#if defined USE_SHADOW_MAP// && defined USE_SCENE_MANAGER
							if (useShader->m_shaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW)
							{
								//pContext->PSSetShaderResources(2, 1, &SystemHandle->m_Application->m_TextureWithShadows->m_shaderTextureResourceView);

								GLtextureClass* shadowSRV = ((VKrenderTextureClass*)SystemHandle->m_Application->m_TextureWithShadows)->m_shaderTextureResourceView;

								shadowtextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
								shadowtextureDescriptor.imageView = shadowSRV->m_vkImageView;
								shadowtextureDescriptor.sampler = m_driver->m_vkSampler[texture->m_wrap]; //shadowSRV->m_vkSampler;

								shadowtextureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
								shadowtextureWrite.dstBinding = 5; // Map on binding = 5 do lightShadow.frag!
								shadowtextureWrite.descriptorCount = 1;
								shadowtextureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
								shadowtextureWrite.pImageInfo = &shadowtextureDescriptor;

								shadowtextureWrite.dstSet = useShader->GetDescriptorSet(frame, i); //m_Shader->m_descriptorSet[m_driver->VKwindowsToRenderArray[0].m_currentFrame];
								vkUpdateDescriptorSets(vkDevice, 1, &shadowtextureWrite, 0, nullptr);
							}
				#endif
							// [3] Set: TangentMapTexture
							useShader->hasNormMap = obj3d.material[obj3d.subsetMaterialArray[i]].hasNormMap;
							if (useShader->hasNormMap)
							{
								//pContext->PSSetShaderResources(3, 1, &meshSRV11[obj3d.material[obj3d.subsetMaterialArray[i]].normMapTexArrayIndex]);

								ASSERT_DEBUG(meshSRV[obj3d.material[obj3d.subsetMaterialArray[i]].normMapTexArrayIndex]);

								auto* normMapTex = ((GLtextureClass*)meshSRV[obj3d.material[obj3d.subsetMaterialArray[i]].normMapTexArrayIndex]);
								if (currentNormalMap != normMapTex)
								{
									currentNormalMap = normMapTex;

									// [1] Set: AlphaMap texture
									bumptextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

									bumptextureDescriptor.imageView = normMapTex->m_vkImageView;
									bumptextureDescriptor.sampler = m_driver->m_vkSampler[texture->m_wrap]; //normMapTex->m_vkSampler;

									bumptextureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
									bumptextureWrite.dstBinding = 4;
									bumptextureWrite.descriptorCount = 1;
									bumptextureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
									bumptextureWrite.pImageInfo = &bumptextureDescriptor;
								}

								bumptextureWrite.dstSet = useShader->GetDescriptorSet(frame, i); //m_Shader->m_descriptorSet[m_driver->VKwindowsToRenderArray[0].m_currentFrame];
								vkUpdateDescriptorSets(vkDevice, 1, &bumptextureWrite, 0, nullptr);
							}

						}
					}
					{
							useShader->SetShaderParameters(pass, pContext, m_world, viewRender, projectionMatrix, (mat4*)lightViewMatrix, (mat4*)ShadowProjectionMatrix);// UPDATE CONST. BUFFER
					}
			
			}
			
			// Render:
			int indexStart = obj3d.meshSubsetIndexStart[i];
			int indexDrawAmount = obj3d.meshSubsetIndexStart[i + 1] - obj3d.meshSubsetIndexStart[i];

			useShader->RenderShader(pass, pContext, texture_index, indexDrawAmount, indexStart);				// Now render the prepared buffers with the shader
		}
	}
}

void vkModelClass::Render(/*VkCommandBuffer*/void* pContext, UINT threadID, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix, bool disableFog)
{
	VkDevice vkDevice = m_driver->m_device;

	if (ModelShaderType > SHADER_COLOR)
	{
		m_Shader->hasAlfaColor = ModelHASAlfaColor;			//IMPORTANT LEVEL 62!
		m_Shader->alfaColor = ModelAlfaColor;					//IMPORTANT LEVEL 62!
	}

	// ----------------------------------------------------------------------------------------
	// Step 1 - Put the "vertex", "index" and "instances" (if exist) buffers on the graphics pipeline to prepare them for drawing:
	// ----------------------------------------------------------------------------------------
	// Render 1 ├── Bind VB
	// Render 2 ├── Bind IB
	SetGeometryBuffers(pContext, pass);

	// ----------------------------------------------------------------------------------------
	// Step 2 - Setup matrices (Unified CBUFFER path)
	// Use caller-provided View/Projection for MiniMap and Shadow-style passes.
	// ----------------------------------------------------------------------------------------
	if (projection == PROJECTION_MINIMAP) 
	{
		projectionMatrix = *((mat4*)ShadowProjectionMatrix);	//Use provided projection: MINI-MAP
		m_viewMatrix = *((mat4*)lightViewMatrix);				//Use provided view
	}
	else
	{
		if (camera != CAMERA_MINIMAP)
		{
			projectionMatrix = m_driver->GetProjectionMatrix(camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
		} else {
			projectionMatrix = *(mat4*)ShadowProjectionMatrix;   //Use provided projection: for Shadows
		}
		m_viewMatrix = m_driver->GetViewMatrix(camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
	}

	// ----------------------------------------------------------------------------------------
	// Step 3: Render Complex Mesh (OBJ/W3D):
	// ----------------------------------------------------------------------------------------
	//
	// Pass Extra Parameters:
	//
	//Single-Material in limit meshSRV size =1
#if defined USE_VIEW2D_SPRITES
	m_Shader->PSfade = model_fade;
#endif
	m_Shader->shaderTypeParameter = (float)shaderTypeParameter;

	//Multi-Material:
	if ((obj3d.material.size() > 0) 
		&& (ModelShaderType != SHADER_FIRE)
		&& (ModelShaderType != SHADER_TEXTURE_LIGHT_FAST)
		&& (ModelShaderType != SHADER_BILLBOARD_ATLAS_FAST)
		)
	{
		RenderSubMesh(pContext, m_driver, &m_worldMatrix, &m_viewMatrix, &projectionMatrix, pass, lightViewMatrix, ShadowProjectionMatrix); // Multiple Material
	}
	else
	{
		//==================================================================================================
		// WOMA Graphics API Rosetta Stone
		// Equivalent concepts across DX11, DX12 and Vulkan.
		//--------------------------------------------------------------------------------------------------
		// GOAL                DX11                      DX12                           VULKAN
		//--------------------------------------------------------------------------------------------------
		// Create Texture      CreateTexture2D          CreateCommittedResource        vkCreateImage
		// Create SRV          CreateShaderResourceView CreateShaderResourceView       VkDescriptorImageInfo
		//
		// Bind Shader->Texture
		//                     PSSetShaderResources     CreateShaderResourceView*      vkUpdateDescriptorSets
		//
		// Activate for Draw
		//                     PSSetShaderResources     SetGraphicsRootDescriptorTable vkCmdBindDescriptorSets
		//--------------------------------------------------------------------------------------------------
		// * DX12 creates the SRV in a descriptor heap; binding happens later through the root descriptor
		//   table. Vulkan separates descriptor update (CPU-side) from descriptor binding (command buffer).
		//==================================================================================================
		UINT frame = m_driver->VKwindowsToRenderArray[0].m_currentFrame; //m_driver->m_frameIndex; 
		if (ModelShaderType >= SHADER_TEXTURE) 
		{
			if (!FRAME[frame]) 
			{
				FRAME[frame] = true;
				VkDescriptorImageInfo textureDescriptor = {};

				textureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				VkWriteDescriptorSet textureWrite = {};
				textureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				textureWrite.dstBinding = 1;
				textureWrite.descriptorCount = 1;
				textureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				textureWrite.pImageInfo = &textureDescriptor;

				if (m_Shader->singleDescriptorSubset)
					textureWrite.dstSet = m_Shader->m_descriptorSet30[frame];	//monitor
				else
					textureWrite.dstSet = m_Shader->GetDescriptorSet(frame, 0); //Index = 0

				for (UINT i = 0; i < meshSRV.size(); i++)
				{
					uint32_t binding = i + 1;
					switch(m_Shader->m_shaderType)
					{
					case SHADER_Terrain_Texture_DEMO60:
					case SHADER_Terrain_Texture_DEMO61:
						if(binding>=2)
							binding++;
						break;
					}

					textureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					textureDescriptor.imageView = meshSRV[i]->m_vkImageView;

				if (m_Shader->m_shaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW ||
					m_Shader->m_shaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED ||
					m_Shader->m_shaderType == SHADER_NORMAL_BUMP_INSTANCED)
				{
					textureDescriptor.sampler = m_driver->m_vkSampler11[meshSRV[i]->m_wrap];
				} else
					textureDescriptor.sampler = m_driver->m_vkSampler[meshSRV[i]->m_wrap];

					VkWriteDescriptorSet textureWrite{};
					textureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

					//textureWrite.dstSet = m_Shader->GetDescriptorSet(frame, 0);
				if (m_Shader->singleDescriptorSubset)
					textureWrite.dstSet = m_Shader->m_descriptorSet30[frame];	//monitor
				else
					textureWrite.dstSet = m_Shader->GetDescriptorSet(frame, 0); //Index = 0

					textureWrite.dstBinding = binding;              // binding 1..11
					textureWrite.descriptorCount = 1;
					textureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					textureWrite.pImageInfo = &textureDescriptor;

					vkUpdateDescriptorSets(vkDevice, 1, &textureWrite, 0, nullptr);
				}
			}
		}
		if (ModelShaderType == SHADER_Double_Color_Terrain)
			UpdateAlphaMapDescriptor(m_Shader, frame, 0);

		m_Shader->Render(pass, pContext, m_indexCount, &m_worldMatrix, &m_viewMatrix, &projectionMatrix);
	}
}

bool vkModelClass::LoadColor(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type,
							 std::vector<ModelColorVertexType>* model, std::vector<UINT>* indexList, 
							 UINT instanceCount)
{
#if defined _DEBUG || defined LINUX_PLATFORM //|| defined ANDROID_PLATFORM
	womalogATfirstframe(TEXT("--------------------------------------------------------------\n[%d]: LOADCOLOR(): %s\n"), gettid(), objectName);
#endif

	//LOADDRIVER
	m_driver = (vkClass*)driver;
	ASSERT_DEBUG(driver);

	MODEL_NAME = objectName;
	ASSERT_DEBUG(model);

	m_vertexCount = (UINT)(*model).size();

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_COLOR;
	else
		ModelShaderType = shader_type;

	ASSERT_DEBUG(ModelShaderType == SHADER_COLOR);

	modelColorVertex = model;
	indexModelList = indexList;

	return InitializeVkBuffers(pContext, objectName, NULL);
}

vkShaderClass* vkModelClass::CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType, UINT monitor)
{
	bool result = true;
	vkShaderClass* shader = NEW vkShaderClass();
	IF_NOT_THROW_EXCEPTION(shader);
	
#if defined WINDOWS_PLATFORM
	#define pass_m_hWnd SystemHandle->m_hWnd
#else
	#define pass_m_hWnd NULL
#endif

	result = shader->Initialize(m_ObjId, objectName, ShaderType, ((vkClass*)m_Driver)->m_device, pass_m_hWnd, PrimitiveTopology, monitor);
	IF_NOT_THROW_EXCEPTION(result);

	if (ShaderType >= SHADER_TEXTURE_LIGHT)
		shader->hasLight = ModelHASlight;	// COLOR AND TEXTURE = FALSE

	if (ShaderType >= SHADER_TEXTURE)
		shader->hasFog = ModelHASfog;
	if (ShaderType >= SHADER_TEXTURE_LIGHT)		// COLOR AND TEXTURE = FALSE
		shader->hasNormMap = ModelHASNormals;

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES
	if (ShaderType == SHADER_TEXTURE_LIGHT_INSTANCED ||			//40: INSTANCED like 23 light, but using Instances
		ShaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED ||	//40: Aux. Shader (render in texture), but using Instances (used on 40,41,42)
		ShaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED ||  //41: INSTANCED like 36 shadow, but using Instances
		ShaderType == SHADER_NORMAL_BUMP_INSTANCED || 				//--: INSTANCED like 35 bump, but using Instances
		ShaderType == SHADER_TEXTURE_GS_INSTANCED ||				//77
		ShaderType == SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED		//98
		)
		shader->m_instanceCount = m_instanceCount;
#endif

	if (ShaderType >= SHADER_Double_Color_Terrain)
		shader->hasColorMap = ModelHASColorMap;	// COLOR AND TEXTURE = FALSE

	return shader;
}

// --------------------------------------------------------------------------------------------
bool vkModelClass::InitializeColorBuffers(void* pContext, TCHAR* objectName, void* indices)
// --------------------------------------------------------------------------------------------
{
	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, NULL));

	return true;
}

bool vkModelClass::InitializeTextureBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	
	if (m_Shader->m_shaderType == SHADER_TEXTURE_WATER)
	{
		for (UINT i = 0; i < m_vertexCount; i++)
		{
			ModelTextureWatervertices.push_back(XMFLOAT3(
				(*modelTextureVertex)[i].x,
				(*modelTextureVertex)[i].y,
				(*modelTextureVertex)[i].z));
		}
	}

	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, textureFile));

	return true;
}

bool vkModelClass::LoadTexture(void* pContext, TCHAR* objectName, void* driver,
	SHADER_TYPE shader_type, std::vector<STRING>* textureFile,
	std::vector<ModelTextureVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	//LOADDRIVER
	m_driver = (vkClass*)driver;
	ASSERT_DEBUG(driver);

	MODEL_NAME = objectName;
	m_vertexCount = (UINT)(*model).size();

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_TEXTURE;
	else
		ModelShaderType = shader_type;

	ASSERT_DEBUG(
		ModelShaderType == SHADER_TEXTURE ||					//22
		ModelShaderType == SHADER_TEXTURE_FONT ||				//27
		ModelShaderType == SHADER_TEXTURE_WATER ||				//54
		ModelShaderType == SHADER_FIRE ||						//72
		ModelShaderType == SHADER_USE_CURVED_REAL_SKY_PLANE 	//96
	);

	modelTextureVertex = model;
	indexModelList = indexList;

	return InitializeVkBuffers(pContext, objectName, textureFile);
}


void vkModelClass::GetIndices()
{
	womalog("\nGetIndices()\n");

	int j = 0;
	if (!indexModelList || indexModelList->empty())
	{
		m_indexCount = m_vertexCount;

		indices = NEW UINT[m_indexCount];
		IF_NOT_THROW_EXCEPTION(indices);

		for (UINT i = 0; i < m_indexCount; i++)
		{
			indices[i] = i;						// Load the index array with data:
		}
	}
	else
	{
		m_indexCount = (UINT)indexModelList->size();

		indices = NEW UINT[m_indexCount];
		IF_NOT_THROW_EXCEPTION(indices);

		for (UINT i = 0; i < m_indexCount; i++)
		{
			indices[i] = (*indexModelList)[i];
		}
	}
}

void vkModelClass::Identity()
{
	m_worldMatrix = m_worldMatrix.mat4identity();
}

void vkModelClass::multiply(void* m) // in radians!!
{
	m_worldMatrix = m_worldMatrix * *(mat4*)m;;
}

void vkModelClass::rotateX(float xZrad) // in radians!!
{
	mat4 m = mathClass.rotateX(xZrad);
	m_worldMatrix = m_worldMatrix * m;
}
void vkModelClass::rotateY(float yZrad) // in radians!!
{
	mat4 m = mathClass.rotateY(-yZrad);
	m_worldMatrix = m_worldMatrix * m;
}
void vkModelClass::rotateZ(float rZrad) // in radians!!
{
	mat4 m = mathClass.rotateZ(rZrad);
	m_worldMatrix = m_worldMatrix * m;
}

void vkModelClass::scale(float x, float y, float z)
{
	m_worldMatrix.m[4 * 0 + 0] = x;
	m_worldMatrix.m[4 * 1 + 1] = y;
	m_worldMatrix.m[4 * 2 + 2] = z;
}
void vkModelClass::translation(float x, float y, float z)
{
	m_worldMatrix.m[4 * 3 + 0] = x;
	m_worldMatrix.m[4 * 3 + 1] = y;
	m_worldMatrix.m[4 * 3 + 2] = z;
}

#if defined MAIN_RENDER_LIGHT_RAY
void vkModelClass::UpdateLightRayVertices(void* pContext, std::vector<ModelColorVertexType>* lightVertexVector)
{
	static float m_previousPosX = -10000;
	static float m_previousPosY = -10000;
	static float m_previousPosZ = -10000;

	VkDevice vkDevice = ((vkClass*)m_Driver)->m_device;

	if (!lightVertexVector || lightVertexVector->size() < 2)
		return;

	// --------------------------------------------------------------------
	// The LightRay is defined by the position of its second vertex.
	// --------------------------------------------------------------------
	float positionX = (*lightVertexVector)[1].x;
	float positionY = (*lightVertexVector)[1].y;
	float positionZ = (*lightVertexVector)[1].z;

	// Nothing changed -> vertex buffer does not need to be updated.
	if ((positionX == m_previousPosX) &&
		(positionY == m_previousPosY) &&
		(positionZ == m_previousPosZ))
	{
		return;
	}

	// Remember the new position.
	m_previousPosX = positionX;
	m_previousPosY = positionY;
	m_previousPosZ = positionZ;

	// --------------------------------------------------------------------
	// Save Vulkan: vertex data on model class:
	// --------------------------------------------------------------------
	m_vertexCount = (UINT)lightVertexVector->size();

	modelColorVertex = lightVertexVector;

	// --------------------------------------------------------------------
	// Update Vulkan Vertex Buffer
	// --------------------------------------------------------------------
	const VkDeviceSize vertexBufferSize = sizeof(ModelColorVertexType) * m_vertexCount;

	void* data = nullptr;

	VkResult vkResult = vkMapMemory(vkDevice, m_vertexBufferMemory, 0, vertexBufferSize, 0, &data);
	ASSERT(vkResult == VK_SUCCESS);

	memcpy(data, modelColorVertex->data(), (size_t)vertexBufferSize);

	vkUnmapMemory(vkDevice, m_vertexBufferMemory);
}
#endif


bool vkModelClass::LoadLight(void* pContext, TCHAR* objectName, void* driver,
	SHADER_TYPE shader_type, std::vector<STRING>* textureFiles,
	std::vector<ModelTextureLightVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	//LOADDRIVER:
	m_driver = (vkClass*)driver;
	ASSERT_DEBUG(driver);

	MODEL_NAME = objectName;
	m_vertexCount = (UINT)(*model).size();

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_TEXTURE_LIGHT;
	else
		ModelShaderType = shader_type;

	ASSERT_DEBUG((ModelShaderType == SHADER_TEXTURE_LIGHT) ||				// SHADER_TYPE =  4
		(ModelShaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW) ||			// SHADER_TYPE =  6
		(ModelShaderType == SHADER_TEXTURE_LIGHT_ALPHAMAP) ||				// SHADER_TYPE =  8
		(ModelShaderType == SHADER_TEXTURE_LIGHT_INSTANCED) ||				// SHADER_TYPE =  9	//lvl40
		(ModelShaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED) ||	// SHADER_TYPE = 11
		(ModelShaderType == SHADER_FIRE) ||                                 // SHADER_TYPE = 22
		(ModelShaderType == SHADER_TEXTURE_GS_INSTANCED) ||                 // SHADER_TYPE = 23
		(ModelShaderType == SHADER_TEXTURE_LIGHT_FAST) ||					// SHADER_TYPE = 25
		(ModelShaderType == SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED) ||	// SHADER_TYPE = 31	//lvl98
		(ModelShaderType == SHADER_TEXTURE_LIGHT98)							// SHADER_TYPE = 32	//lvl98
	);

	modelTextureLightVertex = model;
	indexModelList = indexList;


#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	if (ModelCastShadow)
	{
		modelShadowMapVertex = NEW std::vector<ModelShadowMapVertexType>;
		modelShadowMapVertex->reserve(model->size());

		for (const auto& v : *model)
			modelShadowMapVertex->push_back({ v.x, v.y, v.z });
	}
#endif

	return InitializeVkBuffers(pContext, objectName, textureFiles);
}

bool vkModelClass::InitializeTextureLightBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, textureFile));
	return true;
}



bool vkModelClass::InitializeVkBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	bool result = true;

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
	if (m_instanceCount > 0) //1 or more can be Instancing: its not an error, DONT TOUCH THIS!
	{
		if (ModelShaderType == SHADER_TEXTURE_LIGHT)					//23:
			ModelShaderType = SHADER_TEXTURE_LIGHT_INSTANCED;			//40: INSTANCED Switch SHADER_TEXTURE_LIGHT ==> SHADER_TEXTURE_LIGHT_INSTANCED
		#if defined USE_SHADOW_INSTANCES
		else
			if (ModelShaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW)			//36: Aux. Shader
				ModelShaderType = SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED;//40: Aux. Shader Switch SHADER_TEXTURE_LIGHT_SAVESHADOW ==> SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED
		#endif
	}
#endif

	GetIndices();

	// Create the shader to this object:
	// ------------------------------------------------------------------------------------------------
	m_Shader = CreateShader(objectName, ModelShaderType);
	ASSERT_DEBUG(m_Shader);
	IF_NOT_RETURN_FALSE(m_Shader);

	if (ModelShaderType > SHADER_COLOR)
	{
		m_Shader->hasAlfaColor = ModelHASAlfaColor;			//IMPORTANT LEVEL 62!
		m_Shader->alfaColor = ModelAlfaColor;				//IMPORTANT LEVEL 62!
		m_Shader->hasFog = ModelHASfog;						//IMPORTANT LEVEL 62!
#if DX_ENGINE_LEVEL >= 73 && defined BILLBOARD_FOR_WINDY_GRASS
		m_Shader->isAnimatedBill = isAnimatedBill;
#endif
	}

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP// && defined USE_SCENE_MANAGER
	if (ModelCastShadow &&
		((ModelShaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW || ModelShaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW) ||
			(ModelShaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED || ModelShaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED)
			)
		)
	{
		m_Shader->castShadow = true; // Use Shadow Map Result!
	}
	else
	{
		if (!m_ShaderShadowMap)
		{
			// Create AUX Shadow Map:
		#if DX_ENGINE_LEVEL >= 41 && defined USE_SHADOW_INSTANCES // Normal Bump + Instancing 
			if (m_instanceCount > 0)
			{
				if (ModelCastShadow && m_ObjId > -1)
				{
					m_ShaderShadowMap = CreateShader(objectName, SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED);
					IF_NOT_RETURN_FALSE(m_ShaderShadowMap);
				}
			}
			else
		#endif
				if (ModelCastShadow && m_ObjId > -1)
				{
					m_ShaderShadowMap = CreateShader(objectName, SHADER_TEXTURE_LIGHT_SAVESHADOW);
					IF_NOT_RETURN_FALSE(m_ShaderShadowMap);
				}
		}
	}
#endif

	// ----------------------------------------------------------------------------------------------
	// Send Buffers to "Grph. Card.": The vertex and index buffer that hold the geometry for the triangles
	// ----------------------------------------------------------------------------------------------
	IF_NOT_RETURN_FALSE(result);
	switch (ModelShaderType)
	{
	case SHADER_COLOR:
		result = InitializeColorBuffers(pContext, objectName, indices);
		break;

	case SHADER_TEXTURE:
	case SHADER_TEXTURE_FONT:
	case SHADER_TEXTURE_WATER:
	case SHADER_FIRE:
	case SHADER_USE_CURVED_REAL_SKY_PLANE:

		result = InitializeTextureBuffers(pContext, objectName, textureFile);
		break;

	case SHADER_TEXTURE_LIGHT:						//23 LIGHT 
	case SHADER_TEXTURE_LIGHT_ALPHAMAP:				//34
	case SHADER_TEXTURE_LIGHT98:					//98
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW:			//36 Draw Shadows
	case SHADER_TEXTURE_LIGHT_INSTANCED:			//40 INSTANCED like 23 light, but using Instances
	case SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED: //41
	case SHADER_TEXTURE_GS_INSTANCED:               //77
	case SHADER_TEXTURE_LIGHT_FAST:					//83 LIGHT 
	case SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED:  //98
		result = InitializeTextureLightBuffers(pContext, objectName, textureFile);
		IF_NOT_RETURN_FALSE(result);
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
		if (!ModelCastShadow)
#endif
			break;
		// Fall through: also create the auxiliary shadow buffer.

		//NOTE: After: SHADER_TEXTURE_LIGHT
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP// && defined USE_SCENE_MANAGER
	case SHADER_TEXTURE_LIGHT_SAVESHADOW:			//36: Aux. Shader (render in texture)
	case SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED: //40: Aux. Shader (render in texture), but using Instances
		result = InitializeShadowMapBuffers(pContext, objectName, textureFile);
		break;
#endif

	case SHADER_NORMAL_BUMP:			//35
	case SHADER_NORMAL_BUMP_INSTANCED:  //99
		result = InitializeTextureNormalBumpBuffers(pContext, objectName, textureFile);
		break;

	case SHADER_Double_Color_Terrain:
	case SHADER_Slope_Texture_Terrain:
		result = InitializeTextureDouble_Color_Terrain(pContext, objectName, textureFile); // (m_driver11->m_device11, indices);
		break;
#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
	case SHADER_Terrain_Texture_DEMO60:
	case SHADER_Terrain_Texture_DEMO61:
	case SHADER_Terrain_Texture_DEMO99:
		//result = InitializeTextureHeightMapType_24_Terrain(m_driver11->m_device11, indices);
		result = InitializeTextureHeightMapType_24_Terrain(pContext, objectName, textureFile);
		break;
#endif

	default:
		WomaFatalException("WRONG SHADER!"); // Multi OS support
	}

	womalog("InitializeVkBuffers() - BEFORE SAFE_DELETE_ARRAY(indices)\n");

	SAFE_DELETE_ARRAY(indices);

	womalog("InitializeVkBuffers() - AFTER SAFE_DELETE_ARRAY(indices)\n");

#if defined USE_BOUNDING_VOLUMES
	IF_RENDER_PAGE(RENDER_PAGE >= 78) //AQUI
	{
		// Compute the model-space bounding box center
		XMFLOAT3 center;
		center.x = (minVertex.x + maxVertex.x) * 0.5f;
		center.y = (minVertex.y + maxVertex.y) * 0.5f;
		center.z = (minVertex.z + maxVertex.z) * 0.5f;

		// Store center offset (distance from (0,0,0) to the real model center)
		objectCenterOffset = XMFLOAT4(center.x, center.y, center.z, 0.0f);

		// Compute bounding sphere radius (half diagonal of the box)
		XMVECTOR size = XMVectorSubtract(XMLoadFloat3(&maxVertex), XMLoadFloat3(&minVertex));
		boundingSphere = 0.5f * XMVectorGetX(XMVector3Length(size));
	}
#endif

	return true;
}

// Static 3D geometry is uploaded once through a staging buffer and kept in
// DEVICE_LOCAL memory. This is the Vulkan equivalent of
// D3D11_USAGE_IMMUTABLE and avoids reading millions of vertices each frame
// from HOST_VISIBLE memory.
// --------------------------------------------------------------------------------------------
bool vkModelClass::CreateVKbuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile, SHADER_TYPE shaderType)
// --------------------------------------------------------------------------------------------
{
	VkResult vkResult;
	vkClass* driver = (vkClass*)m_Driver;
	ASSERT_DEBUG(driver);

	VkDevice vkDevice = driver->m_device;
	ASSERT_DEBUG(vkDevice);

	womalog("vkModelClass::CreateVKbuffers() - START %s %p m_indexBuffer: %p m_indexBufferMemory: %p\n", objectName, this, m_indexBuffer, m_indexBufferMemory);

	if (shaderType == SHADER_AUTO)
		shaderType = ModelShaderType;

	// ========================================================================
	// INDEX BUFFER
	// ========================================================================
	ASSERT_DEBUG(m_indexCount);
	indexBufferSize = sizeof(UINT) * m_indexCount;

	if (Model3D) 
	{
		if (!m_indexBuffer && !m_indexBufferMemory)
			driver->CreateAndUploadBuffer(indexBufferSize, indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, m_indexBuffer, m_indexBufferMemory);
	}
	else
	{
		//
		// 2D: [Sprites / 2D MODEL] - Store in CPU-visible shared memory (HOST_VISIBLE)
		//

		// Create Index Buffer
		VkBufferCreateInfo indexBufferInfo{};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = indexBufferSize;
		indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (!m_indexBuffer)
			VK_CHECK(vkCreateBuffer(vkDevice, &indexBufferInfo, nullptr, &m_indexBuffer));

		// Get memory requirements
		VkMemoryRequirements indexMemRequirements{};
		vkGetBufferMemoryRequirements(vkDevice, m_indexBuffer, &indexMemRequirements);

		// Allocate memory
		VkMemoryAllocateInfo indexAllocInfo{};
		indexAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		indexAllocInfo.allocationSize = indexMemRequirements.size;
		indexAllocInfo.memoryTypeIndex = driver->FindMemoryType(indexMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (!m_indexBufferMemory)
		{
			VK_CHECK(vkAllocateMemory(vkDevice, &indexAllocInfo, nullptr, &m_indexBufferMemory));

			// Bind memory to buffer
			VK_CHECK(vkBindBufferMemory(vkDevice, m_indexBuffer, m_indexBufferMemory, 0));

			// Copy WOMA index data directly into the Vulkan buffer
			void* mappedIndexMemory = nullptr;
			VK_CHECK(vkMapMemory(vkDevice, m_indexBufferMemory, 0, indexBufferSize, 0, &mappedIndexMemory));
			memcpy(mappedIndexMemory, indices, (size_t)indexBufferSize);
			vkUnmapMemory(vkDevice, m_indexBufferMemory);
		}
	}

	//
	// ========================================================================
	// VERTEX BUFFER
	// ========================================================================
	//
	vertexBufferSize = 0;
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	VkDeviceSize vertexBufferSize_4Shadows = 0;
#endif

	switch (shaderType)
	{
	case SHADER_COLOR:
		m_vertexCount = (uint32_t)modelColorVertex->size();
		vertexBufferSize = sizeof(ModelColorVertexType) * m_vertexCount;
		break;

	case SHADER_TEXTURE:		//22
	case SHADER_FIRE:
	case SHADER_USE_CURVED_REAL_SKY_PLANE:
		m_vertexCount = (uint32_t)modelTextureVertex->size();
		vertexBufferSize = sizeof(ModelTextureVertexType) * m_vertexCount;
		break;

	case SHADER_TEXTURE_WATER:
		m_vertexCount = (uint32_t)ModelTextureWatervertices.size();
		vertexBufferSize = sizeof(XMFLOAT3) * m_vertexCount;
		break;

	case SHADER_TEXTURE_LIGHT:				//23
	case SHADER_TEXTURE_LIGHT_ALPHAMAP:		//34
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW:	//36
	case SHADER_TEXTURE_LIGHT_INSTANCED:	//40
	case SHADER_TEXTURE_GS_INSTANCED:		//77
	case SHADER_TEXTURE_LIGHT_FAST:			//83
	case SHADER_TEXTURE_LIGHT98:			//98
		m_vertexCount = (uint32_t)modelTextureLightVertex->size();
		vertexBufferSize = sizeof(ModelTextureLightVertexType) * m_vertexCount;
		break;

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	case SHADER_TEXTURE_LIGHT_SAVESHADOW:
		// Shadow Pass (position only)
		m_vertexCount = (uint32_t)(modelShadowMapVertex)->size();
		vertexBufferSize_4Shadows = sizeof(ModelShadowMapVertexType) * m_vertexCount;
		break;
#endif

	case SHADER_TEXTURE_FONT:
		m_vertexCount = (uint32_t)modelTextureVertex->size();
		vertexBufferSize = sizeof(textFontVertexType) * m_vertexCount;
		break;

	case SHADER_NORMAL_BUMP:
		m_vertexCount = (uint32_t)(modelNormalBumpVertex)->size();	// Set the number of vertices in the vertex array.
		vertexBufferSize = sizeof(ModelNormalBumpVertexType) * m_vertexCount;
		break;

	case SHADER_Double_Color_Terrain:
		m_vertexCount = (uint32_t)(ModelTextureDouble_Color_TerrainTypeVertex)->size();	// Set the number of vertices in the vertex array.
		vertexBufferSize = sizeof(ModelTextureDouble_Color_TerrainType) * m_vertexCount;
		break;

	case SHADER_Terrain_Texture_DEMO60:
		m_vertexCount = (uint32_t)(modelTextureHeightMapType_24_Terrain)->size();	// Set the number of vertices in the vertex array.
		vertexBufferSize = sizeof(HeightMapType_24) * m_vertexCount;
		break;

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false); //Shader not supported yet.
		return false;
	}

	currentVertexBufferSize = vertexBufferSize;

	#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	if (shaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW)
		currentVertexBufferSize = vertexBufferSize_4Shadows;
	#endif

	ASSERT_DEBUG(m_vertexCount > 0);						// Better check, if object is empty...
	womalog("ASSERT_DEBUG(m_vertexCount > 0); ok!\n");
	ASSERT_DEBUG(currentVertexBufferSize > 0);				// Better check, if object is empty...
	womalog("ASSERT_DEBUG(currentVertexBufferSize > 0); ok!\n");

	//---------------------------------------------------------------------------------------------------------
	void* data = nullptr;

	if (Model3D)
	{
	}
	else
	{
		VkBufferCreateInfo bufferInfo{};

		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = currentVertexBufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	

		if (shaderType != SHADER_TEXTURE_LIGHT_SAVESHADOW)
		{
			// 1. Create Vertex Buffer
			if (!m_vertexBuffer)
			{
				vkResult = vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &m_vertexBuffer);
				if (vkResult != VK_SUCCESS)
				{
					WomaFatalException(("Could not create Vulkan vertex buffer."));
					return false;
				}

				// 2. Get memory requirements
				VkMemoryRequirements memRequirements{};
				vkGetBufferMemoryRequirements(vkDevice, m_vertexBuffer, &memRequirements);
		
				// 3. Allocate Vertex Buffer memory
				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = driver->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				VK_CHECK( vkAllocateMemory(vkDevice, &allocInfo, nullptr, &m_vertexBufferMemory));
		
				// 4. Bind memory to Vertex Buffer
				VK_CHECK(vkBindBufferMemory(vkDevice, m_vertexBuffer, m_vertexBufferMemory, 0));
		
				// 5. Upload WOMA vertices directly into Vulkan memory
				VK_CHECK(vkMapMemory(vkDevice, m_vertexBufferMemory, 0, currentVertexBufferSize, 0, &data));
			}
		}
		else
		{
			// 1. Create Vertex Buffer
			if (!m_vertexBuffer_4Shadows)
			{
				vkResult = vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &m_vertexBuffer_4Shadows);
				if (vkResult != VK_SUCCESS)
				{
					WomaFatalException(("Could not create Vulkan vertex buffer."));
					return false;
				}

				// 2. Get memory requirements
				VkMemoryRequirements memRequirements{};
				vkGetBufferMemoryRequirements(vkDevice, m_vertexBuffer_4Shadows, &memRequirements);

				// 3. Allocate Vertex Buffer memory
				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = driver->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				VK_CHECK(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &m_vertexBufferMemory_4Shadows));

				// 4. Bind memory to Vertex Buffer
				VK_CHECK(vkBindBufferMemory(vkDevice, m_vertexBuffer_4Shadows, m_vertexBufferMemory_4Shadows, 0));

				// 5. Upload WOMA vertices directly into Vulkan memory
				VK_CHECK(vkMapMemory(vkDevice, m_vertexBufferMemory_4Shadows, 0, currentVertexBufferSize, 0, &data));
			}
		}
	}

	const void* srcVertices = nullptr;
	switch (shaderType)
	{
	case SHADER_COLOR:
		srcVertices = modelColorVertex->data();
		break;

	case SHADER_TEXTURE:
	case SHADER_TEXTURE_FONT:
		srcVertices = modelTextureVertex->data();
		break;

	case SHADER_TEXTURE_WATER:
		srcVertices = ModelTextureWatervertices.data();
		break;

	case SHADER_TEXTURE_LIGHT:              //23
	case SHADER_TEXTURE_LIGHT_ALPHAMAP:     //34
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW: //36
	case SHADER_TEXTURE_LIGHT_INSTANCED:    //40
	case SHADER_TEXTURE_GS_INSTANCED:       //77
	case SHADER_TEXTURE_LIGHT_FAST:         //83
	case SHADER_TEXTURE_LIGHT98:            //98
		srcVertices = modelTextureLightVertex->data();
		break;

	case SHADER_NORMAL_BUMP:
		srcVertices = modelNormalBumpVertex->data();
		break;

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	case SHADER_TEXTURE_LIGHT_SAVESHADOW:
		srcVertices = modelShadowMapVertex->data();
		break;
#endif

	case SHADER_Double_Color_Terrain:
		srcVertices = ModelTextureDouble_Color_TerrainTypeVertex->data();
		break;

	case SHADER_Terrain_Texture_DEMO60:
		srcVertices = modelTextureHeightMapType_24_Terrain->data();
		break;

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false); //Shader not supported yet.
		return false;
	}
		
	if (Model3D) 
	{
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
		if (shaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW)
		{
			driver->CreateAndUploadBuffer(currentVertexBufferSize, srcVertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_vertexBuffer_4Shadows, m_vertexBufferMemory_4Shadows);
		}
		else
#endif
		{
			driver->CreateAndUploadBuffer(currentVertexBufferSize, srcVertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_vertexBuffer, m_vertexBufferMemory);
		}
	} 
	else
	{

	memcpy(data, srcVertices, (size_t)currentVertexBufferSize);

	if (shaderType != SHADER_TEXTURE_LIGHT_SAVESHADOW)
		vkUnmapMemory(vkDevice, m_vertexBufferMemory);
	else
		vkUnmapMemory(vkDevice, m_vertexBufferMemory_4Shadows);
	}


#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	if (shaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW)
		return true;
#endif

	//
	// LOAD IMAGES:
	//
	// ------------------------------------------------------------------------------------------------
	if (/*ModelShaderType*/ shaderType >= SHADER_TEXTURE)
	{
		bool result = false;

		UINT meshSRV_size = (UINT)meshSRV.size();

		if (meshSRV_size == 0 && textureFile)
		{
			for (UINT i = 0; i < textureFile->size(); i++)
			{
				// Initialize the texture object: ###: AddTexture(pixels, width, height, wrap);
				TCHAR* file = (TCHAR*)(*textureFile)[i].c_str();

				// Create the texture object for this model:
				m_Texture = GetOrCreateTexture(m_Driver, file, pContext, Model3D);// NEW GLtextureClass;
				IF_NOT_THROW_EXCEPTION(m_Texture);

#if defined USE_VIEW2D_SPRITES //LEVEL24
				if (!Model3D)
				{
					if (m_Texture)
					{
						if (SpriteTextureWidth == 0)
							SpriteTextureWidth = m_Texture->width;

						if (SpriteTextureHeight == 0)
							SpriteTextureHeight = m_Texture->height;
					}
				}
#endif
				meshSRV.push_back(m_Texture);
				m_Texture = NULL; // Ownership is now in meshSRV.
			}
		}

		womalog("m_indexBuffer created: %p\n", m_indexBuffer);
	}

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES
	if (m_instanceCount > 0)
	{
		InstanceType* instances = NULL;

		IF_NOT_RETURN_FALSE(instances = NEW InstanceType[m_instanceCount]);

		SystemHandle->m_Application->WOMA_APPLICATION_SetInstancePositions(
			SystemHandle->xml_loader.theWorldXML[m_ObjId].id,
			m_instanceCount,
			instances,
			SystemHandle->xml_loader.theWorldXML[m_ObjId].type);

		VkDeviceSize bufferSize = sizeof(InstanceType) * m_instanceCount;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &m_instanceBuffer));

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vkDevice, m_instanceBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = driver->FindMemoryType(memRequirements.memoryTypeBits,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VK_CHECK(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &m_instanceBufferMemory));

		vkBindBufferMemory(vkDevice, m_instanceBuffer, m_instanceBufferMemory, 0);

		void* data;
		vkMapMemory(vkDevice, m_instanceBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, instances, (size_t)bufferSize);
		vkUnmapMemory(vkDevice, m_instanceBufferMemory);

		SAFE_DELETE_ARRAY(instances);
	}
#endif

	womalog("vkModelClass::CreateVKbuffers() - END\n");

	return true;
}

#if defined USE_VIEW2D_SPRITES // Sprites:24
bool vkModelClass::RenderSprite(void* pContext, int positionX, int positionY, float scale, float fade)
{
	model_fade = fade;

	if (!UpdateBuffersRotY(pContext, positionX, positionY))
		return false;

	m_worldMatrix.mat4identity();
	m_worldMatrix.m[0] = scale;
	m_worldMatrix.m[5] = scale;
	m_worldMatrix.m[10]= scale;

	Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);

	return true;
}

bool vkModelClass::UpdateBuffersRotY(void* pContext, int positionX, int positionY)
{
	static int previousPosX = -10000;
	static int previousPosY = -10000;

	bool RenderfirstTime = m_driver->RenderfirstTime;

	if (((positionX == previousPosX) && (positionY == previousPosY)) && !RenderfirstTime)
		return true;

	previousPosX = positionX;
	previousPosY = positionY;

	ASSERT_DEBUG(SpriteTextureWidth);  // MANDATORY: SpriteTextureWidth >0
	ASSERT_DEBUG(SpriteTextureHeight); // MANDATORY: SpriteTextureHeight >0

	float left = (float)(WOMA::AppSettings->WINDOW_WIDTH / 2) * -1.0f + (float)positionX;
	float right = left + (float)SpriteTextureWidth;

	//LINUX & ANDROID:
	float top = (float)(WOMA::AppSettings->WINDOW_HEIGHT / 2) - (float)positionY;
	float bottom = top - (float)SpriteTextureHeight;

	/*
		------
		|t1 / |
		|  /  |
		| / t2|
		|/----|
	*/	
	static ModelTextureVertexType vertices[6];

	// First triangle (t1)
	vertices[0].x = left;
	vertices[0].y = top;
	vertices[0].z = 0;
	vertices[0].tu = 0;
	vertices[0].tv = 0;

	vertices[1].x = right;
	vertices[1].y = bottom;
	vertices[1].z = 0;
	vertices[1].tu = m_xTexture;
	vertices[1].tv = 1;

	vertices[2].x = left;
	vertices[2].y = bottom;
	vertices[2].z = 0;
	vertices[2].tu = 0;
	vertices[2].tv = 1;

	// Second triangle (t2)
	vertices[3].x = left;
	vertices[3].y = top;
	vertices[3].z = 0;
	vertices[3].tu = 0;
	vertices[3].tv = 0;

	vertices[4].x = right;
	vertices[4].y = top;
	vertices[4].z = 0;
	vertices[4].tu = m_xTexture;
	vertices[4].tv = 0;

	vertices[5].x = right;
	vertices[5].y = bottom;
	vertices[5].z = 0;
	vertices[5].tu = m_xTexture;
	vertices[5].tv = 1;

	// upload para o Vulkan vertex buffer...
	const VkDeviceSize vertexBufferSize = sizeof(ModelTextureVertexType) * 6;

	VkDevice vkDevice = m_driver->m_device;

	void* data = nullptr;

	VkResult vkResult = vkMapMemory(vkDevice, m_vertexBufferMemory, 0, vertexBufferSize, 0, &data);
	ASSERT(vkResult == VK_SUCCESS);

	memcpy(data, vertices, (size_t)vertexBufferSize);

	vkUnmapMemory(vkDevice, m_vertexBufferMemory);

	return true;
}
bool vkModelClass::UpdateSpriteBuffersRotY(void* pContext, int positionX, int positionY)
{

	return true;
}
#endif

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
void vkModelClass::RenderSky(void* pContext, UINT camera, float fadeLight)
{
	//Continuous update vars:
	m_Shader->PSfade = fadeLight;
	model_fade = fadeLight;

	// Set Initial Vars:
	if (m_Driver->RenderfirstTime)
	{
		m_Shader->isSky = true;
	}

	Render(pContext, 0, CAMERA_SKY, PROJECTION_PERSPECTIVE);
}
#endif

bool vkModelClass::LoadModel(void* pContext, TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow, bool renderShadow, UINT instanceCount)
{
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
	m_instanceCount = instanceCount;
#endif

	const TCHAR* extension = _tcsrchr(filename.c_str(), '.');

	bool res = false;
#if defined LOADW3D
	if (_tcsicmp(extension, TEXT(".w3d")) == 0 || _tcsicmp(extension, TEXT(".W3D")) == 0)
	{
		bool res = LoadW3D(pContext, shader_type, g_driver, filename, castShadow, renderShadow, instanceCount);
		if (res)
		{
			return res;
		}
		else {
			womalog("WARNING: file not found: %s", filename.c_str());
			const size_t pos = filename.find_last_of(TEXT('.'));
			if (pos != STRING::npos)
				filename.replace(pos, filename.length() - pos, TEXT(".obj"));
			else
				filename += TEXT(".obj");
		}
	}
#endif

	STRING BASEDIR = "";
	BASEDIR += filename;

	if (res == false || _tcsicmp(extension, TEXT(".obj")) == 0 || _tcsicmp(extension, TEXT(".OBJ")) == 0)
	{
		womalog("modelClass.LoadOBJ: shader_type: %d - %s\n", (UINT)shader_type, (TCHAR*)BASEDIR.c_str());
		bool b = modelClass.LoadOBJ(pContext, this, shader_type, g_driver, BASEDIR.c_str(), castShadow, renderShadow, instanceCount, 0);
		if (!b)
		{
			womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
			WomaMessageBox((TCHAR*)BASEDIR.c_str(), TEXT("Error, Could not load: ")); ASSERT(false);
		}
		if (b) {
			womalog("modelClass.CreateObject: shader_type: %d - %s\n", (UINT)shader_type, (TCHAR*)BASEDIR.c_str());
			res = modelClass.CreateObject(pContext, this, (TCHAR*)BASEDIR.c_str(), g_driver, shader_type /*SHADER_AUTO*/, filename, castShadow, renderShadow); // Auto Detect Shader Type
			womalog("modelClass.CreateObject: result: %d\n", res);
			return res;
		}
	}

	return false;
}

bool vkModelClass::LoadBump(void* pContext, TCHAR* objectName, void* driver,
	SHADER_TYPE shader_type, std::vector<STRING>* textureFile,
	std::vector<ModelNormalBumpVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	//LOADDRIVER
	m_driver = (vkClass*)driver;
	ASSERT_DEBUG(driver);

	MODEL_NAME = objectName;
	m_vertexCount = (UINT)(*model).size();

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_NORMAL_BUMP;
	else
		ModelShaderType = shader_type;

	ASSERT(ModelShaderType == SHADER_NORMAL_BUMP);

	modelNormalBumpVertex = model; //*
	indexModelList = indexList;

#if defined _DEBUG || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
	if (m_Driver->RenderfirstTime && (model && !model->empty()))
	{
		womalog("\n");
		womalog("============================================================\n");
		womalog("[BUMP DEBUG] MODEL: %s\n", objectName);
		womalog("[BUMP DEBUG] Vertices: %zu | Indices: %zu | VertexSize: %zu\n",
			model->size(),
			indexList ? indexList->size() : 0,
			sizeof(ModelNormalBumpVertexType));
		womalog("============================================================\n");

		for (size_t i = 0; i < model->size(); ++i)
		{
			const ModelNormalBumpVertexType& v = (*model)[i];

			womalog(
				"[V%zu]\n"
				"  POS     = (%+.6f, %+.6f, %+.6f)\n"
				"  UV      = (%+.6f, %+.6f)\n"
				"  NORMAL  = (%+.6f, %+.6f, %+.6f)\n"
				"  TANGENT = (%+.6f, %+.6f, %+.6f)\n"
				"  BINO    = (%+.6f, %+.6f, %+.6f)\n",
				i,
				v.x, v.y, v.z,
				v.tu, v.tv,
				v.nx, v.ny, v.nz,
				v.tx, v.ty, v.tz,
				v.bx, v.by, v.bz);
		}

		womalog("\n[BUMP DEBUG] INDEX LIST:\n");

		if (indexList)
		{
			for (size_t i = 0; i < indexList->size(); ++i)
			{
				womalog("%u%s",
					(*indexList)[i],
					((i + 1) % 3 == 0) ? "\n" : ", ");
			}
		}

		womalog("============================================================\n\n");
	}
#endif


	return InitializeVkBuffers(pContext, objectName, textureFile);
}

//Ver: 3 + LIGHT + BUMP
bool vkModelClass::InitializeTextureNormalBumpBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, textureFile));
	return true;
}

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP //&& defined USE_SCENE_MANAGER
//Ver: 1 + COLOR
bool vkModelClass::InitializeShadowMapBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, textureFile, SHADER_TEXTURE_LIGHT_SAVESHADOW));

	return true;
}
#endif

bool vkModelClass::InitializeTextureDouble_Color_Terrain(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, textureFile, SHADER_Double_Color_Terrain));

	return true;
}

bool vkModelClass::LoadTerrain(void* pContext, 
	TCHAR* objectName, void* driver, SHADER_TYPE shader_type, 
	std::vector<STRING>* textureFile, std::vector<ModelTextureDouble_Color_TerrainType>* model, 
	std::vector<UINT>* indexList, UINT instanceCount)
{
	//LOADDRIVER
	m_driver = (vkClass*)driver;
	ASSERT_DEBUG(driver);

	MODEL_NAME = objectName;
	m_vertexCount = (UINT)(*model).size();

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_Double_Color_Terrain;
	else
		ModelShaderType = shader_type;

	ASSERT(ModelShaderType == SHADER_Double_Color_Terrain || ModelShaderType == SHADER_Slope_Texture_Terrain);

	ModelTextureDouble_Color_TerrainTypeVertex = model;
	indexModelList = indexList;
	return InitializeVkBuffers(pContext, objectName, textureFile);
}


bool vkModelClass::InitializeTextureHeightMapType_24_Terrain(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile)
{
	IF_NOT_RETURN_FALSE(CreateVKbuffers(pContext, objectName, textureFile, SHADER_Terrain_Texture_DEMO60));

	return true;
}

bool vkModelClass::LoadTerrain(void* pContext,
	TCHAR* objectName, void* driver, SHADER_TYPE shader_type,
	std::vector<STRING>* textureFile, std::vector<HeightMapType_24>* model,
	std::vector<UINT>* indexList, UINT instanceCount)
{
	//LOADDRIVER
	m_driver = (vkClass*)driver;
	ASSERT_DEBUG(driver);

	MODEL_NAME = objectName;
	m_vertexCount = (UINT)(*model).size();

	ASSERT_DEBUG(shader_type != SHADER_AUTO)

	ModelShaderType = shader_type;

	ASSERT(ModelShaderType >= SHADER_Terrain_Texture_DEMO60);

	modelTextureHeightMapType_24_Terrain = model;
	indexModelList = indexList;
	return InitializeVkBuffers(pContext, objectName, textureFile);
}

////#if DX_ENGINE_LEVEL >= 97
bool vkModelClass::LoadWaterFall(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureWaterfall>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	return false;
}

bool vkModelClass::LoadSmoke(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<SmokeVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	return false;
}
void vkModelClass::SetAlpha(float a) {};
////#endif
#endif
