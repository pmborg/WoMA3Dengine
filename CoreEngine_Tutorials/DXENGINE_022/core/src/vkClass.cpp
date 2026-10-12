// ----------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------
// Filename: vkClass.cpp
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
//
// PURPOSE: 
//
//WomaDriverClass
//	vkClass
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

// -----------------------------------------------------------------------------
// WoMA Vulkan Driver Architecture (Level 23+)
// -----------------------------------------------------------------------------
//
//          Application / Scene
//                 │
//                 ▼
//         Camera + World Matrix
//                 │
//                 ▼
//      World / View / Projection
//                 │
//                 ▼
//            vkShaderClass
//        ┌───────────────────────┐
//        │ m_uniformBufferVS     │
//        │ m_uniformBufferPS     │
//        │ DescriptorSetLayout   │
//        │ DescriptorPool        │
//        └──────────┬────────────┘
//                   │
//                   ▼
//             Descriptor Set
//                   │
//                   ▼
//         Graphics Pipeline
//                   │
//                   ▼
//             Command Buffer
//                   │
//                   ▼
//          vkQueueSubmit()
//                   │
//                   ▼
//          vkQueuePresentKHR()
//
// Synchronization:
//   imageAvailableSemaphore
//           │
//   imagesInFlight[]
//           │
//   inFlightFences[]
//           │
//     currentFrame
//
// -----------------------------------------------------------------------------
// NOTE:
// World/View/Projection matrices are native WoMA structures.
// The Vulkan backend consumes existing WVP data; it does not generate it.
// -----------------------------------------------------------------------------

#include "platform.h"
#if (defined VULKAN) && DX_ENGINE_LEVEL >= 19 
#pragma warning(push)
#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
#include "mem_leak.h"
#include "stateMachine.h"
#include "womadriverclass.h"
#include "OSengine.h"
#include "GLmathClass.h"
#include "vkClass.h"

#include "glTextureclass.h"

#ifndef ANDROID_PLATFORM
VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	womalog("DebugCallback()::VULKAN VALIDATION: %s\n", pCallbackData->pMessage);

	return VK_FALSE;
}
#endif

vkClass::vkClass()
{
	CLASSLOADER();
	
	_tcscpy_s(driverName, TEXT("VULKAN")); // driverName = TEXT ("VULKAN");
	_tcscpy_s(szShaderModel, TEXT(""));

	gl_Camera = NULL;
}

vkClass::~vkClass() 
{ 
	SAFE_DELETE(vkmathClass);
	Shutdown(); 
	CLASSDELETE(); 
}

void vkClass::WaitForIdle()
{
	if (m_device)
	{
		vkDeviceWaitIdle(m_device);
		womalog("vkClass::WaitForIdle():: Vulkan device idle\n");
	}
}

// ------------------------------------------------------------------
void vkClass::UpdateViewportScissorState(UINT monitor)
// ------------------------------------------------------------------
{
	auto& vkWindow = ((vkClass*)m_Driver)->VKwindowsToRenderArray[monitor];

	// Describe viewport and scissor
	//VkViewport viewport = {};
	vkWindow.viewport.x = 0.0f;
	vkWindow.viewport.y = 0.0f;
	vkWindow.viewport.width = (float)vkWindow.surfCapabilities.currentExtent.width;
	vkWindow.viewport.height = (float)vkWindow.surfCapabilities.currentExtent.height;
	vkWindow.viewport.minDepth = 0.0f;
	vkWindow.viewport.maxDepth = 1.0f;

	//VkRect2D scissor = {};
	vkWindow.scissor.offset.x = 0;
	vkWindow.scissor.offset.y = 0;
	vkWindow.scissor.extent.width = vkWindow.surfCapabilities.currentExtent.width;
	vkWindow.scissor.extent.height = vkWindow.surfCapabilities.currentExtent.height;

	// Note: scissor test is always enabled (although dynamic scissor is possible)
	// Number of viewports must match number of scissors
	//VkPipelineViewportStateCreateInfo viewportCreateInfo = {};
	vkWindow.viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vkWindow.viewportCreateInfo.viewportCount = 1;
	vkWindow.viewportCreateInfo.pViewports = &vkWindow.viewport;
	vkWindow.viewportCreateInfo.scissorCount = 1;
	vkWindow.viewportCreateInfo.pScissors = &vkWindow.scissor;

	//womalog("vkClass::UpdateViewportScissorState()::Viewport: %.0f x %.0f\n", vkWindow.viewport.width, vkWindow.viewport.height);
	//womalog("vkClass::UpdateViewportScissorState()::Scissor: %u x %u\n", vkWindow.scissor.extent.width, vkWindow.scissor.extent.height);
}

void vkClass::FinalizeInitialization()
{
	womalog("vkClass::FinalizeInitialization():: Start\n");

	SubmitLoadContext();

	womalog("vkClass::FinalizeInitialization():: Vulkan renderer ready\n");
}

void vkClass::Shutdown2D() {}

void vkClass::Shutdown()
{
	womalog("vkClass::Shutdown():: START\n");
	Shutdown2D();

	SAFE_DELETE(gl_Camera);

#if defined USE_FRUSTUM
	SAFE_DELETE (frustum);
#endif

	if (m_device)
	{
		vkDeviceWaitIdle(m_device);
		womalog("vkClass::Shutdown::Vulkan device idle\n");

		// Texture cache owns the GLtextureClass objects.
		for (UINT i = 0; i < g_allTexturePointerArray.size(); i++)
		{
			GLtextureClass* tex = (GLtextureClass*)g_allTexturePointerArray[i];
			SAFE_DELETE(tex);
			g_allTexturePointerArray[i] = NULL;
		}

		g_allTexturePointerArray.clear();
		g_allTextureNameArray.clear();
	}

	for (UINT m = 0; m < VKwindowsToRenderArray.size(); m++)
	{
		DestroySwapChainResources(m);

		auto& vkWindow = VKwindowsToRenderArray[m];

		if (vkWindow.m_render_pass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(m_device, vkWindow.m_render_pass, nullptr);
			vkWindow.m_render_pass = VK_NULL_HANDLE;
		}
	}
	if (m_device)
	{
		for (size_t i = 0; i < 2; i++)
		{
			if (m_vkSampler[i] != VK_NULL_HANDLE)
				vkDestroySampler(m_device, m_vkSampler[i], nullptr);
			m_vkSampler[i] = VK_NULL_HANDLE;
		}
	
	}

	for (UINT m = 0; m < VKwindowsToRenderArray.size(); m++)
	{
		// ---------------------------------------------------------
		// Destroy Synchronization objects
		// ---------------------------------------------------------

		// imageAvailableSemaphores (Frames-In-Flight)
		if (!VKwindowsToRenderArray[m].imageAvailableSemaphores.empty())
		{
			for (VkSemaphore semaphore :
			VKwindowsToRenderArray[m].imageAvailableSemaphores)
			{
				if (semaphore)
					vkDestroySemaphore(m_device, semaphore, nullptr);
			}

			VKwindowsToRenderArray[m].imageAvailableSemaphores.clear();
			VKwindowsToRenderArray[m].imageAvailableSemaphores.shrink_to_fit();
		}

		// renderFinishedSemaphores (SwapChain Images)
		if (!VKwindowsToRenderArray[m].renderFinishedSemaphores.empty())
		{
			for (VkSemaphore semaphore :
			VKwindowsToRenderArray[m].renderFinishedSemaphores)
			{
				if (semaphore)
					vkDestroySemaphore(m_device, semaphore, nullptr);
			}

			VKwindowsToRenderArray[m].renderFinishedSemaphores.clear();
			VKwindowsToRenderArray[m].renderFinishedSemaphores.shrink_to_fit();
		}

		// inFlightFences (Frames-In-Flight)
		if (!VKwindowsToRenderArray[m].inFlightFences.empty())
		{
			for (VkFence fence :
			VKwindowsToRenderArray[m].inFlightFences)
			{
				if (fence)
					vkDestroyFence(m_device, fence, nullptr);
			}

			VKwindowsToRenderArray[m].inFlightFences.clear();
			VKwindowsToRenderArray[m].inFlightFences.shrink_to_fit();
		}
	}
	womalog("vkClass::Shutdown::Destroying Synchronization objects\n");

	if (m_commandPool)
	{
		vkDestroyCommandPool(m_device, m_commandPool, nullptr);
		m_commandPool = VK_NULL_HANDLE;
		womalog("vkClass::Shutdown::Destroying CommandPool\n");
	}

	if (m_device)
	{
		vkDestroyDevice(m_device, nullptr);
		m_device = VK_NULL_HANDLE;
		womalog("vkClass::Shutdown::Destroying Device\n");
	}

	for (UINT i = 0; i < VKwindowsToRenderArray.size(); i++)
	{
		auto& vkWindow = VKwindowsToRenderArray[i];

		if (vkWindow.m_surface)
		{
			vkDestroySurfaceKHR(m_instance, vkWindow.m_surface, nullptr);
			vkWindow.m_surface = VK_NULL_HANDLE;
			womalog("vkClass::Shutdown::Destroying Surface\n");
		}
	}

#ifndef ANDROID_PLATFORM
	if (m_debugMessenger)
	{
		auto DestroyDebugUtilsMessenger =
			(PFN_vkDestroyDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(
				m_instance,
				"vkDestroyDebugUtilsMessengerEXT");

		if (DestroyDebugUtilsMessenger)
		{
			DestroyDebugUtilsMessenger(
				m_instance,
				m_debugMessenger,
				nullptr);
		}

		m_debugMessenger = VK_NULL_HANDLE;
	}
#endif

	if (m_instance)
	{
		vkDestroyInstance(m_instance, nullptr);
		m_instance = VK_NULL_HANDLE;
		womalog("vkClass::Shutdown::Destroying Instance\n");
	}
}

BOOL vkClass::CheckAPIdriver(UINT adaptor)
{
#if defined(WINDOWS_PLATFORM)
	USE_THIS_ADAPTER_CARD = adaptor;
#else
	USE_THIS_ADAPTER_CARD = 0;
#endif
	return TRUE;
}

bool vkClass::Resize(UINT USE_MONITOR, int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits)
{
	if (WOMA::game_state >= GAME_STOP)
		return false;

	RenderfirstTime = true;	 // Used on SPRITES!

	// [0] DX11: create_or_resize_swap
	IF_NOT_RETURN_FALSE(create_or_resize_swap(USE_MONITOR, screenWidth, screenHeight, fullscreen));

	// [1] DX11: CreateRenderTargetView
	IF_NOT_RETURN_FALSE(CreateSwapChainImageViews(USE_MONITOR));

	// Get MSAA Samples:
	msaaSamples = GetMSAASampleCount();

	// [2] DX11: createDepthStencil
	IF_NOT_RETURN_FALSE(CreateDepthResources(USE_MONITOR));

	// [3] DX11: setViewportDevice
	UpdateViewportScissorState(USE_MONITOR);

	// [4] DX11: createSetDepthStencilView
	IF_NOT_RETURN_FALSE(CreateRenderPass(USE_MONITOR));

	// [5] DX11: SetBackBufferRenderTarget
	IF_NOT_RETURN_FALSE(CreateFramebuffers(USE_MONITOR));

#if defined _DEBUG
	auto& vkWindow = ((vkClass*)m_Driver)->VKwindowsToRenderArray[USE_MONITOR];

	womalog(
		"VULKAN EXTENT: %u x %u | WOMA WINDOW: %u x %u\n",
		vkWindow.surfCapabilities.currentExtent.width,  /*vkWindow.m_swapChainExtent.width*/
		vkWindow.surfCapabilities.currentExtent.height, /*vkWindow.m_swapChainExtent.height*/
		WOMA::AppSettings->WINDOW_WIDTH,
		WOMA::AppSettings->WINDOW_HEIGHT);
#endif

	//[6]: DX11: setProjectionMatrixWorldMatrixOrthoMatrix
	setProjectionMatrixWorldMatrixOrthoMatrix(screenWidth, screenHeight, screenNear, screenDepth);

	// [7] DX11: Initialize2Dand3DCamera
#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES
	Initialize2Dand3DCamera();
#endif

	return true;
}

bool vkClass::OnInit(int _USE_MONITOR, /*HWND*/void* hwnd, int screenWidth, int screenHeight, UINT depthBits, float screenDepth, float screenNear, 
								BOOL msaa, bool vsync, BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize)  
{ 
	m_VSYNC_ENABLED = vsync;

	womalog("-------------------------\n");
	womalogauto(TEXT("vkClass::OnInit::INITIALIZING DRIVER: %s\n"), driverName);
	womalog("-------------------------\n");

	VKwindowsToRenderArray.resize(WOMA::windowsToRenderArray.size());

	womalog("vkClass::OnInit::VKwindowsToRenderArray.size() = %d\n", VKwindowsToRenderArray.size());
	womalog("vkClass::OnInit::windowsToRenderArray.size() = %d\n", WOMA::windowsToRenderArray.size());

	ASSERT_DEBUG(WOMA::windowsToRenderArray.size() > 0); 

	// -----------------------------------------------------------------
	// Select GPU
	// -----------------------------------------------------------------
	ASSERT(CheckAPIdriver(WOMA::AppSettings->ADAPTOR));
	IF_NOT_RETURN_FALSE(CreateInstance());
#if DX_ENGINE_LEVEL >= 21 && !defined ANDROID_PLATFORM
	if (ENABLE_DEBUGGING)
		CreateDebugCallback();
#endif
	IF_NOT_RETURN_FALSE(PickPhysicalDevice());

	// ---------------------------------------------------------
	// Select Depth Format (once per device)
	// ---------------------------------------------------------
	m_depthFormat = FindDepthFormat();
	ASSERT(m_depthFormat != VK_FORMAT_UNDEFINED);

	womalog("vkClass::OnInit::Depth Format: %d\n", m_depthFormat);

	// -----------------------------------------------------------------
	// Initialize Vulkan
	// -----------------------------------------------------------------
	IF_NOT_RETURN_FALSE(CreateSurface(_USE_MONITOR));
	IF_NOT_RETURN_FALSE(FindQueueFamilies(_USE_MONITOR));

	IF_NOT_RETURN_FALSE(CreateLogicalDevice());

	//Init Step: 6 Before Resize (SetCamera2D & SetCamera3D)!
	Initialize2Dand3DCamera();

	ASSERT(Resize(_USE_MONITOR, screenWidth, screenHeight, screenNear, screenDepth, fullscreen, depthBits));

	IF_NOT_RETURN_FALSE(CreateCommandPool());

	for (UINT m = 0; m < WOMA::windowsToRenderArray.size(); m++)
	{
		IF_NOT_RETURN_FALSE(CreateCommandBuffers(m));
		IF_NOT_RETURN_FALSE(CreateSyncObjects(m));
	}

#if defined USE_FRUSTUM
	if (frustum)
		SAFE_DELETE(frustum);
	frustum = NEW FrustumClass;	// Create Frustum
#endif

	womalog("vkClass::OnInit:: Vulkan initialization completed successfully.\n");

	return true; 
}

bool vkClass::CreateInstance()
{
	womalog("vkClass::CreateInstance()\n");

	//
	// VULKAN APPLICATION INFO
	//
	//VkApplicationInfo appInfo{};						 
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = WOMA::APP_FULLNAME;
	appInfo.applicationVersion = VK_MAKE_VERSION(WOMA_BUILD_YEAR, WOMA_BUILD_MONTH, WOMA_BUILD_DAY);
	appInfo.pEngineName = PROJECT_NAME;
	appInfo.engineVersion = VK_MAKE_VERSION(WOMA_BUILD_YEAR, WOMA_BUILD_MONTH, WOMA_BUILD_DAY);

#if defined WINDOWS_PLATFORM
	appInfo.apiVersion = VK_API_VERSION_1_4;
#elif defined LINUX_PLATFORM
	appInfo.apiVersion = VK_API_VERSION_1_3;
#elif defined ANDROID_PLATFORM
	appInfo.apiVersion = VK_API_VERSION_1_0;
#endif


	//
	// get extensionCount
	//
	UINT extensionCount = 0;
	VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	ASSERT_DEBUG(result == VK_SUCCESS);

	//
	// get those extensionNames
	//
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
	ASSERT_DEBUG(result == VK_SUCCESS);

	for (const auto& extension : availableExtensions)
		womalog("vkClass::CreateInstance()::Available Extension: %s\n", extension.extensionName);


	//
	// INFO
	//
	//VkInstanceCreateInfo createInfo{};							   
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	if (!ENABLE_DEBUGGING)
	{
		// Declarar as extensões básicas universais
		static const char* extensions[] =
		{
			VK_KHR_SURFACE_EXTENSION_NAME,

	#ifdef WINDOWS_PLATFORM
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	#endif

	#ifdef LINUX_PLATFORM
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME, // Ativado para bater certo com o teu XOpenDisplay/XCreateWindow
	#endif

	#ifdef ANDROID_PLATFORM
			VK_KHR_ANDROID_SURFACE_EXTENSION_NAME, // Garante que também funciona em Android futuramente
	#endif
		};

		// Calcula dinamicamente o número exato de extensões no array graças às macros
		createInfo.enabledExtensionCount = sizeof(extensions) / sizeof(extensions[0]);

		createInfo.ppEnabledExtensionNames = extensions;

		for (UINT i = 0; i < (sizeof(extensions) / sizeof(extensions[0])); i++)
			womalog("vkClass::CreateInstance()::Extension[%u] = %s\n", i, extensions[i]);
	}
	else
	{
		static const char* extensions[] =
		{
		#ifdef WINDOWS_PLATFORM
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		#endif

		#ifdef LINUX_PLATFORM
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
		#endif

		#if defined FORCE_LINUX_VULKAN_DEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		#endif

		#ifdef ANDROID_PLATFORM
			VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
		#endif

		#ifdef WINDOWS_PLATFORM
			VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
		#endif

			VK_KHR_SURFACE_EXTENSION_NAME
		};

		createInfo.enabledExtensionCount = static_cast<UINT>(sizeof(extensions) / sizeof(extensions[0]));

		createInfo.ppEnabledExtensionNames = extensions;

		for (UINT i = 0; i < (sizeof(extensions) / sizeof(extensions[0])); i++)
			womalog("vkClass::CreateInstance()::Extension[%u] = %s\n", i, extensions[i]);
	}

	//
	// ENABLE_DEBUGGING
	//
	if (ENABLE_DEBUGGING)
	{
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = &DEBUG_LAYER;
	}

	UINT layerCount = 0;
	result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	ASSERT_DEBUG(result == VK_SUCCESS);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	ASSERT_DEBUG(result == VK_SUCCESS);

	for (const auto& layer : availableLayers)
		womalog("vkClass::CreateInstance()::Available Layer: %s\n", layer.layerName);

#ifndef ANDROID_PLATFORM
	//
	// ENABLE SHADER PRINTF
	//

	VkValidationFeatureEnableEXT enabledValidationFeatures[] =
	{
		VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT
	};

	VkValidationFeaturesEXT validationFeatures{};										  
	validationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;

	validationFeatures.enabledValidationFeatureCount =
		static_cast<UINT>(
			sizeof(enabledValidationFeatures) /
			sizeof(enabledValidationFeatures[0]));

	validationFeatures.pEnabledValidationFeatures = enabledValidationFeatures;

	createInfo.pNext = &validationFeatures;
#endif

	//
	// VULKAN CREATE INSTANCE
	//
	result = vkCreateInstance(&createInfo, nullptr, &m_instance);
	if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
		womalog("vkClass::CreateInstance()::cannot find a compatible Vulkan ICD\n");
		return false;
	}
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateInstance() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateInstance() Vulkan Instance Created (%d extensions enabled)\n", createInfo.enabledExtensionCount);



	return true;
}



bool vkClass::PickPhysicalDevice()
{
	UINT deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		womalog("vkClass::PickPhysicalDevice No Vulkan compatible GPU found!\n");
		return false;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
	womalog("vkClass::PickPhysicalDeviceFound %u Vulkan GPU(s)\n", deviceCount);

	VkPhysicalDeviceProperties props;
	for (size_t i = 0; i < deviceCount; i++)
	{
		vkGetPhysicalDeviceProperties(devices[i], &props);
		womalog("vkClass::PickPhysicalDeviceGPU[%d]: %s\n", i, props.deviceName);
	}
	
	if (USE_THIS_ADAPTER_CARD < 0 || USE_THIS_ADAPTER_CARD >= (int)deviceCount)
	{
		womalog("vkClass::PickPhysicalDeviceWARNING: Invalid Vulkan adapter %d. Using adapter 0.\n", USE_THIS_ADAPTER_CARD);
		USE_THIS_ADAPTER_CARD = 0;
	}

	m_physicalDevice = devices[USE_THIS_ADAPTER_CARD];
	ASSERT_DEBUG(m_physicalDevice);
	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);

	womalog("------------------------------------------------------------\n");
	womalog("vkClass::PickPhysicalDevice Selected Vulkan Physical Device\n");
	womalog("------------------------------------------------------------\n");
	womalog("vkClass::PickPhysicalDevice Adapter        : %d\n", USE_THIS_ADAPTER_CARD);
	womalog("vkClass::PickPhysicalDevice GPU            : %s\n", m_deviceProperties.deviceName);

	VulkanApiMajor = VK_API_VERSION_MAJOR(m_deviceProperties.apiVersion);
	VulkanApiMinor = VK_API_VERSION_MINOR(m_deviceProperties.apiVersion);
	VulkanApiPatch = VK_API_VERSION_PATCH(m_deviceProperties.apiVersion);

	StringCchPrintf(szShaderModel, MAX_STR_LEN, TEXT("VK%u.%u"), VulkanApiMajor, VulkanApiMinor);

	womalog("vkClass::PickPhysicalDevice API Version    : %u.%u.%u\n", VulkanApiMajor, VulkanApiMinor, VulkanApiPatch);

	womalog("vkClass::PickPhysicalDevice Driver Version : %u\n", m_deviceProperties.driverVersion);
	womalog("------------------------------------------------------------\n");

	vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);

	return true;
}

bool vkClass::FindQueueFamilies(UINT monitor)
{
	auto& vkWindow = VKwindowsToRenderArray[monitor];

	UINT queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (UINT i = 0; i < queueFamilyCount; i++)
	{
		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, vkWindow.m_surface, &presentSupport);

		const VkQueueFamilyProperties& q = queueFamilies[i];

		womalog("vkClass::FindQueueFamilies QueueFamily[%u]\n", i);
		womalog("vkClass::FindQueueFamilies     queueCount : %u\n", q.queueCount);
		womalog("vkClass::FindQueueFamilies     queueFlags : 0x%X\n", q.queueFlags);

		if (q.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			womalog("    GRAPHICS\n");

		if (q.queueFlags & VK_QUEUE_COMPUTE_BIT)
			womalog("    COMPUTE\n");

		if (q.queueFlags & VK_QUEUE_TRANSFER_BIT)
			womalog("    TRANSFER\n");

		if (q.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			womalog("    SPARSE_BINDING\n");

		if (m_graphicsQueueFamilyIndex == UINT32_MAX && q.queueCount > 0 && (q.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			m_graphicsQueueFamilyIndex = i;
			if (presentSupport)
				m_present_queue_family_index = i;
		}
	}

	ASSERT(m_graphicsQueueFamilyIndex != UINT32_MAX);
	ASSERT(m_present_queue_family_index != UINT32_MAX);

	womalog("vkClass::FindQueueFamilies Found Graphics Queue Family Index: %u\n", m_graphicsQueueFamilyIndex);

	return true;
}

#include <set> // Necessário para filtrar índices duplicados

bool vkClass::CreateLogicalDevice()
{
	float queuePriority[1] = { 1.0f }; // 1 = MAX Priority

	// 1. Recolher todos os índices únicos de filas (Gráficos + Apresentação de todos os monitores)
	std::set<UINT> uniqueQueueFamilies;

	// Adicionar a fila de gráficos principal
	uniqueQueueFamilies.insert(m_graphicsQueueFamilyIndex);

	// Adicionar as filas de apresentação de todos os monitores registados
	// Nota: Se mantiveres a lógica antiga baseada em variáveis globais, podes usar apenas: uniqueQueueFamilies.insert(m_present_queue_family_index);
	// Mas o ideal para multi-monitor real é percorrer o teu array de janelas:
	for (size_t i = 0; i < VKwindowsToRenderArray.size(); i++)
	{
		// Se usares a minha sugestão anterior de guardar por monitor:
		// uniqueQueueFamilies.insert(VKwindowsToRenderArray[i].presentQueueFamilyIndex);

		// Se ainda usas a variável global por agora:
		uniqueQueueFamilies.insert(m_present_queue_family_index);
	}

	// 2. Criar a estrutura para cada fila única encontrada
	//std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (UINT queueFamily : uniqueQueueFamilies)
	{
		//VkDeviceQueueCreateInfo queueCreateInfo{};									
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<UINT>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &m_deviceFeatures;

	const char* deviceExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	createInfo.enabledExtensionCount = 1;
	createInfo.ppEnabledExtensionNames = deviceExtensions;

	VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device);
	ASSERT_DEBUG(m_device);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateLogicalDevice failed (%d)\n", result);
		return false;
	}

	// 3. Obter o Handle para a fila de Gráficos Principal
	vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
	womalog("vkClass::CreateLogicalDevice Vulkan: Graphics Queue Ready\n");

	// 4. Obter os Handles de Apresentação para cada janela/monitor
	for (size_t i = 0; i < VKwindowsToRenderArray.size(); i++)
	{
		// Se guardares as queues por monitor na estrutura (Recomendado):
		// vkGetDeviceQueue(m_device, VKwindowsToRenderArray[i].presentQueueFamilyIndex, 0, &VKwindowsToRenderArray[i].m_presentQueue);

		// Se usares a variável global (Apenas para o monitor principal funcionar por agora):
		if (i == 0) {
			vkGetDeviceQueue(m_device, m_present_queue_family_index, 0, &VKwindowsToRenderArray[i].presentQueue);
			womalog("vkClass::CreateLogicalDevice Vulkan: Present Queue Ready\n");
		}

		womalog(
			"vkClass::CreateLogicalDevice Vulkan Queues: graphicsFamily=%u presentFamily=%u\n",
			m_graphicsQueueFamilyIndex,
			m_present_queue_family_index);

		womalog(
			"vkClass::CreateLogicalDevice Vulkan Queues: graphics=%p present=%p\n",
			m_graphicsQueue,
			VKwindowsToRenderArray[i].presentQueue);
	}

	return true;
}

bool vkClass::CreateSurface(UINT monitor)
{
	auto& vkWindow = VKwindowsToRenderArray[monitor];

#if defined WINDOWS_PLATFORM
	VkWin32SurfaceCreateInfoKHR surfaceInfo{};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.hinstance = SystemHandle->m_hinstance;
	surfaceInfo.hwnd = (HWND)WOMA::windowsToRenderArray[monitor].hWnd;

#elif defined ANDROID_PLATFORM
	//VkAndroidSurfaceCreateInfoKHR surfaceInfo{};
	//surfaceInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	//surfaceInfo.window = engine.app->window; //From: WOMA ENGINE

#elif defined(LINUX_PLATFORM)
	VkXlibSurfaceCreateInfoKHR surfaceInfo{};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.dpy = Win.display;     // O Display* obtido no XOpenDisplay
	surfaceInfo.window = Win.window;   // A Window obtida no XCreateWindow
#endif

#ifdef WINDOWS_PLATFORM
	// Refresh Win32 cached window sizes
	GetWindowRect(surfaceInfo.hwnd, &WOMA::windowsToRenderArray[monitor].m_rcWindowBounds);
	GetClientRect(surfaceInfo.hwnd, &WOMA::windowsToRenderArray[monitor].m_rcWindowClient);

	RECT& rcB = WOMA::windowsToRenderArray[monitor].m_rcWindowBounds;
	RECT& rcC = WOMA::windowsToRenderArray[monitor].m_rcWindowClient;

	womalog("WindowBounds : %ld x %ld\n", rcB.right - rcB.left, rcB.bottom - rcB.top);
	womalog("ClientRect   : %ld x %ld\n", rcC.right - rcC.left, rcC.bottom - rcC.top);

	VkResult result = vkCreateWin32SurfaceKHR(m_instance, &surfaceInfo, nullptr, &vkWindow.m_surface);

#elif defined(ANDROID_PLATFORM)
	VkAndroidSurfaceCreateInfoKHR surfaceInfo{};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = nullptr;
	surfaceInfo.flags = 0;
	surfaceInfo.window = engine.app->window;

	// 1. Declarar um ponteiro de função com a assinatura exata da extensão do Android
	typedef VkResult(VKAPI_PTR* PFN_vkCreateAndroidSurfaceKHR)(
		VkInstance instance,
		const VkAndroidSurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface);

	// 2. Pedir diretamente à instância do Vulkan o endereço real da função na GPU Adreno
	PFN_vkCreateAndroidSurfaceKHR pfnCreateAndroidSurface =
		(PFN_vkCreateAndroidSurfaceKHR)vkGetInstanceProcAddr(m_instance, "vkCreateAndroidSurfaceKHR");

	womalog("surfaceInfo.window = %p\n", surfaceInfo.window);
	womalog("android_app->window = %p\n", engine.app->window);
	womalog("FINAL surfaceInfo.window = %p\n", surfaceInfo.window);

	VkResult result;
	if (pfnCreateAndroidSurface != nullptr)
	{
		// 3. Se a GPU Adreno tiver a função (e ela tem!), chama-a diretamente pelo endereço de memória
		result = pfnCreateAndroidSurface(m_instance, &surfaceInfo, nullptr, &vkWindow.m_surface);
	}
	else
	{
		womalog("CRITICAL: vkGetInstanceProcAddr nao conseguiu encontrar a funcao 'vkCreateAndroidSurfaceKHR' no driver!\n");
		return false;
	}

	if (result != VK_SUCCESS)
	{
		womalog("vkCreateAndroidSurfaceKHR() falhou com o erro: %d\n", result);
		return false;
	}

#elif defined(LINUX_PLATFORM)
	// Usar a função correta da Xlib
	VkResult result = vkCreateXlibSurfaceKHR(m_instance, &surfaceInfo, nullptr, &vkWindow.m_surface);
#endif

	if (result != VK_SUCCESS)
	{
	#ifdef WINDOWS_PLATFORM
		womalog("vkCreateWin32SurfaceKHR() failed (%d)\n", result);

	#elif defined(ANDROID_PLATFORM)
		womalog("vkCreateAndroidSurfaceKHR() failed (%d)\n", result);

	#elif defined(LINUX_PLATFORM)
		womalog("vkCreateXlibSurfaceKHR() failed (%d)\n", result);
	#endif
		return false;
	}

	womalog("vkClass::CreateSurface Vulkan Surface Created\n");

	return true;
}

void vkClass::DestroySwapChainResources(UINT monitor)
{
	auto& vkWindow = VKwindowsToRenderArray[monitor];

	//CLEAR: Images & Views
	for (VkImageView view : vkWindow.m_swapchainImageViews)
		if (view)
			vkDestroyImageView(m_device, view, nullptr);

	vkWindow.m_swapchainImageViews.clear();
	vkWindow.m_swapchainImages.clear();

	vkWindow.m_swapchainImageViews.shrink_to_fit();
	vkWindow.m_swapchainImages.shrink_to_fit();

	for (VkFramebuffer fb : vkWindow.m_framebuffers)
	{
		if (fb)
			vkDestroyFramebuffer(m_device, fb, nullptr);
	}

	vkWindow.m_framebuffers.clear();
	vkWindow.m_framebuffers.shrink_to_fit();

	if (vkWindow.depthView)
	{
		vkDestroyImageView(m_device, vkWindow.depthView, nullptr);
		vkWindow.depthView = VK_NULL_HANDLE;
	}

	if (vkWindow.depthImage)
	{
		vkDestroyImage(m_device, vkWindow.depthImage, nullptr);
		vkWindow.depthImage = VK_NULL_HANDLE;
	}

	if (vkWindow.depthMemory)
	{
		vkFreeMemory(m_device, vkWindow.depthMemory, nullptr);
		vkWindow.depthMemory = VK_NULL_HANDLE;
	}

	if (vkWindow.m_swap_chain)
	{
		vkDestroySwapchainKHR(m_device, vkWindow.m_swap_chain, nullptr);
		vkWindow.m_swap_chain = VK_NULL_HANDLE;
	}
}

bool vkClass::create_or_resize_swap(UINT monitor, int screenWidth, int screenHeight, BOOL fullscreen)
{
	auto& vkWindow = VKwindowsToRenderArray[monitor];

	// 1) OBRIGATÓRIO: Esperar que o GPU termine todo o trabalho pendente antes de destruir recursos
	if (m_device) {
		vkDeviceWaitIdle(m_device);
	}

	// 2) Destruir recursos antigos da Swapchain deste monitor específico
	if (vkWindow.m_swap_chain)
	{
		DestroySwapChainResources(monitor);
	}

	// 3) Destruir o Render Pass DESTE monitor (Mudado de global para local da struct)
	if (vkWindow.m_render_pass)
	{
		vkDestroyRenderPass(m_device, vkWindow.m_render_pass, nullptr);
		vkWindow.m_render_pass = VK_NULL_HANDLE;
	}

	// 4) Recriar as infraestruturas básicas de imagem
	IF_NOT_RETURN_FALSE(CreateSwapChain(monitor));


	return true;
}

bool vkClass::CreateSwapChain(UINT monitor)
{
	if (WOMA::game_state >= GAME_STOP)
		return false;

	womalog("vkClass::CreateSwapChain()\n");

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	//
	// Get physical Device Surface properties and formats
	//
	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, vkWindow.m_surface, &vkWindow.surfCapabilities);
	ASSERT(result == VK_SUCCESS);

	womalog("vkClass::CreateSwapChain Surface Capabilities\n");
	womalog("vkClass::CreateSwapChain     minImageCount    : %u\n", vkWindow.surfCapabilities.minImageCount);
	womalog("vkClass::CreateSwapChain     maxImageCount    : %u\n", vkWindow.surfCapabilities.maxImageCount);
	womalog("vkClass::CreateSwapChain     currentExtent    : %u x %u\n",vkWindow.surfCapabilities.currentExtent.width, vkWindow.surfCapabilities.currentExtent.height);

	//
	//Get format Count:
	//
	UINT formatCount = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, vkWindow.m_surface, &formatCount, nullptr);
	ASSERT(result == VK_SUCCESS);
	ASSERT(formatCount > 0);

	womalog("vkClass::CreateSwapChain Surface Formats: %u\n", formatCount);

	//Get Formats list:
	std::vector<VkSurfaceFormatKHR> formats(formatCount);

	result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, vkWindow.m_surface, &formatCount, formats.data());
	ASSERT(result == VK_SUCCESS);

	for (UINT i = 0; i < formatCount; i++)
		womalog("vkClass::CreateSwapChain Format[%u]: format=%d colorspace=%d\n", i, formats[i].format, formats[i].colorSpace);

	if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
		vkWindow.m_swapchainColorFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else {
		ASSERT(formatCount >= 1);
		vkWindow.m_swapchainColorFormat.format = formats[0].format;
	}

	//
	UINT presentModeCount;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, vkWindow.m_surface, &presentModeCount, NULL);
	ASSERT(result == VK_SUCCESS);

	std::vector<VkPresentModeKHR> presentModes(presentModeCount);

	womalog("vkClass::CreateSwapChain Present Modes: %u\n", presentModeCount);

	//
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, vkWindow.m_surface, &presentModeCount, presentModes.data());
	ASSERT(result == VK_SUCCESS);

	char* mode = "UNKNOWN";
	for (UINT i = 0; i < presentModeCount; i++)
	{
		switch (presentModes[i])
		{
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			mode = "IMMEDIATE";
			break;

		case VK_PRESENT_MODE_MAILBOX_KHR:
			mode = "MAILBOX";
			break;

		case VK_PRESENT_MODE_FIFO_KHR:
			mode = "FIFO";
			break;

		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			mode = "FIFO_RELAXED";
			break;
#ifdef VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR
		case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
			mode = "SHARED_DEMAND_REFRESH";
			break;
#endif
#ifdef VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR
		case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
			mode = "SHARED_CONTINUOUS_REFRESH";
			break;
#endif
		default:
		{
			static char txt[32];
			_stprintf(txt, "UNKNOWN(%d)", (int)presentModes[i]);
			mode = txt;
		}
		}

		womalog("vkClass::CreateSwapChain PresentMode[%u]: %s\n", i, mode);
	}

#if defined WINDOWS_PLATFORM
	// Sanity check:
	// The Vulkan Surface extent must always match the Win32 ClientRect.
	// If these values differ, the HWND/Surface association is incorrect.
	RECT& rc = WOMA::windowsToRenderArray[monitor].m_rcWindowClient;
	womalog("ClientRect      : %ld x %ld\n", rc.right - rc.left, rc.bottom - rc.top);
	womalog("Surface Extent  : %u x %u\n", vkWindow.surfCapabilities.currentExtent.width, vkWindow.surfCapabilities.currentExtent.height);
#elif defined LINUX_PLATFORM
	// Equivalente para Linux usando a dimensão que definiste no teu motor
	womalog("vkClass::CreateSwapChain Linux Win Size  : %d x %d\n", Win.width, Win.height);
	womalog("vkClass::CreateSwapChain Surface Extent  : %u x %u\n", vkWindow.surfCapabilities.currentExtent.width, vkWindow.surfCapabilities.currentExtent.height);
#endif

	if (vkWindow.surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		vkWindow.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else {
		vkWindow.preTransform = vkWindow.surfCapabilities.currentTransform;
	}

	// Find a supported composite alpha mode:
	for (UINT i = 0; i < sizeof(compositeAlphaFlags) / sizeof(compositeAlphaFlags[0]); i++) {
		if (vkWindow.surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
			vkWindow.compositeAlpha = compositeAlphaFlags[i];
			break;
		}
	}

	// Default required by Vulkan specification
	vkWindow.swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	if (!m_VSYNC_ENABLED)
	{
		for (auto mode : presentModes)
		{
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				vkWindow.swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}

			if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				vkWindow.swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
	}

	womalog("vkClass::CreateSwapChain Selected Present Mode: ");

	switch (vkWindow.swapchainPresentMode)
	{
	case VK_PRESENT_MODE_FIFO_KHR:
		womalog("FIFO (VSync)\n");
		break;

	case VK_PRESENT_MODE_MAILBOX_KHR:
		womalog("MAILBOX (Triple Buffer)\n");
		break;

	case VK_PRESENT_MODE_IMMEDIATE_KHR:
		womalog("IMMEDIATE (No VSync)\n");
		break;

	default:
		womalog("OTHER\n");
	}

	// --- FIX IMAGE COUNT ---
	UINT desiredSwapchainImagesCount;

	if (WOMA::AppSettings->UseTripleBuffering)
		desiredSwapchainImagesCount = 3;
	else
		desiredSwapchainImagesCount =
		(WOMA::AppSettings->UseDoubleBuffering) ? 2 : 1;

	if (desiredSwapchainImagesCount < vkWindow.surfCapabilities.minImageCount) {
		desiredSwapchainImagesCount = vkWindow.surfCapabilities.minImageCount;
	}
	if (vkWindow.surfCapabilities.maxImageCount > 0 && desiredSwapchainImagesCount > vkWindow.surfCapabilities.maxImageCount) {
		desiredSwapchainImagesCount = vkWindow.surfCapabilities.maxImageCount;
	}

	//VkSwapchainCreateInfoKHR swapchain_ci{};									 
	swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_ci.pNext = NULL;
	swapchain_ci.surface = vkWindow.m_surface;
	swapchain_ci.minImageCount = desiredSwapchainImagesCount;
	swapchain_ci.imageFormat = vkWindow.m_swapchainColorFormat.format;

	swapchain_ci.imageExtent = vkWindow.surfCapabilities.currentExtent;

	swapchain_ci.preTransform = vkWindow.preTransform;
	swapchain_ci.compositeAlpha = vkWindow.compositeAlpha;
	swapchain_ci.imageArrayLayers = 1;
	swapchain_ci.presentMode = vkWindow.swapchainPresentMode;
	swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
	swapchain_ci.clipped = true;
	//swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	swapchain_ci.imageColorSpace = vkWindow.m_swapchainColorFormat.colorSpace;
	swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_ci.queueFamilyIndexCount = 0;
	swapchain_ci.pQueueFamilyIndices = NULL;
	UINT queueFamilyIndices[2] = { m_graphicsQueueFamilyIndex, m_present_queue_family_index };
	if (m_graphicsQueueFamilyIndex != m_present_queue_family_index) {
		swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_ci.queueFamilyIndexCount = 2;
		swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
	}

	result = vkCreateSwapchainKHR(m_device, &swapchain_ci, NULL, &vkWindow.m_swap_chain);
	ASSERT(result == VK_SUCCESS);

	womalog("vkClass::CreateSwapChain Monitor[%u]: vkCreateSwapchainKHR() returned %d\n", monitor, result);

	return true;
}


bool vkClass::CreateSwapChainImageViews(UINT monitor)
{
	womalog("vkClass::CreateSwapChainImageViews()\n");

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	// 1) How many images there are?
	UINT imageCount = 0;

	VkResult result = vkGetSwapchainImagesKHR(m_device, vkWindow.m_swap_chain, &imageCount, nullptr);
	ASSERT(result == VK_SUCCESS);
	
	womalog("vkClass::CreateSwapChainImageViews SwapChain Images: %u\n", imageCount);

	// 2) Get handles for swap chain images:
	vkWindow.m_swapchainImages.resize(imageCount);

	result = vkGetSwapchainImagesKHR(m_device, vkWindow.m_swap_chain, &imageCount, vkWindow.m_swapchainImages.data());
	ASSERT(result == VK_SUCCESS);

#ifdef VULKAN1_1
	vkWindow.imagesInFlight.resize(vkWindow.m_swapchainImages.size(), VK_NULL_HANDLE);
#endif

	for (UINT i = 0; i < imageCount; i++)
		womalog("SwapChain Image[%u] = %p\n", i, (void*)vkWindow.m_swapchainImages[i]);

	vkWindow.m_swapchainImageViews.resize(imageCount);
	for (UINT i = 0; i < imageCount; i++) {
		//VkImageViewCreateInfo imageViewCI = {};
		imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCI.image = vkWindow.m_swapchainImages[i];
		imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCI.format = vkWindow.m_swapchainColorFormat.format;

		// NO Remap Channels:
		imageViewCI.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCI.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCI.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCI.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCI.subresourceRange.baseMipLevel = 0;
		imageViewCI.subresourceRange.levelCount = 1;
		imageViewCI.subresourceRange.baseArrayLayer = 0;
		imageViewCI.subresourceRange.layerCount = 1;

		result = vkCreateImageView(m_device, &imageViewCI, NULL, &vkWindow.m_swapchainImageViews[i]);
		ASSERT(result == VK_SUCCESS);

		womalog("ImageView[%u] created\n", i);
	}

	womalog("vkClass::CreateSwapChainImageViews Created %u SwapChain ImageViews\n", imageCount);

	return true;
}

VkSampleCountFlagBits vkClass::GetMSAASampleCount()
{
	VkSampleCountFlagBits MSAA_COUNT = VK_SAMPLE_COUNT_1_BIT;

	switch (MSAA_COUNT)
	{
		//case 1:  return VK_SAMPLE_COUNT_1_BIT;
		case 2:  MSAA_COUNT = VK_SAMPLE_COUNT_2_BIT;
		case 4:  MSAA_COUNT = VK_SAMPLE_COUNT_4_BIT;
		case 8:  MSAA_COUNT = VK_SAMPLE_COUNT_8_BIT;
		case 16: MSAA_COUNT = VK_SAMPLE_COUNT_16_BIT;
		case 32: MSAA_COUNT = VK_SAMPLE_COUNT_32_BIT;
		case 64: MSAA_COUNT = VK_SAMPLE_COUNT_64_BIT;
	}

	womalog("vkClass::GetMSAASampleCount(): %u\n", MSAA_COUNT);

	return MSAA_COUNT;
}

VkFormat vkClass::FindDepthFormat()
{
	const VkFormat candidates[] =
	{
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};

	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(
			m_physicalDevice,
			format,
			&props);

		if (props.optimalTilingFeatures &
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			return format;
		}
	}

	womalog("vkClass::FindDepthFormat No supported depth format found!");
	abort();
	return VK_FORMAT_UNDEFINED;
}

bool vkClass::CreateRenderPass(UINT monitor)
{
	womalog("vkClass::CreateRenderPass (Monitor=%u)\n", monitor);

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	//VkAttachmentDescription colorAttachment{};
	colorAttachment.format = vkWindow.m_swapchainColorFormat.format;
	colorAttachment.samples = msaaSamples;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachment.flags = 0;

	//m_depthFormat = FindDepthFormat();
	//womalog("Depth Format: %d\n", m_depthFormat);

	//VkAttachmentDescription depthAttachment{};
	depthAttachment.format = m_depthFormat;
	depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachment.flags = 0;

	VkAttachmentDescription attachments[] =
	{
		colorAttachment,
		depthAttachment
	};

	//VkAttachmentReference color_reference = {};
	color_reference.attachment = 0;
	color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//VkAttachmentReference depth_reference = {};
	depth_reference.attachment = 1;
	depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags = 0;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = NULL;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_reference;
	subpass.pResolveAttachments = NULL;
	subpass.pDepthStencilAttachment = &depth_reference;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = NULL;

	// Subpass dependency to wait for wsi image acquired semaphore before starting layout transition
	//VkSubpassDependency subpass_dependency = {};
	subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependency.dstSubpass = 0;
	subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.srcAccessMask = 0;
	subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpass_dependency.dependencyFlags = 0;

	VkRenderPassCreateInfo rp_info = {};
	rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rp_info.pNext = NULL;
	rp_info.attachmentCount = _countof(attachments); //sizeof(attachments) / sizeof(attachments[0]);
	rp_info.pAttachments = attachments;
	rp_info.subpassCount = 1;
	rp_info.pSubpasses = &subpass;
	rp_info.dependencyCount = 1;
	rp_info.pDependencies = &subpass_dependency;

	//
	//
	//
	VkResult result = vkCreateRenderPass(m_device, &rp_info, NULL, &VKwindowsToRenderArray[monitor].m_render_pass);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateRenderPass() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateRenderPass created (Monitor=%u)\n", monitor);


	return true;
}

bool vkClass::memory_type_from_properties(UINT typeBits, VkFlags requirements_mask, UINT* typeIndex)
{
	for (UINT i = 0; i < m_memoryProperties.memoryTypeCount; i++) {
		if ((typeBits & 1) == 1) {
			if ((m_memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
				*typeIndex = i;
				return true;
			}
		}
		typeBits >>= 1;
	}

	return false;
}

bool vkClass::CreateDepthResources(UINT monitor)
{
	womalog("vkClass::CreateDepthResources()\n");

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	/* VULKAN_KEY_START */
	//VkImageCreateInfo image_info = {};

	VkFormatProperties props;
	vkGetPhysicalDeviceFormatProperties(m_physicalDevice, m_depthFormat, &props);
	if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		image_info.tiling = VK_IMAGE_TILING_LINEAR;
	}
	else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	}
	else {
		/* Try other depth formats? */
		womalog("vkClass::CreateDepthResources Selected depth format not supported.\n");
		WomaFatalException("vkClass::CreateDepthResources Unsupported depth format");
	}

	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.pNext = NULL;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.format = m_depthFormat;

	image_info.extent.width = vkWindow.surfCapabilities.currentExtent.width;
	image_info.extent.height = vkWindow.surfCapabilities.currentExtent.height;

	image_info.extent.depth = 1;
	image_info.mipLevels = 1;
	image_info.arrayLayers = 1;
	image_info.samples = msaaSamples;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	image_info.queueFamilyIndexCount = 0;
	image_info.pQueueFamilyIndices = NULL;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.flags = 0;

	/* Create image */
	VkResult result = vkCreateImage(m_device, &image_info, NULL, &vkWindow.depthImage);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateDepthResources vkCreateImage() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateDepthResources Depth Image created\n");

	VkMemoryRequirements mem_reqs;
	vkGetImageMemoryRequirements(m_device, vkWindow.depthImage, &mem_reqs);

	//VkMemoryAllocateInfo mem_alloc = {};
	mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_alloc.pNext = NULL;
	mem_alloc.allocationSize = 0;
	mem_alloc.memoryTypeIndex = 0;

	mem_alloc.allocationSize = mem_reqs.size;
	/* Use the memory properties to determine the type of memory required */
	bool pass = memory_type_from_properties(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
	ASSERT(pass);

	/* Allocate memory */
	result = vkAllocateMemory(m_device, &mem_alloc, NULL, &vkWindow.depthMemory);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateDepthResources vkAllocateMemory() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateDepthResources Depth Memory allocated\n");


	/* Bind memory */
	result = vkBindImageMemory(m_device, vkWindow.depthImage, vkWindow.depthMemory, 0);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateDepthResources vkBindImageMemory() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateDepthResources Depth Image memory bound\n");

	//VkImageViewCreateInfo view_info = {};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.pNext = NULL;
	view_info.image = VK_NULL_HANDLE;
	view_info.format = m_depthFormat;

	view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = 1;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount = 1;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.flags = 0;

	/* Create image view */
	view_info.image = vkWindow.depthImage;
	result = vkCreateImageView(m_device, &view_info, NULL, &vkWindow.depthView);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateDepthResources vkCreateImageView() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateDepthResources Depth ImageView created\n");

	return true;
}

bool vkClass::CreateFramebuffers(UINT monitor)
{
	womalog("vkClass::CreateFramebuffers()\n");

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	VkImageView attachments[2];
	attachments[1] = vkWindow.depthView;

	//VkFramebufferCreateInfo fb_info{};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.pNext = NULL;
	fb_info.renderPass = VKwindowsToRenderArray[monitor].m_render_pass;
	fb_info.attachmentCount = 2;
	fb_info.pAttachments = attachments;
	fb_info.width = vkWindow.surfCapabilities.currentExtent.width;
	fb_info.height = vkWindow.surfCapabilities.currentExtent.height;

	fb_info.layers = 1;

	vkWindow.m_framebuffers.resize(vkWindow.m_swapchainImageViews.size());
	for (size_t i = 0; i < vkWindow.m_swapchainImageViews.size(); i++)
	{
		attachments[0] = vkWindow.m_swapchainImageViews[i];
		VkResult result = vkCreateFramebuffer(m_device, &fb_info, nullptr, &vkWindow.m_framebuffers[i]);
		if (result != VK_SUCCESS)
		{
			womalog("vkClass::CreateFramebuffers() failed (%d)\n", result);
			return false;
		}
		womalog("Framebuffer %u created\n", i);
	}

	womalog("vkClass::CreateFramebuffers Created %u Framebuffers\n", (UINT)vkWindow.m_framebuffers.size());

	return true;
}

bool vkClass::CreateCommandPool()
{
	womalog("vkClass::CreateCommandPool()\n");

	/* Create a command pool to allocate our command buffer from */
	//VkCommandPoolCreateInfo cmd_pool_info = {};
	cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmd_pool_info.queueFamilyIndex = m_graphicsQueueFamilyIndex;
	cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkResult result = vkCreateCommandPool(m_device, &cmd_pool_info, NULL, &m_commandPool);
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateCommandPool() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateCommandPool CommandPool created (QueueFamily=%u)\n", m_graphicsQueueFamilyIndex);
	womalog("vkClass::CreateCommandPool CommandPool = %p\n", m_commandPool);

	return true;
}

bool vkClass::CreateCommandBuffers(UINT monitor)
{
	womalog("vkClass::CreateCommandBuffers()\n");

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	vkWindow.m_commandBuffers.resize(vkWindow.m_framebuffers.size());

	/* Create the command buffer from the command pool */
	//VkCommandBufferAllocateInfo cmd{};
	cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd.pNext = NULL;
	cmd.commandPool = m_commandPool;
	cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd.commandBufferCount = (UINT)vkWindow.m_commandBuffers.size();

	VkResult result = vkAllocateCommandBuffers(m_device, &cmd, vkWindow.m_commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateCommandBuffers() failed (%d)\n", result);
		return false;
	}

	womalog("vkClass::CreateCommandBuffers Allocated %u CommandBuffers\n", (UINT)vkWindow.m_commandBuffers.size());

	return true;
}

bool vkClass::CreateSyncObjects(UINT monitor)
{
	womalog("vkClass::CreateSyncObjects()\n");

	auto& vkWindow = VKwindowsToRenderArray[monitor];

	//VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	//VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkResult result;

	// ---------------------------------------------------------
	// Frames-In-Flight resources
	// ---------------------------------------------------------

	vkWindow.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	vkWindow.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		result = vkCreateSemaphore(
			m_device,
			&semaphoreInfo,
			nullptr,
			&vkWindow.imageAvailableSemaphores[i]);

		if (result != VK_SUCCESS)
		{
			womalog("vkClass::CreateSyncObjects vkCreateSemaphore(imageAvailable[%u]) failed (%d)\n", i, result);
			return false;
		}

		result = vkCreateFence(
			m_device,
			&fenceInfo,
			nullptr,
			&vkWindow.inFlightFences[i]);

		if (result != VK_SUCCESS)
		{
			womalog("vkClass::CreateSyncObjects vkCreateFence(inFlight[%u]) failed (%d)\n", i, result);
			return false;
		}
	}

	// ---------------------------------------------------------
	// Um renderFinished por imagem da SwapChain
	// ---------------------------------------------------------

	vkWindow.renderFinishedSemaphores.resize(vkWindow.m_swapchainImages.size());
	//vkWindow.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < vkWindow.renderFinishedSemaphores.size(); ++i)
	{
		result = vkCreateSemaphore(
			m_device,
			&semaphoreInfo,
			nullptr,
			&vkWindow.renderFinishedSemaphores[i]);

		if (result != VK_SUCCESS)
		{
			womalog("vkClass::CreateSyncObjects vkCreateSemaphore(renderFinished[%zu]) failed (%d)\n", i, result);
			return false;
		}
	}

#ifdef VULKAN1_1
	ASSERT(vkWindow.imageAvailableSemaphores.size() == MAX_FRAMES_IN_FLIGHT);
	//ASSERT(vkWindow.renderFinishedSemaphores.size() == MAX_FRAMES_IN_FLIGHT);
	ASSERT(vkWindow.renderFinishedSemaphores.size() == vkWindow.m_swapchainImages.size());
	ASSERT(vkWindow.inFlightFences.size() == MAX_FRAMES_IN_FLIGHT);

	vkWindow.imagesInFlight.resize(vkWindow.m_swapchainImages.size(), VK_NULL_HANDLE);

	womalog("vkClass::CreateSyncObjects Sync: swapImages=%zu imagesInFlight=%zu currentFrame=%u\n",
		vkWindow.m_swapchainImages.size(),
		vkWindow.imagesInFlight.size(),
		vkWindow.m_currentFrame);
#endif

	womalog("vkClass::CreateSyncObjects Synchronization objects created\n");
	return true;
}

// -----------------------------------------------------------------
void vkClass::BeginScene(UINT monitorWindow)
{
	auto& vkWindow = VKwindowsToRenderArray[monitorWindow];

	ASSERT_DEBUG(VKwindowsToRenderArray.size() > 0);

	VkResult result = vkWaitForFences(m_device, 1, &vkWindow.inFlightFences[vkWindow.m_currentFrame], VK_TRUE, UINT64_MAX);
	ASSERT_DEBUG(result == VK_SUCCESS);

	// 1) Pedir à Swapchain o índice da próxima imagem disponível e sinalizar o semáforo
	result = vkAcquireNextImageKHR(
		m_device,
		vkWindow.m_swap_chain,
		UINT64_MAX,                  // Esperar o tempo que for preciso
		VKwindowsToRenderArray[monitorWindow].imageAvailableSemaphores[vkWindow.m_currentFrame],     // Semáforo que será sinalizado quando a imagem estiver pronta
		VK_NULL_HANDLE,              // Sem Fence (visto que já usas Semáforo)
		&vkWindow.m_currentImageIndex // O driver vai preencher esta variável com o índice (ex: 0, 1 ou 2)
	);

	// Tratar cenários onde a janela mudou de tamanho em Linux durante o acquire
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		womalog("VK_ERROR_OUT_OF_DATE_KHR\n");
#if defined USE_ALLOW_MAINWINDOW_RESIZE
		SystemHandle->ONRESIZE(/*pContext*/ NULL);
#endif
		return;
	}

	ASSERT_DEBUG(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);

	// 2) Agora sim, os teus ASSERTS e o resto do código funcionam com o índice atualizado!
	ASSERT(vkWindow.m_currentImageIndex < vkWindow.m_commandBuffers.size());
	ASSERT(vkWindow.m_currentImageIndex < vkWindow.m_framebuffers.size());

#ifdef VULKAN1_1
	// image belong to another frame and must wait until that frame is finished
	if (vkWindow.imagesInFlight[vkWindow.m_currentImageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(m_device, 1, &vkWindow.imagesInFlight[vkWindow.m_currentImageIndex], VK_TRUE, UINT64_MAX);
	}
#endif

	result = vkResetFences(m_device, 1, &vkWindow.inFlightFences[vkWindow.m_currentFrame]);
	ASSERT_DEBUG(result == VK_SUCCESS);

	VkCommandBuffer cmd = vkWindow.m_commandBuffers[vkWindow.m_currentImageIndex];
	m_currentCommandBuffer = cmd;

	// Reset previous contents
	result = vkResetCommandBuffer(cmd, 0);
	ASSERT_DEBUG(result == VK_SUCCESS);

	//VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	//
	// [1]
	//
	result = vkBeginCommandBuffer(cmd, &beginInfo);
	ASSERT(result == VK_SUCCESS);

	//VkRenderPassBeginInfo rp_begin{};
	rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rp_begin.renderPass = VKwindowsToRenderArray[monitorWindow].m_render_pass;
	rp_begin.framebuffer = vkWindow.m_framebuffers[vkWindow.m_currentImageIndex];
	rp_begin.renderArea.offset.x = 0;
	rp_begin.renderArea.offset.y = 0;
	rp_begin.renderArea.extent = vkWindow.surfCapabilities.currentExtent;

	// Synchronize Vulkan clear color with the common WoMA driver state.
	m_clearColor.color.float32[0] = driver_ClearColor[0];
	m_clearColor.color.float32[1] = driver_ClearColor[1];
	m_clearColor.color.float32[2] = driver_ClearColor[2];
	m_clearColor.color.float32[3] = driver_ClearColor[3];

	VkClearValue clearValues[2];
	clearValues[0] = m_clearColor;
	clearValues[1].depthStencil = { 1.0f, 0 };

	rp_begin.clearValueCount = 2;
	rp_begin.pClearValues = clearValues;

	//
	//[2]
	//
	vkCmdBeginRenderPass(cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)vkWindow.surfCapabilities.currentExtent.width;
	viewport.height = (float)vkWindow.surfCapabilities.currentExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vkCmdSetViewport(cmd, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = vkWindow.surfCapabilities.currentExtent;

	vkCmdSetScissor(cmd, 0, 1, &scissor);
}

// ----------------------------------------------------------------------------------------------
void vkClass::ClearDepthBuffer(void* pContext)
// ----------------------------------------------------------------------------------------------
{
	VkCommandBuffer commandBuffer = (VkCommandBuffer)pContext;

	auto& vkWindow = VKwindowsToRenderArray[0];

	//VkClearAttachment clearAttachment{};
	clearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	clearAttachment.clearValue.depthStencil.depth = 1.0f;
	clearAttachment.clearValue.depthStencil.stencil = 0;

	//VkClearRect clearRect{};
	clearRect.rect.offset = { 0, 0 };
	clearRect.rect.extent =
	{
		vkWindow.surfCapabilities.currentExtent.width,
		vkWindow.surfCapabilities.currentExtent.height
	};
	clearRect.baseArrayLayer = 0;
	clearRect.layerCount = 1;

	vkCmdClearAttachments(commandBuffer, 1, &clearAttachment, 1, &clearRect);
}

// TODO[Vulkan]:
// Centralize descriptor/resource management.
//
// DX11 requires ResetResource() because SRVs remain bound to the immediate context.
// Vulkan records bindings per CommandBuffer, so a frame-end reset is unnecessary.
//
// Future architecture:
//   - One central resource manager owns Images, ImageViews and Samplers.
//   - DescriptorSets are updated only when resources change.
//   - CommandBuffers only bind the appropriate DescriptorSet for each draw.
//   - Avoid traversing models/shaders to perform global cleanup.
//
void vkClass::ResetResource(void* pContext, UINT monitorWindow)
{
}

void vkClass::EndScene(UINT monitorWindow)
{
	if (WOMA::game_state == GAME_MINIMIZED)
		return;

	auto& vkWindow = VKwindowsToRenderArray[monitorWindow];

	if (vkWindow.m_commandBuffers.empty())
		return;

	if (vkWindow.m_currentImageIndex >= vkWindow.m_commandBuffers.size())
		return;

	//
	// 1
	//
	VkCommandBuffer cmd = vkWindow.m_commandBuffers[vkWindow.m_currentImageIndex];
	vkCmdEndRenderPass(cmd);

	//
	// 2
	//
	VkResult result = vkEndCommandBuffer(cmd);
	ASSERT_DEBUG(result == VK_SUCCESS);
	//3
#ifdef VULKAN1_1
	vkWindow.imagesInFlight[vkWindow.m_currentImageIndex] = vkWindow.inFlightFences[vkWindow.m_currentFrame];
#endif


	//4
	VkPipelineStageFlags waitStages[] =
	{
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	};

	//VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &vkWindow.imageAvailableSemaphores[vkWindow.m_currentFrame];
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmd;

	submitInfo.signalSemaphoreCount = 1;
	//submitInfo.pSignalSemaphores = &vkWindow.renderFinishedSemaphores[vkWindow.m_currentFrame];
	submitInfo.pSignalSemaphores = &vkWindow.renderFinishedSemaphores[vkWindow.m_currentImageIndex];

	result = vkQueueSubmit( m_graphicsQueue, 1, &submitInfo, vkWindow.inFlightFences[vkWindow.m_currentFrame]);		
	ASSERT_DEBUG(result == VK_SUCCESS);

	//5
	//VkPresentInfoKHR presentInfo{};

	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	//presentInfo.pWaitSemaphores = &vkWindow.renderFinishedSemaphores[vkWindow.m_currentFrame];
	presentInfo.pWaitSemaphores = &vkWindow.renderFinishedSemaphores[vkWindow.m_currentImageIndex];

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &vkWindow.m_swap_chain;
	presentInfo.pImageIndices = &vkWindow.m_currentImageIndex;

	result = vkQueuePresentKHR( /*m_graphicsQueue*/vkWindow.presentQueue, &presentInfo);
	switch (result)
	{
		case VK_SUCCESS:
			break;

		case VK_SUBOPTIMAL_KHR:
		// NOTE Android Vulkan : VK_ERROR_OUT_OF_DATE_KHR deixou de disparar ONRESIZE() a partir de vkQueuePresentKHR()
	#if !defined ANDROID_PLATFORM
		case VK_ERROR_OUT_OF_DATE_KHR:
	#if defined _DEBUG
			womalog("vkClass::EndScene VK_ERROR_OUT_OF_DATE_KHR\n");
	#endif
	#if defined USE_ALLOW_MAINWINDOW_RESIZE
			SystemHandle->ONRESIZE(/*pContext*/ NULL);
	#endif
	#else
		// Android: tolerate SUBOPTIMAL during surface stabilization.
	#endif
			break;

		default:
			womalog("vkClass::EndScene VK_presetn_default_ERROR\n");
			ASSERT_DEBUG(false);
			break;
	}

	vkWindow.m_currentFrame = (vkWindow.m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
void vkClass::TurnOnAlphaBlending(void* pContext)
{
	//if (g_AlphaBlend) return;
	// Turn on the alpha blending.
	g_AlphaBlend = true;
}

void vkClass::TurnOffAlphaBlending(void* pContext)
{
	//if (!g_AlphaBlend) return;
	// Turn off the alpha blending.
	g_AlphaBlend = false;
}
#endif

void vkClass::SetCamera2D()
{
	RenderfirstTime = true;
	
	GLcameraClass GLm_Camera;
	
	// Set Fixed Camera for 2D:
	GLm_Camera.SetRotation(0, 0, 0);			// NOTE: On 2D This values have always these values!
	GLm_Camera.SetPosition(0.0f, 0.0f, -1.0f);	// NOTE: On 2D This values have always these values!
	
	// Calculate: 2D ViewMatrix
	GLm_Camera.CalculateViewMatrix();			// ((OpenGLClass*)m_Driver)->DX_Camera->Render(); || ((DX_CLASS*)m_Driver)->DX_Camera->Render();
	
	// Get the view from the camera and 2D objects.
	gl_Camera->m_viewmatrix2D.m[0]  = GLm_Camera.m_viewMatrix.m[0];
	gl_Camera->m_viewmatrix2D.m[1]  = GLm_Camera.m_viewMatrix.m[1];
	gl_Camera->m_viewmatrix2D.m[2]  = GLm_Camera.m_viewMatrix.m[2];
	gl_Camera->m_viewmatrix2D.m[3]  = GLm_Camera.m_viewMatrix.m[3];
	
	gl_Camera->m_viewmatrix2D.m[4]  = GLm_Camera.m_viewMatrix.m[4];
	gl_Camera->m_viewmatrix2D.m[5]  = GLm_Camera.m_viewMatrix.m[5];
	gl_Camera->m_viewmatrix2D.m[6]  = GLm_Camera.m_viewMatrix.m[6];
	gl_Camera->m_viewmatrix2D.m[7]  = GLm_Camera.m_viewMatrix.m[7];
	
	gl_Camera->m_viewmatrix2D.m[8]  = GLm_Camera.m_viewMatrix.m[8];
	gl_Camera->m_viewmatrix2D.m[9]  = GLm_Camera.m_viewMatrix.m[9];
	gl_Camera->m_viewmatrix2D.m[10] = GLm_Camera.m_viewMatrix.m[10];
	gl_Camera->m_viewmatrix2D.m[11] = GLm_Camera.m_viewMatrix.m[11];
	
	gl_Camera->m_viewmatrix2D.m[12] = GLm_Camera.m_viewMatrix.m[12];
	gl_Camera->m_viewmatrix2D.m[13] = GLm_Camera.m_viewMatrix.m[13];
	gl_Camera->m_viewmatrix2D.m[14] = GLm_Camera.m_viewMatrix.m[14];
	gl_Camera->m_viewmatrix2D.m[15] = GLm_Camera.m_viewMatrix.m[15];
}

// ------------------------------------------------------------------
void vkClass::Initialize2Dand3DCamera()
// ------------------------------------------------------------------
{
	if (!gl_Camera)
	{
		gl_Camera = NEW GLcameraClass; // GL Implementation
		IF_NOT_THROW_EXCEPTION(gl_Camera);
	}

#if defined USE_VIEW2D_SPRITES // 26
	SetCamera2D();
#endif

	gl_Camera->SetPosition(	WOMA::AppSettings->INIT_CAMX, 
							WOMA::AppSettings->INIT_CAMY,//+0.35f,
							WOMA::AppSettings->INIT_CAMZ);

	gl_Camera->SetRotation(	WOMA::AppSettings->INIT_ROTX, 
							WOMA::AppSettings->INIT_ROTY,
							WOMA::AppSettings->INIT_ROTZ);

	gl_Camera->CalculateViewMatrix();

	// SETUP 3D Sky Camera:
}

// ----------------------------------------------------------------------------------------------
void vkClass::setProjectionMatrixWorldMatrixOrthoMatrix(int screenWidth, int screenHeight, float screenNear, float screenDepth)
// ----------------------------------------------------------------------------------------------
{
	ASSERT_DEBUG(screenWidth * screenHeight);
	ASSERT_DEBUG(screenNear > 0);
	ASSERT_DEBUG(screenDepth > screenNear);

	// Create the projection matrix:
	UINT num_monitors = (UINT)WOMA::windowsToRenderArray.size();

	float fieldOfView = (float)(PI / 4.0f) / // Or... 90deg => fieldOfView = (90 / 2) * 0,0174532925f;
		num_monitors;		 // 90: 3(num "Impar" monitors)
	float screenAspect = (float)screenWidth / (float)screenHeight;

	m_projectionMatrix = vkmathClass->BuildPerspectiveFovLHMatrix(fieldOfView, screenAspect, screenNear, screenDepth);

#ifdef USE_SKY_CAMERA_DOME
	m_projectionMatrix_sky = vkmathClass->BuildPerspectiveFovLHMatrix(fieldOfView, screenAspect, screenNear, 512.0f);
#endif

#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES
	BuildOrthoMatrix(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth, true /*leftHand*/);
#endif

}

// ------------------------------------------------------------------
bool vkClass::Initialize(float* clearColor, void* pContext)
// ------------------------------------------------------------------
{
	auto& vkWindow = ((vkClass*)m_Driver)->VKwindowsToRenderArray[0];

	//mathClass = NULL;
	if (!vkmathClass)
	{
		vkmathClass = NEW GLmathClass;
		IF_NOT_THROW_EXCEPTION (vkmathClass);
	}

	womalog ("vkClass::Initialize()\n");

	// Set default clear color:
	driver_ClearColor[0] = *clearColor++;
	driver_ClearColor[1] = *clearColor++;
	driver_ClearColor[2] = *clearColor++;
	driver_ClearColor[3] = *clearColor++;

	Initialize2Dand3DCamera(); //Calculate ViewMatrix


	// Set the field of view and screen aspect ratio.
	float fieldOfView = 3.14159265358979323846f / 4.0f;
	//float screenAspect = (float)WOMA::AppSettings->WINDOW_WIDTH / (float)WOMA::AppSettings->WINDOW_HEIGHT;
	float screenAspect =
		static_cast<float>(vkWindow.surfCapabilities.currentExtent.width) /
		static_cast<float>(vkWindow.surfCapabilities.currentExtent.height);

	// Build the perspective projection matrix.
	m_projectionMatrix =vkmathClass->BuildPerspectiveFovLHMatrix(fieldOfView, screenAspect, WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH);

	//
	// CREATE A TEXTURE SAMPLER:
	//
	// Shadow Map use Clamp (no Repeat)
	IF_NOT_RETURN_FALSE(CreateVulkanSampler(&m_vkSampler[SAMPLER_WRAP_FALSE], false));
	IF_NOT_RETURN_FALSE(CreateVulkanSampler(&m_vkSampler[SAMPLER_WRAP_TRUE], true));

	return true;
}


void vkClass::addText(int Xpos, int Ypos, TCHAR* printText, float R, float G, float B)
{
}

void vkClass::RenderDriverText(void* pContext)
{
}

void vkClass::GetProjectionMatrix(float* matrix)
{
	matrix = (float*)&m_projectionMatrix;	//memcpy (matrix, m_projectionMatrix, 16*sizeof(float));
}


void vkClass::GetVideoCardInfo(char* cardName)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
}

void vkClass::BuildOrthoMatrix(mat4* matrix, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool leftHand)
{
	if (leftHand == true)
	{
		matrix->m[0] = 2.0f / screenWidth;
		matrix->m[1] = 0.0f;
		matrix->m[2] = 0.0f;
		matrix->m[3] = 0.0f;

		matrix->m[4] = 0.0f;
		matrix->m[5] = 2.0f / screenHeight;
		matrix->m[6] = 0.0f;
		matrix->m[7] = 0.0f;

		matrix->m[8] = 0.0f;
		matrix->m[9] = 0.0f;
		matrix->m[10] = 1.0f / (screenDepth - screenNear);
		matrix->m[11] = 0.0f;

		matrix->m[12] = 0.0f;
		matrix->m[13] = 0.0f;
		matrix->m[14] = screenNear / (screenNear - screenDepth);
		matrix->m[15] = 1.0f;
	}
	else
	{
		matrix->m[0] = 2.0f / screenWidth;
		matrix->m[1] = 0.0f;
		matrix->m[2] = 0.0f;
		matrix->m[3] = 0.0f;

		matrix->m[4] = 0.0f;
		matrix->m[5] = 2.0f / screenHeight;
		matrix->m[6] = 0.0f;
		matrix->m[7] = 0.0f;

		matrix->m[8] = 0.0f;
		matrix->m[9] = 0.0f;
		matrix->m[10] = 1.0f / (screenNear - screenDepth);
		matrix->m[11] = 0.0f;

		matrix->m[12] = 0.0f;
		matrix->m[13] = 0.0f;
		matrix->m[14] = screenNear / (screenNear - screenDepth);
		matrix->m[15] = 1.0f;
	}

	return;
}

UINT vkClass::FindMemoryType(UINT typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;

	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	for (UINT i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			//womalog("vkClass::FindMemoryType: %d\n", i);
			return i;
		}
	}

	WomaFatalException(("vkClass::FindMemoryType Failed to find suitable Vulkan memory type."));
	return 0;
}

void* vkClass::GetDeviceContext()
{
	return (void*)m_currentCommandBuffer;
}

void* vkClass::GetLoadContext()
{
	if (m_currentLoadCommandBuffer == VK_NULL_HANDLE)
	{
		//VkCommandBufferAllocateInfo cmdBufAllocateInfo{};
		cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufAllocateInfo.commandPool = m_commandPool;
		cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufAllocateInfo.commandBufferCount = 1;

		VkResult result = vkAllocateCommandBuffers(
			m_device,
			&cmdBufAllocateInfo,
			&m_currentLoadCommandBuffer);
		ASSERT_DEBUG(result == VK_SUCCESS);
	}
	else
	{
		// Reutilizar após o resize
		VkResult result = vkResetCommandBuffer(m_currentLoadCommandBuffer, 0);
		ASSERT_DEBUG(result == VK_SUCCESS);
	}

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkResult result = vkBeginCommandBuffer(m_currentLoadCommandBuffer, &beginInfo);
	ASSERT_DEBUG(result == VK_SUCCESS);

	return (void*)m_currentLoadCommandBuffer;
}

#ifndef ANDROID_PLATFORM
void vkClass::CreateDebugCallback()
{
	if (!ENABLE_DEBUGGING)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType =
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = DebugCallback;

	auto CreateDebugUtilsMessenger =
		(PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(
			m_instance,
			"vkCreateDebugUtilsMessengerEXT");

	if (!CreateDebugUtilsMessenger)
	{
		womalog("vkClass::CreateDebugCallback Could not load vkCreateDebugUtilsMessengerEXT\n");
		return;
	}

	VkResult result =
		CreateDebugUtilsMessenger(
			m_instance,
			&createInfo,
			nullptr,
			&m_debugMessenger);

	if (result != VK_SUCCESS)
	{
		womalog("vkClass::CreateDebugCallback Could not create Vulkan debug messenger (%d)\n", result);
		return;
	}

	womalog("vkClass::CreateDebugCallback Vulkan debug messenger created\n");

}
#endif

void vkClass::SetRasterizerState(void* pContext, UINT CullMode, UINT fillMode)
{
	m_CullMode = CullMode;
	m_fillMode = fillMode;
}

// ----------------------------------------------------------------------------------------------
void vkClass::TurnZBufferOn(void* pContext)
// ----------------------------------------------------------------------------------------------
{
	//if (g_Zbuffer) return;
	g_Zbuffer = true;

	if (m_Driver->RenderfirstTime)
		womalog("vkClass::TurnZBufferOn: Z=%d\n", g_Zbuffer);
}

// ----------------------------------------------------------------------------------------------
void vkClass::TurnZBufferOff(void* pContext)
// ----------------------------------------------------------------------------------------------
{
	//if (!g_Zbuffer) return;
	g_Zbuffer = false;

	if (m_Driver->RenderfirstTime)
		womalog("vkClass::TurnZBufferOff: Z=%d\n", g_Zbuffer);
}

bool vkClass::SubmitLoadContext()
{
	if (m_currentLoadCommandBuffer == VK_NULL_HANDLE)
		return true;

	if (m_Driver->RenderfirstTime)
		womalog("[vkClass::SubmitLoadContext] m_currentLoadCommandBuffer = %p\n", m_currentLoadCommandBuffer);

	VkResult result = vkEndCommandBuffer(m_currentLoadCommandBuffer);
	ASSERT_DEBUG(result == VK_SUCCESS);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_currentLoadCommandBuffer;

	result = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	ASSERT_DEBUG(result == VK_SUCCESS);

	result = vkQueueWaitIdle(m_graphicsQueue);
	ASSERT_DEBUG(result == VK_SUCCESS);

	return result == VK_SUCCESS;
}

void* vkClass::BeginOffscreenScene(UINT monitor)
{
	ASSERT(m_offscreenCommandBuffer == VK_NULL_HANDLE);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VK_CHECK(vkAllocateCommandBuffers(m_device, &allocInfo, &m_offscreenCommandBuffer));

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_CHECK(vkBeginCommandBuffer(m_offscreenCommandBuffer, &beginInfo));

	return m_offscreenCommandBuffer;
}

void vkClass::EndOffscreenScene(UINT monitor)
{
	ASSERT(m_offscreenCommandBuffer != VK_NULL_HANDLE);

	// Fechar o render pass da shadow
	//vkCmdEndRenderPass(m_offscreenCommandBuffer);

	VK_CHECK(vkEndCommandBuffer(m_offscreenCommandBuffer));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_offscreenCommandBuffer;

	VK_CHECK(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(m_graphicsQueue));

	vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_offscreenCommandBuffer);
	m_offscreenCommandBuffer = VK_NULL_HANDLE;
}

// --------------------------------------------------------------------------------------------
// GetViewMatrix()
// DX11 equivalent for selecting the correct view matrix depending on render pass.
// --------------------------------------------------------------------------------------------
mat4 vkClass::GetViewMatrix(UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{

	switch (projection)
	{
	default:
	case PROJECTION_PERSPECTIVE:

		if (camera == CAMERA_NORMAL)
			return ((GLcameraClass*)gl_Camera)->m_viewMatrix;

		break;

#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES
	case PROJECTION_ORTHOGRAPH:
		return ((GLcameraClass*)gl_Camera)->m_viewmatrix2D;
#endif
	}

	ASSERT(FALSE);
	return mat4();
}

mat4 vkClass::GetProjectionMatrix(UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
#if defined USE_SKY_CAMERA_DOME
	if (camera == CAMERA_SKY)
		return m_projectionMatrix_sky;
#endif

	switch (projection)
	{
	default:
	case PROJECTION_PERSPECTIVE:
		return m_projectionMatrix;

#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	case PROJECTION_ORTHOGRAPH:
		return m_orthoMatrix;
#endif
	}

	ASSERT(false);
	return m_projectionMatrix;
}

// --------------------------------------------------------------------------------------------
void vkClass::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
// --------------------------------------------------------------------------------------------
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}

// --------------------------------------------------------------------------------------------
VkCommandBuffer vkClass::BeginSingleTimeCommands()
// --------------------------------------------------------------------------------------------
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VK_CHECK(vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer));

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

	return commandBuffer;
}

// --------------------------------------------------------------------------------------------
void vkClass::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
// --------------------------------------------------------------------------------------------
{
	VK_CHECK(vkEndCommandBuffer(commandBuffer));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VK_CHECK(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(m_graphicsQueue));

	vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}

// --------------------------------------------------------------------------------------------
// Immutable Buffer Upload (DEVICE_LOCAL via staging)
// DX11 equivalent: D3D11_USAGE_IMMUTABLE
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
bool vkClass::CreateAndUploadBuffer(
	VkDeviceSize size,
	const void* srcData,
	VkBufferUsageFlags finalUsage,
	VkBuffer& dstBuffer,
	VkDeviceMemory& dstMemory)
	// --------------------------------------------------------------------------------------------
{
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VkDeviceMemory stagingMemory = VK_NULL_HANDLE;

	// ----------------------------------------------------------
	// STAGING BUFFER (HOST_VISIBLE)
	// ----------------------------------------------------------
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_CHECK(vkCreateBuffer(m_device, &bufferInfo, nullptr, &stagingBuffer));

	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(m_device, stagingBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex =
		FindMemoryType(memRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VK_CHECK(vkAllocateMemory(m_device, &allocInfo, nullptr, &stagingMemory));
	VK_CHECK(vkBindBufferMemory(m_device, stagingBuffer, stagingMemory, 0));

	void* mapped = nullptr;
	VK_CHECK(vkMapMemory(m_device, stagingMemory, 0, size, 0, &mapped));
	memcpy(mapped, srcData, (size_t)size);
	vkUnmapMemory(m_device, stagingMemory);

	// ----------------------------------------------------------
	// FINAL BUFFER (DEVICE_LOCAL)
	// ----------------------------------------------------------
	bufferInfo.usage = finalUsage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	VK_CHECK(vkCreateBuffer(m_device, &bufferInfo, nullptr, &dstBuffer));

	vkGetBufferMemoryRequirements(m_device, dstBuffer, &memRequirements);

	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VK_CHECK(vkAllocateMemory(m_device, &allocInfo, nullptr, &dstMemory));
	VK_CHECK(vkBindBufferMemory(m_device, dstBuffer, dstMemory, 0));

	// ----------------------------------------------------------
	// GPU COPY
	// ----------------------------------------------------------
	CopyBuffer(stagingBuffer, dstBuffer, size);

	vkDestroyBuffer(m_device, stagingBuffer, nullptr);
	vkFreeMemory(m_device, stagingMemory, nullptr);

	return true;
}

bool vkClass::CreateVulkanSampler(void* pContext, bool wrap)
{
	vkClass* driver = (vkClass*)m_Driver;
	VkDevice vkDevice = driver->m_device;

	VkSamplerCreateInfo sampler{};
	sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler.magFilter = VK_FILTER_LINEAR;
	sampler.minFilter = VK_FILTER_LINEAR;
	sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	if (wrap)
	{
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	}
	else
	{
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	}

	sampler.mipLodBias = 0.0f;
	sampler.compareOp = VK_COMPARE_OP_NEVER;
	sampler.minLod = 0.0f;
	sampler.maxLod = VK_LOD_CLAMP_NONE; // 0.0f;

	if (WOMA::AppSettings->MSAA_Anisotropic)
	{
		sampler.maxAnisotropy = (float)WOMA::AppSettings->MSAA_AnisotropicLevel;
		sampler.anisotropyEnable = VK_TRUE;
	}
	else
	{
		sampler.maxAnisotropy = 1.0f;
		sampler.anisotropyEnable = VK_FALSE;
	}

	sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	return vkCreateSampler(vkDevice, &sampler, nullptr, &m_vkSampler[wrap]) == VK_SUCCESS;
}

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
// ----------------------------------------------------------------------------------------------
ImageLoaderClass* vkClass::CaptureScreenShot(int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	return false;
}
#endif

#pragma warning(pop)
#endif
