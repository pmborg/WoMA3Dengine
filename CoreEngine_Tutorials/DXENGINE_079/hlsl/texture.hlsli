float2 functiongetTextureWidth(Texture2D tex)
{
    uint width_texels;
    uint height_texels;
    tex.GetDimensions(width_texels, height_texels);
    return float2(width_texels, height_texels);
}

// Bilinear interpolation function for a 2D texture
//14372 FPS
float4 BilinearInterpolation(Texture2D tex, float2 texCoords)
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

// Trilinear interpolation function for a 2D texture with mipmaps
//14673 FPS
float4 TrilinearInterpolation(Texture2D tex, float2 texCoords, float mipLevel)
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
    float4 texel00 = tex.SampleLevel(SampleType, texCoords, mipLevel);
    float4 texel01 = tex.SampleLevel(SampleType, texCoords, mipLevel + 1.0f);
    
    // Perform bilinear interpolation for each of the two levels
    float4 top = lerp(texel00, texel01, fcoords.x);
    float4 bottom = lerp(texel00, texel01, fcoords.y);
    
    // Perform the final interpolation between the two levels
    return lerp(top, bottom, mipLevel);
}


// ------
// USAGE:
// ------

float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel)
{
#if defined PS_USE_BilinearInterpolation
        return BilinearInterpolation(tex, texCoords);
#elif defined PS_USE_TrilinearInterpolation
        return TrilinearInterpolation(tex, texCoords, mipLevel);
#else
    return tex.Sample(SampleType, texCoords);
#endif  
}

#if defined PS_USE_BilinearInterpolation || defined PS_USE_TrilinearInterpolation
	//float4 textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0);
#else
	//Normal:
	//float4 textureColor = shaderTexture.Sample(SampleType, input.texCoords)
#endif
