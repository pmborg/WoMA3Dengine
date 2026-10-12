// --------------------------------------------------------------------------------------------
// Filename: GLtextureClass.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// PURPOSE:
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "platform.h"

#if (defined OPENGL3 || defined OPENGL4 || defined VULKAN) && DX_ENGINE_LEVEL >= 22
#include "OSengine.h"
#include "womadriverclass.h"
#include "GLopenGLclass.h"
#include "glTextureclass.h"
#include "mem_leak.h"
#if defined VULKAN
#include "vkClass.h"
#endif

#include "fileLoader.h"

#if defined ANDROID_PLATFORM
#if defined GLES2
#include "C:\WoMA3Dengine\ThirdParty\GL3Plus\GL\glcorearb.h"
#endif

#include "defs.h"
#include "AndroidEngine.h"
#endif

GLtextureClass::GLtextureClass()
{
	CLASSLOADER();
	m_textureID = NULL;
	loaded = false;
}

GLtextureClass::~GLtextureClass() 
{
	Shutdown();
	CLASSDELETE();
}

bool GLtextureClass::Initialize(void* pContext, TCHAR* filename, unsigned int textureUnit, bool wrap)
{
	// Load the Image file.
	STRING filename_ = filename;

	if (loaded)
		return true;

	loaded = loadImage(pContext, (TCHAR*)filename_.c_str(), wrap);
	ASSERT(loaded);

	womalog("[CONTEXT] GLtextureClass::Initialize pContext = %p file=%s\n", pContext, filename);
	return loaded;
}

void GLtextureClass::Shutdown()
{
	// If the texture was loaded then make sure to release it on shutdown.
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3 && loaded && m_textureID)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = NULL;
		loaded = false;
	}

#if defined VULKAN
	vkClass* driver = (vkClass*)m_Driver;
	if (driver && WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		VkDevice vkDevice = driver->m_device;

		vkDeviceWaitIdle(driver->m_device);

		if (m_vkImageView)	{ vkDestroyImageView(driver->m_device, m_vkImageView, nullptr); m_vkImageView = VK_NULL_HANDLE; }
		if (m_vkImage)		{ vkDestroyImage(driver->m_device, m_vkImage, nullptr); m_vkImage = VK_NULL_HANDLE; }
		if (m_vkImageMemory){ vkFreeMemory(driver->m_device, m_vkImageMemory, nullptr); m_vkImageMemory = VK_NULL_HANDLE; }
		if (m_vkStagingBuffer) { vkDestroyBuffer(driver->m_device, m_vkStagingBuffer, nullptr); m_vkStagingBuffer = VK_NULL_HANDLE; }
		if (m_vkStagingMemory) { vkFreeMemory(driver->m_device, m_vkStagingMemory, nullptr); m_vkStagingMemory = VK_NULL_HANDLE; }
	}
#endif
}

#pragma warning( push )
#pragma warning( disable : 4996 )
bool GLtextureClass::loadImage(void* pContext, TCHAR *fileName, bool wrap)
{
	unsigned char* pixels = NULL;  // Image in 32bits pixels
	UINT imageSize;
	int decodeResult=0;

	womalog(TEXT("[%d]: GLtextureClass::loadImage(%s)\n"), gettid(), fileName);

#if defined ANDROID_PLATFORM_
	//C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Debug\Package\assets\engine\data
	//C:\WoMAengine2023\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft PNG seafloor.dds
/*
	* -JPEG
	* -PNG
	* -GIF
	* -WebP
	* -BMP
	* -ICO
	* -WBMP
	* -HEIF
*/

	AAssetManager* assetManager = engine_state.app->activity->assetManager;
	auto spAndroidRobotTexture = TextureAsset::loadAsset(assetManager, fileName);

#else
	const TCHAR *extension = _tcsrchr(fileName, '.');
	if (extension == NULL) return false;

#if defined USE_IMAGE_JPG
	if (_tcsicmp(extension, TEXT(".jpg")) == 0 || _tcsicmp(extension, TEXT(".jpeg")) == 0)
	{
		pixels = ImageLoader.loadJPEG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_PNG
	if (_tcsicmp(extension, TEXT(".png")) == 0)
	{
		pixels = ImageLoader.loadPNG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_TGA
	if (_tcsicmp(extension, TEXT(".tga")) == 0)
	{
		pixels = ImageLoader.loadTGA(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_BMP
	if (_tcsicmp(extension, TEXT(".bmp")) == 0)
	{
		pixels = ImageLoader.loadBMP(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_TIFF
	if (_tcsicmp(extension, TEXT(".tif")) == 0)
	{
		pixels = ImageLoader.loadTIF(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_DDS
	if (_tcsicmp(extension, TEXT(".dds")) == 0)
	{
		pixels = ImageLoader.loadDDS(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
	{
		WomaFatalException ("IMAGE: Format not supported!");
		return false;
	}

	if (!pixels)
		return false;

#ifdef VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
		AddTextureVulkan(pContext, pixels, width, height, wrap, imageSize);
	else
#endif
		AddTexture(pixels, width, height, wrap);
	
	SAFE_DELETE_ARRAY(pixels);	// Release the image loaded from file
#endif

	return true;
}
#pragma warning( pop )

void GLtextureClass::AddTexture(unsigned char* image32b, int width, int height, bool wrap)
{
	// Create the texture

	GLenum  err;
	//glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &m_textureID);				// Generate an ID for the texture.
	err = glGetError(); if (err != GL_NO_ERROR) {womalog(TEXT("glGenTextures err: %04x"), err);}
	glBindTexture(GL_TEXTURE_2D, m_textureID);	// Bind the texture as a 2D texture.
	err = glGetError(); if (err != GL_NO_ERROR) {womalog(TEXT("glBindTexture err: %04x"), err);}

	//32bits
#if defined WINDOWS_PLATFORM
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image32b); // Load/Copy the image data into the texture unit: WINDOWS/ANDROID
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image32b);	// LINUX & ANDROID
#endif 

	// Set the texture color to either wrap around or clamp to the edge.
	if (wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
	#if defined WINDOWS_PLATFORM
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#endif
	}

	// Set the texture filtering: Anisotropic filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);		// Generate mipmaps for the texture.
	err = glGetError(); 
	if (err != GL_NO_ERROR) 
		{ womalog(TEXT("ERROR! glGenerateMipmap err: %04x"), err); }
}

#ifdef VULKAN


//
//https://gitverse.ru/auroraos/mirror_Vulkan/content/master/examples/texture/texture.cpp
//

bool GLtextureClass::AddTextureVulkan(void* pContext, unsigned char* image32b, int width, int height, bool wrap, UINT imageSize)
{
	vkClass* driver = (vkClass*)m_Driver;
	VkDevice vkDevice = ((vkClass*)m_Driver)->m_device;
	VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
	
	m_wrap = wrap;

	//TODO....
	// 1. staging buffer
	// 2. VkImage
	// 3. copy pixels -> image
	// 4. layout transition
	// 5. image view
	// 6. sampler

	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = imageSize;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult vkResult = vkCreateBuffer(vkDevice, &bufferCreateInfo, nullptr, &m_vkStagingBuffer);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkCreateBuffer")); }

	// Get memory requirements for the staging buffer (alignment, memory type bits)
	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(vkDevice, m_vkStagingBuffer, &memReqs);

	VkMemoryAllocateInfo memAllocInfo = {};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	memAllocInfo.allocationSize = memReqs.size;

	// Get memory type index for a host visible buffer
	memAllocInfo.memoryTypeIndex = driver->FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkResult = vkAllocateMemory(vkDevice, &memAllocInfo, nullptr, &m_vkStagingMemory);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkAllocateMemory")); }

	vkResult = vkBindBufferMemory(vkDevice, m_vkStagingBuffer, m_vkStagingMemory, 0);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkBindBufferMemory")); }

	// Copy texture data into host local staging buffer
	uint8_t* data;
	vkResult = vkMapMemory(vkDevice, m_vkStagingMemory, 0, memReqs.size, 0, (void**)&data);


	memcpy(data, image32b, imageSize);
	vkUnmapMemory(vkDevice, m_vkStagingMemory);

	// Setup buffer copy regions for each mip level
	std::vector<VkBufferImageCopy> bufferCopyRegions;
	uint32_t offset = 0;

		// Setup a buffer image copy structure for the current mip level
		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.mipLevel = 0; //i;
		bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = width;
		bufferCopyRegion.imageExtent.height = height;
		bufferCopyRegion.imageExtent.depth = 1;
		bufferCopyRegion.bufferOffset = offset;
		bufferCopyRegions.push_back(bufferCopyRegion);

	// Create optimal tiled target image on the device
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = format;
	imageCreateInfo.mipLevels = 1; // texture.mipLevels;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// Set initial layout of the image to undefined
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.extent = { (UINT)width, (UINT)height, 1 };
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	vkResult = vkCreateImage(vkDevice, &imageCreateInfo, nullptr, &m_vkImage);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkCreateImage")); }

	vkGetImageMemoryRequirements(vkDevice, m_vkImage, &memReqs);
	memAllocInfo.allocationSize = memReqs.size;
	memAllocInfo.memoryTypeIndex = driver->FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	vkResult = vkAllocateMemory(vkDevice, &memAllocInfo, nullptr, &m_vkImageMemory);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkAllocateMemory")); }

	vkResult = vkBindImageMemory(vkDevice, m_vkImage, m_vkImageMemory, 0);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkBindImageMemory")); }

	VkCommandBuffer copyCmd = (VkCommandBuffer)pContext;
	womalog("AddTextureVulkan() copyCmd=%p\n", copyCmd);

	// Image memory barriers for the texture image

	// The sub resource range describes the regions of the image that will be transitioned using the memory barriers below
	VkImageSubresourceRange subresourceRange = {};
	// Image only contains color data
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	// Start at first mip level
	subresourceRange.baseMipLevel = 0;
	// We will transition on all mip levels
	subresourceRange.levelCount = 1; // texture.mipLevels;
	// The 2D texture only has one layer
	subresourceRange.layerCount = 1;

	// Transition the texture image layout to transfer target, so we can safely copy our buffer data to it.
	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.image = m_vkImage;
	imageMemoryBarrier.subresourceRange = subresourceRange;
	imageMemoryBarrier.srcAccessMask = 0;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

	// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition
	// Source pipeline stage is host write/read execution (VK_PIPELINE_STAGE_HOST_BIT)
	// Destination pipeline stage is copy command execution (VK_PIPELINE_STAGE_TRANSFER_BIT)
	vkCmdPipelineBarrier(
		copyCmd,
		VK_PIPELINE_STAGE_HOST_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &imageMemoryBarrier);

	// Copy mip levels from staging buffer
	vkCmdCopyBufferToImage(
		copyCmd,
		m_vkStagingBuffer,
		m_vkImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		static_cast<uint32_t>(bufferCopyRegions.size()),
		bufferCopyRegions.data());

	// Once the data has been uploaded we transfer to the texture image to the shader read layout, so it can be sampled from
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition
	// Source pipeline stage is copy command execution (VK_PIPELINE_STAGE_TRANSFER_BIT)
	// Destination pipeline stage fragment shader access (VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
	vkCmdPipelineBarrier(
		copyCmd,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &imageMemoryBarrier);

	//
	// CREATE IMAGE VIEW
	//
	VkImageViewCreateInfo view = {};
	view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view.format = format;

	// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
	// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
	view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	view.subresourceRange.baseMipLevel = 0;
	view.subresourceRange.baseArrayLayer = 0;
	view.subresourceRange.layerCount = 1;

	// Linear tiling usually won't support mip maps
	// Only set mip map count if optimal tiling is used
	view.subresourceRange.levelCount = 1; // (useStaging) ? texture.mipLevels : 1;

	// The view will be based on the texture's image
	view.image = m_vkImage;
	vkResult=vkCreateImageView(vkDevice, &view, nullptr, &m_vkImageView);
	if (vkResult != VK_SUCCESS)
		{ WomaFatalException(("Could not create vkCreateImageView")); }

	return true;
}
#endif

// textureFactory.cpp / textureFactory.h
GLtextureClass* GetOrCreateTexture(WomaDriverClass* driver, const STRING& filename, void* pContext, bool Model3D)
{
	// Already loaded?
	for (UINT i = 0; i < driver->g_allTextureNameArray.size(); i++)
	{
		if (driver->g_allTextureNameArray[i] == filename)
			return static_cast<GLtextureClass*>(driver->g_allTexturePointerArray[i]);
	}

	// First time: create and register.
	GLtextureClass* texture = NEW GLtextureClass;

	// Initialize the texture object: ###: AddTexture(pixels, width, height, wrap);
	TCHAR* file = (TCHAR*)(&filename)[0].c_str();
	#if defined WINDOWS_PLATFORM
		bool result = texture->Initialize(/*NULL*/ pContext, WOMA::LoadFile(file), 0, /*wrap*/ Model3D);
	#else
		bool result = texture->Initialize(/*NULL*/ pContext, file, 0, /*wrap*/ Model3D);
	#endif
	if(!result) { 
		WomaMessageBox(file, TEXT("GLmodelClass: Texture File not found"));
		ASSERT(false); 
	}

	driver->g_allTextureNameArray.push_back(filename);
	driver->g_allTexturePointerArray.push_back(texture);

	return texture;
}

#endif
