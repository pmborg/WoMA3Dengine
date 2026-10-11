// --------------------------------------------------------------------------------------------
// Filename: vkClass.h
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
// PURPOSE: Define OPEN GL engine interfaces
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once

#ifndef vkClass_h_
#define vkClass_h_

#include "platform.h"
#if (defined VULKAN) && DX_ENGINE_LEVEL >= 19

enum VK_SAMPLER_TYPE
{
	SAMPLER_WRAP_FALSE = 0,
	SAMPLER_WRAP_TRUE,
};

#ifdef LINUX_PLATFORM
#define FORCE_LINUX_VULKAN_DEBUG
#endif

#if defined _DEBUG && defined WINDOWS_PLATFORM
#define FORCE_WINDOWS_VULKAN_DEBUG
#endif

#if (defined FORCE_WINDOWS_VULKAN_DEBUG || defined FORCE_LINUX_VULKAN_DEBUG) && !defined ANDROID_PLATFORM
static constexpr bool ENABLE_DEBUGGING = true;
#else
static constexpr bool ENABLE_DEBUGGING = false;
#endif

static constexpr const char* DEBUG_LAYER = "VK_LAYER_KHRONOS_validation";

//////////////
// INCLUDES //
//////////////
#if true //defined WINDOWS_PLATFORM

#include "womadriverclass.h"	//woma

#include "GLmathClass.h" //WOMA Math Class

#if defined WINDOWS_PLATFORM
	#define VK_USE_PLATFORM_WIN32_KHR
#endif
#ifdef LINUX_PLATFORM
	#define VK_USE_PLATFORM_XLIB_KHR 
#endif
#if defined ANDROID_PLATFORM
	#define VK_USE_PLATFORM_ANDROID_KHR
#endif
#include <vulkan/vulkan.h>

#pragma comment( lib, "vulkan-1.lib" )

#ifndef VK_API_VERSION_MAJOR
	#define VK_API_VERSION_MAJOR(version) (((uint32_t)(version)) >> 22)
	#define VK_API_VERSION_MINOR(version) ((((uint32_t)(version)) >> 12) & 0x3FF)
	#define VK_API_VERSION_PATCH(version) (((uint32_t)(version)) & 0xFFF)
#endif

#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 // Color Shader
#include "GLcameraClass.h"
#endif


//--------------------------------
static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

struct VKwindowDataContainer
{
	// ---------------------------------------------------------
	// Surface
	// ---------------------------------------------------------
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	// ---------------------------------------------------------
	// Surface Capabilities
	// ---------------------------------------------------------
	VkSurfaceCapabilitiesKHR surfCapabilities{};
	VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	// ---------------------------------------------------------
	// SwapChain
	// ---------------------------------------------------------
	VkSwapchainKHR m_swap_chain = VK_NULL_HANDLE;
	VkSurfaceFormatKHR m_swapchainColorFormat{};
	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	uint32_t m_currentImageIndex = 0;   // Swapchain Image Index
	uint32_t m_currentFrame = 0;        // Frame-In-Flight

#ifdef VULKAN1_1
	// Associa cada imagem da Swapchain à Fence que a está a usar.
	std::vector<VkFence> imagesInFlight;
#endif

	// ---------------------------------------------------------
	// Viewport / Scissor (Window dependent)
	// ---------------------------------------------------------
	VkViewport viewport{};
	VkRect2D scissor{};
	VkPipelineViewportStateCreateInfo viewportCreateInfo{};

	// ---------------------------------------------------------
	// SwapChain Images
	// ---------------------------------------------------------
	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainImageViews;

	// ---------------------------------------------------------
	// Depth Buffer
	// ---------------------------------------------------------
	VkImage depthImage = VK_NULL_HANDLE;
	VkDeviceMemory depthMemory = VK_NULL_HANDLE;
	VkImageView depthView = VK_NULL_HANDLE;

	// ---------------------------------------------------------
	// RenderPass
	// ---------------------------------------------------------
	VkRenderPass m_render_pass = VK_NULL_HANDLE;

	// ---------------------------------------------------------
	// Framebuffers
	// ---------------------------------------------------------
	std::vector<VkFramebuffer> m_framebuffers;

	// ---------------------------------------------------------
	// Command Buffers
	// ---------------------------------------------------------
	std::vector<VkCommandBuffer> m_commandBuffers;

	// ---------------------------------------------------------
	// Synchronization
	// ---------------------------------------------------------
	// Rotação por Frame-In-Flight (0..MAX_FRAMES_IN_FLIGHT-1)
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<VkSemaphore> renderFinishedSemaphores; // Um por imagem da Swapchain

	//
	//
	//
	VkQueue presentQueue;
};

#define VK_CHECK(A) { ASSERT(A == VK_SUCCESS); }

////////////////////////////////////////////////////////////////////////////////
// Class name: vkClass
////////////////////////////////////////////////////////////////////////////////
class vkClass : public WomaDriverClass
{
public:
	vkClass();
	~vkClass();
	void WaitForIdle();
	void Shutdown2D();
	void Shutdown() override;

	void UpdateViewportScissorState(UINT monitor);
	void RenderDriverText(void* pContext);
	void addText(int Xpos, int Ypos, TCHAR* printText, float R, float G, float B);

	void SetCamera2D();
	void Initialize2Dand3DCamera();
	void setProjectionMatrixWorldMatrixOrthoMatrix(int screenWidth, int screenHeight, float screenNear, float screenDepth);
	bool Initialize(float* clearColor, void* pContext=NULL);
	void FinalizeInitialization();



	BOOL CheckAPIdriver(UINT adaptor);
    bool OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
        float screenDepth, float screenNear, BOOL msaa, bool vsync,
        BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);

	bool CreateInstance();
	bool PickPhysicalDevice();
	bool FindQueueFamilies(UINT monitor);
	bool CreateLogicalDevice();
	bool CreateSurface(UINT monitor);

	bool Resize(UINT USE_MONITOR, int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits);
	bool create_or_resize_swap(UINT USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen);

	bool CreateSwapChain(UINT monitor);
	bool CreateSwapChainImageViews(UINT monitor);
	void DestroySwapChainResources(UINT monitor);

	bool CreateRenderPass(UINT monitor);
	bool memory_type_from_properties(uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);
	bool CreateDepthResources(UINT monitor);
	bool CreateFramebuffers(UINT monitor);

	bool CreateCommandPool();
	bool CreateCommandBuffers(UINT monitor);
	bool CreateSyncObjects(UINT monitor);

	void BeginScene(UINT monitorWindow);
	void ClearDepthBuffer(void* pContext);
	void ResetResource(void* pContext, UINT monitorWindow);
	void EndScene(UINT monitorWindow);

	void SetRasterizerState(void* pContext, UINT cullMode, UINT fillMode);

	#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
	//We have two new functions for turning on and off alpha blending:
	void TurnOnAlphaBlending(void* pContext);
	void TurnOffAlphaBlending(void* pContext);
	#endif

	void GetWorldMatrix(float*);
	void GetProjectionMatrix(float*);
	void GetVideoCardInfo(char*);
	void BuildOrthoMatrix(mat4* matrix, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool leftHand);

	// Vulkan API version supported by the selected GPU
	UINT VulkanApiMajor = 0;
	UINT VulkanApiMinor = 0;
	UINT VulkanApiPatch = 0;

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	ImageLoaderClass* CaptureScreenShot(int screenWidth, int screenHeight);
#endif	 

	mat4 GetViewMatrix(UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);
	mat4 GetProjectionMatrix(UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);

	VkSampler      m_vkSampler[2] = {};


	// ---------------------------------------------------------
	// VARs
	// ---------------------------------------------------------
	VkSampleCountFlagBits GetMSAASampleCount();
	VkFormat FindDepthFormat();

	mat4 m_projectionMatrix;
	mat4 m_orthoMatrix;
#ifdef USE_SKY_CAMERA_DOME
	mat4 m_projectionMatrix_sky;
#endif

	VkDevice      m_device = VK_NULL_HANDLE;				//Equivalent to: ID3D11Device

	//UINT m_frameIndex=0;
	std::vector<VKwindowDataContainer> VKwindowsToRenderArray;

private:
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	int USE_THIS_ADAPTER_CARD=-1;
	GLmathClass* vkmathClass = NULL;

	// ---------------------------------------------------------
	// Instance
	// ---------------------------------------------------------
	VkInstance       m_instance = VK_NULL_HANDLE;

	// ---------------------------------------------------------
	// Device
	// ---------------------------------------------------------
	VkSampleCountFlagBits msaaSamples;

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;		//Equivalent to: IDXGIAdapter

	VkPhysicalDeviceMemoryProperties m_memoryProperties{};
	VkPhysicalDeviceFeatures         m_deviceFeatures{};
	VkPhysicalDeviceProperties       m_deviceProperties{};

	//GLOBAL:
	uint32_t m_graphicsQueueFamilyIndex = UINT32_MAX;
	uint32_t m_present_queue_family_index = UINT32_MAX;

	// ---------------------------------------------------------
	// Queues
	// ---------------------------------------------------------
	VkQueue m_graphicsQueue = VK_NULL_HANDLE;				//Equivalent to: ID3D11DeviceContext

	VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};

	VkClearValue m_clearColor;
	VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

	// ---------------------------------------------------------
	// Command System
	// ---------------------------------------------------------
	VkCommandPool m_commandPool = VK_NULL_HANDLE;

	//--------------------------------
	char m_videoCardDescription[128];

	VkApplicationInfo appInfo{};
	VkInstanceCreateInfo createInfo{};
	VkDeviceQueueCreateInfo queueCreateInfo{};
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	VkSwapchainCreateInfoKHR swapchain_ci{};
	VkImageViewCreateInfo imageViewCI = {};
	VkAttachmentDescription colorAttachment{};
	VkAttachmentDescription depthAttachment{};
	VkAttachmentReference color_reference = {};
	VkAttachmentReference depth_reference = {};
	VkSubpassDescription subpass = {};
	VkSubpassDependency subpass_dependency = {};
	VkRenderPassCreateInfo rp_info = {};
	VkImageCreateInfo image_info = {};
	VkMemoryAllocateInfo mem_alloc = {};
	VkImageViewCreateInfo view_info = {};
	VkFramebufferCreateInfo fb_info{};
	VkCommandPoolCreateInfo cmd_pool_info = {};
	VkCommandBufferAllocateInfo cmd{};
	VkSemaphoreCreateInfo semaphoreInfo{};
	VkFenceCreateInfo fenceInfo{};
	VkCommandBufferBeginInfo beginInfo{};
	VkRenderPassBeginInfo rp_begin{};
	VkClearAttachment clearAttachment{};
	VkClearRect clearRect{};
	VkSubmitInfo submitInfo{};
	VkPresentInfoKHR presentInfo{};
	VkCommandBufferAllocateInfo cmdBufAllocateInfo{};



};

#endif
#endif
#endif
