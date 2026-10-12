// --------------------------------------------------------------------------------------------
// Filename: TextureSampling.hlsli
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
*   Woma engine unified texture sampling library (Nearest / Bilinear / Trilinear / Cubic)
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

// Return texture dimensions
float2 functiongetTextureWidth(Texture2D tex)
{
    uint width_texels;
    uint height_texels;
    tex.GetDimensions(width_texels, height_texels);
    return float2(width_texels, height_texels);
}

// Catmull-Rom interpolation helper
float4 catmullRom(float4 p0, float4 p1, float4 p2, float4 p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    return 0.5 * (
        (2.0 * p1) +
        (-p0 + p2) * t +
        (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3) * t2 +
        (-p0 + 3.0 * p1 - 3.0 * p2 + p3) * t3
    );
}

//
// NEAREST INTERPOLATION
//
float4 NearestInterpolation(SamplerState smp, Texture2D tex, float2 uv)
{
    // Sample uses hardware nearest mode based on sampler state
    return tex.SampleLevel(smp, uv, 0);
}

//
// BILINEAR INTERPOLATION
//
//14372 FPS
float4 BilinearInterpolation(SamplerState smp, Texture2D tex, float2 texCoords)
{
    float2 texSize = functiongetTextureWidth(tex);
    
    // Calculate the integer and fractional parts of the texture coordinates
    float2 texelSize = 1.0f / texSize; // Size of each texel
    float2 baseCoords = texCoords * texSize; // Scale texCoords to the texture size
    float2 fcoords = frac(baseCoords); // Fractional part (for interpolation)
    int2 icoords = int2(floor(baseCoords)); // Integer part (for sampling texels)

    // Fetch four surrounding texels
    float4 topLeft = tex.Load(int3(icoords.x, icoords.y, 0));
    float4 topRight = tex.Load(int3(icoords.x + 1, icoords.y, 0));
    float4 bottomLeft = tex.Load(int3(icoords.x, icoords.y + 1, 0));
    float4 bottomRight = tex.Load(int3(icoords.x + 1, icoords.y + 1, 0));

    // Perform bilinear interpolation
    float4 top = lerp(topLeft, topRight, fcoords.x); // Interpolate top row
    float4 bottom = lerp(bottomLeft, bottomRight, fcoords.x); // Interpolate bottom row
    return lerp(top, bottom, fcoords.y); // Interpolate between the top and bottom rows
}


//
// TRILINEAR INTERPOLATION (mip-based)
//
//14673 FPS
float4 TrilinearInterpolation(SamplerState smp, Texture2D tex, float2 texCoords, float mipLevel)
{
    float2 texSize = functiongetTextureWidth(tex);
    
    // Calculate the integer and fractional parts of the texture coordinates
    float2 texelSize = 1.0f / texSize; // Size of each texel
    float2 baseCoords = texCoords * texSize; // Scale texCoords to the texture size
    float2 fcoords = frac(baseCoords); // Fractional part (for interpolation)
    int2 icoords = int2(floor(baseCoords)); // Integer part (for sampling texels)

    // Clamp mipLevel between 0 and max mip level
    mipLevel = clamp(mipLevel, 0.0f, functiongetTextureWidth(tex).x); // Assuming square texture for simplicity

    // Fetch the texels from the texture at two different mipmap levels
    // Mipmap level is chosen based on mipLevel
    float4 texel00 = tex.SampleLevel(smp, texCoords, mipLevel);
    float4 texel01 = tex.SampleLevel(smp, texCoords, mipLevel + 1.0f);
    
    // Perform bilinear interpolation for each of the two levels
    float4 top = lerp(texel00, texel01, fcoords.x);
    float4 bottom = lerp(texel00, texel01, fcoords.y);
    
    // Perform the final interpolation between the two levels
    return lerp(top, bottom, mipLevel);
}


//
// CUBIC (CATMULL-ROM) INTERPOLATION (Higher-quality filtering for special effects)
//
float4 CubicInterpolation(SamplerState smp, Texture2D tex, float2 uv)
{
    float2 texSize = functiongetTextureWidth(tex);
    float2 px = 1.0 / texSize;

    // Convert to pixel grid
    float2 uvGrid = uv * texSize;
    float2 base = floor(uvGrid - 0.5);
    float2 f = frac(uvGrid - 0.5);

    // 4 rows × 4 columns
    float4 rows[4];

    // For j = -1 to 2 -> 4 vertical offsets
    [unroll]
    for (int j = -1; j <= 2; ++j)
    {
        float2 vUV = (base + float2(0, j)) / texSize;

        float4 c0 = tex.SampleLevel(smp, vUV - float2(px.x, 0), 0);
        float4 c1 = tex.SampleLevel(smp, vUV, 0);
        float4 c2 = tex.SampleLevel(smp, vUV + float2(px.x, 0), 0);
        float4 c3 = tex.SampleLevel(smp, vUV + float2(2 * px.x, 0), 0);

        rows[j + 1] = catmullRom(c0, c1, c2, c3, f.x);
    }

    // Vertical Catmull-Rom
    return catmullRom(rows[0], rows[1], rows[2], rows[3], f.y);
}
