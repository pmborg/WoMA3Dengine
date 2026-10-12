// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan
// TEXTURE SAMPLING
// Since Level 23 currently uses the normal hardware sampler.
// --------------------------------------------------------------------------------------------

// IMPORTANT:
// Do not replace hardware sampling with texelFetch()/manual filtering
// for LEVEL33. Vulkan samplers already provide Wrap/Clamp and filtering.
// Manual filtering is reserved for future Cubic interpolation.

vec4 catmullRom(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t)
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

vec4 NearestInterpolation(sampler2D tex, vec2 uv)
{
    ivec2 texSize = textureSize(tex, 0);

    ivec2 texel = ivec2(
        floor(uv * vec2(texSize))
    );

    texel = clamp(texel, ivec2(0), texSize - ivec2(1));

    return texelFetch(tex, texel, 0);
}

vec4 BilinearInterpolation(sampler2D tex, vec2 uv)
{
    ivec2 texSize = textureSize(tex, 0);

    vec2 coord = uv * vec2(texSize) - 0.5;

    ivec2 base = ivec2(floor(coord));
    vec2 f = fract(coord);

    ivec2 p00 = clamp(base + ivec2(0, 0), ivec2(0), texSize - ivec2(1));

    ivec2 p10 = clamp(base + ivec2(1, 0), ivec2(0), texSize - ivec2(1));

    ivec2 p01 = clamp(base + ivec2(0, 1), ivec2(0), texSize - ivec2(1));

    ivec2 p11 = clamp(base + ivec2(1, 1), ivec2(0), texSize - ivec2(1));

    vec4 c00 = texelFetch(tex, p00, 0);
    vec4 c10 = texelFetch(tex, p10, 0);
    vec4 c01 = texelFetch(tex, p01, 0);
    vec4 c11 = texelFetch(tex, p11, 0);

    vec4 cx0 = mix(c00, c10, f.x);
    vec4 cx1 = mix(c01, c11, f.x);

    return mix(cx0, cx1, f.y);
}

vec4 TrilinearInterpolation(sampler2D tex, vec2 texCoords, uint mipLevel)
{
    vec2 texSize = vec2(textureSize(tex, 0));

    vec2 baseCoords = texCoords * texSize;
    vec2 fcoords = fract(baseCoords);

    // Same DX logic
    float mip = clamp(float(mipLevel), 0.0, texSize.x);

    vec4 texel00 = textureLod(tex, texCoords, mip);
    vec4 texel01 = textureLod(tex, texCoords, mip + 1.0);

    vec4 top = mix(texel00, texel01, fcoords.x);
    vec4 bottom = mix(texel00, texel01, fcoords.y);

    return mix(top, bottom, mip);
}


vec4 CubicInterpolation(sampler2D tex, vec2 uv)
{
    vec2 texSize = vec2(textureSize(tex, 0));
    vec2 px = 1.0 / texSize;

    vec2 uvGrid = uv * texSize;
    vec2 base = floor(uvGrid - 0.5);
    vec2 f = fract(uvGrid - 0.5);

    vec4 rows[4];

    for (int j = -1; j <= 2; ++j)
    {
        vec2 vUV = (base + vec2(0.0, float(j))) / texSize;

        vec4 c0 = textureLod(tex, vUV - vec2(px.x, 0.0), 0.0);

        vec4 c1 = textureLod(tex, vUV, 0.0);

        vec4 c2 = textureLod(tex, vUV + vec2(px.x, 0.0), 0.0);

        vec4 c3 = textureLod(tex, vUV + vec2(2.0 * px.x, 0.0), 0.0);

        rows[j + 1] = catmullRom(c0, c1, c2, c3, f.x);
    }

    return catmullRom(rows[0], rows[1], rows[2], rows[3], f.y);
}

vec4 GetShaderTexture(sampler2D tex, vec2 texCoords, uint mipLevel, int mode)
{
    switch (mode)
    {
        case 0:
            return NearestInterpolation(tex, texCoords);

        case 1:
            return BilinearInterpolation(tex, texCoords);

        case 2:
            return TrilinearInterpolation(tex, texCoords, mipLevel);

        case 3:
            return CubicInterpolation(tex, texCoords);

        default:
            return texture(tex, texCoords);
    }
}

// --------------------------------------------------------------------------------------------
// Hardware texture sampling.
// The Vulkan sampler created in C++ already controls:
//
//   - Wrap / Clamp / Mirror
//   - Nearest / Bilinear
//   - Trilinear (when mipmaps exist)
//   - Anisotropic filtering
//
// Only Cubic filtering requires a custom shader implementation.
// --------------------------------------------------------------------------------------------
// LEVEL33 NOTE:
// Modes 0,1,2 and 4 use Vulkan hardware texture sampling.
// Wrap/Clamp, Bilinear, Trilinear and Anisotropic filtering
// are controlled by the VkSampler created in C++.
// Only Cubic filtering requires a custom shader implementation.
vec4 GetShaderTextureSimple(sampler2D tex, vec2 uv, float mipLevel, int mode)
{
    switch (mode)
    {
        case 3: // Cubic (future/custom implementation)
            return CubicInterpolation(tex, uv);

        default:
            // Modes 0,1,2,4 use the hardware sampler.
            return texture(tex, uv);
    }
}

/*
// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc1(vec3 Normal) { return vec4(max(dot(Normal, ps.lightDirection.xyz), 0.0)); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc2(vec3 Normal) { return vec4(max(dot(Normal, -ps.lightDirection.xyz), 0.0)); }


// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
float PSlightFunc1(vec3 normal) { return clamp(dot(normal, -ps.lightDirection.xyz), 0.0, 1.0); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
float PSlightFunc2(vec3 normal) { return clamp(dot(normal, ps.lightDirection.xyz), 0.0, 1.0); }
*/