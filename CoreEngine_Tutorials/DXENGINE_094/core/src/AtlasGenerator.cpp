// --------------------------------------------------------------------------------------------
// Filename: AtlasGenerator.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#include "woma_macros.h"
#include "fileLoader.h"
#include "AtlasGenerator.h"
#include "DirectXTex.h"
#include <filesystem>
#include <fstream>
#include <vector>

using namespace DirectX;
namespace fs = std::filesystem;

#if defined GENERATE_ATLAS_INTEGRATION_DDS
extern std::vector<STRING> billboardTextures;
#endif

#if defined GENERATE_ATLAS_INTEGRATION_DDS

HRESULT GenerateBillboardAtlasDDS(
	ID3D11Device* device,
	const std::vector<STRING>& texturePaths,
	ID3D11ShaderResourceView** outSRV,
	std::vector<AtlasRegion>& outRegions)
{
	if (!device || texturePaths.empty())
		return E_INVALIDARG;

	std::vector<ScratchImage> images;
	std::vector<const Image*> imgPtrs;

	// 1) Load billboard textures
	for (const auto& texPath : texturePaths)
	{
		ScratchImage img;
		WCHAR WtexPath[MAX_STR_LEN] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, WOMA::LoadFile((char*)texPath.c_str()), -1, WtexPath, MAX_STR_LEN);

		HRESULT hr = LoadFromWICFile(WtexPath, WIC_FLAGS_FORCE_SRGB, nullptr, img);
		if (FAILED(hr)) {
			womalog(TEXT("[ERROR] Failed to load: %s\n"), texPath.c_str());
			return hr;
		}
		imgPtrs.push_back(img.GetImage(0, 0, 0));
		images.push_back(std::move(img));
	}
	// 2) Compute atlas layout
	const UINT texCount = (UINT)images.size();

	// find max source dimension (cell size)
	UINT maxDim = 0;
	for (const auto& img : images) {
		auto md = img.GetMetadata();
		maxDim = MAX(maxDim, (UINT)MAX(md.width, md.height));
	}

	// grid & atlas side (power-of-two)
	UINT grid = (UINT)ceilf(sqrtf((float)texCount));
	UINT atlasSize = 1;
	while (atlasSize < grid * maxDim) atlasSize <<= 1;

	womalog(TEXT("[DEBUG] Generating atlas: %u x %u | %u textures\n"), atlasSize, atlasSize, texCount);

	// 3) Create atlas image (optionally with mips later)
	TexMetadata meta = images[0].GetMetadata();
	meta.width = atlasSize;
	meta.height = atlasSize;
	meta.arraySize = 1;
	meta.mipLevels = 1;

	ScratchImage atlas;
	HRESULT hr = atlas.Initialize(meta);
	ThrowIfFailed(hr);

	const Image* dst = atlas.GetImage(0, 0, 0);

	// 4) Pack each image in its cell; compute exact UVs by its own w/h
	outRegions.clear(); outRegions.reserve(texCount);

	// small half-texel inset to avoid bleeding between tiles:
	const float inset = 0.5f / float(atlasSize);

	for (UINT i = 0; i < texCount; ++i)
	{
		const Image* src = imgPtrs[i];
		UINT w = src->width;
		UINT h = src->height;

		// place at cell (gx, gy)
		UINT gx = i % grid;
		UINT gy = i / grid;
		UINT x = gx * maxDim;
		UINT y = gy * maxDim;

		// copy (no scaling)
		DirectX::Rect r(0, 0, w, h);
		hr = CopyRectangle(*src, r, *dst, TEX_FILTER_DEFAULT, x, y);
		ThrowIfFailed(hr);

		// UVs as offset + size (with a tiny inset to reduce bleeding)
		float u0 = (x + inset) / float(atlasSize);
		float v0 = (y + inset) / float(atlasSize);
		float u1 = (x + w - inset) / float(atlasSize);
		float v1 = (y + h - inset) / float(atlasSize);

		AtlasRegion reg;
		reg.offsetU = u0;
		reg.offsetV = v0;
		reg.scaleU = (u1 - u0);
		reg.scaleV = (v1 - v0);
		outRegions.push_back(reg);
	}

	// 5) Save DDS atlas
	hr = SaveToDDSFile(
		atlas.GetImages(),
		atlas.GetImageCount(),
		atlas.GetMetadata(),
		DDS_FLAGS_NONE,
		L"billboard_atlas.dds"
	);
	if (FAILED(hr)) {
		womalog(TEXT("[ERROR] Failed to save DDS atlas!\n"));
		return hr;
	}
	womalog(TEXT("[INFO] Billboard DDS atlas generated: billboard_atlas.dds\n"));

	// 6) Export PNG preview
#ifdef EXPORT_ATLAS_DEBUG_PNG
	hr = SaveToWICFile(
		atlas.GetImages(),
		atlas.GetImageCount(),
		WIC_FLAGS_NONE,
		GetWICCodec(WIC_CODEC_PNG),
		L"billboard_atlas_preview.png"
	);
	if (SUCCEEDED(hr)) {
		womalog(TEXT("[INFO] Billboard atlas PNG preview exported: billboard_atlas_preview.png\n"));
	}
#endif

	// 7) Create SRV for rendering
	hr = CreateShaderResourceView(
		device,
		atlas.GetImages(),
		atlas.GetImageCount(),
		atlas.GetMetadata(),
		outSRV
	);
	if (FAILED(hr)) {
		womalog(TEXT("[ERROR] Failed to create SRV for atlas.\n"));
		return hr;
	}
	return S_OK;
}

#endif

