#version 450

// --------------------------------------------------------------------------------------------
// Filename: 050TextureFog.vert
// WoMA Engine 3D - Vulkan Level 50
// Equivalent to DX: 051Texture.hlsl (Texture + Fog)
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// VERTEX INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float fragFogFactor;


// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::vkVSconstantBufferType exactly.
// Do not reorder fields.
// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER
// --------------------------------------------------------------------------------------------
// Same layout used by previous Vulkan levels
layout(set = 0, binding = 0) uniform UBO
{
    mat4	WVP;
	mat4	WV;
	int		VShasFog;
	float	VSfogStart;
	float	VSfogEnd;
	int		Pad1;
} cb;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);

    gl_Position = cb.WVP * pos;
	gl_Position.y = -gl_Position.y;
    fragTexCoord = inTexCoord;

    if (cb.VShasFog > 0)
    {
        vec4 cameraViewPosition = cb.WV * pos;
        fragFogFactor = clamp((cb.VSfogEnd - cameraViewPosition.z) / (cb.VSfogEnd - cb.VSfogStart), 0.0, 1.0);
    }
    else
        fragFogFactor = 1.0;
}