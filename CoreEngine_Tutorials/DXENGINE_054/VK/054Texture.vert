#version 450

// --------------------------------------------------------------------------------------------
// Filename: 054Texture.vert
// World of Middle Age (WoMA) - Vulkan
// LEVEL 54: Water Movement
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT FROM VERTEX
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec4 inColor;

// --------------------------------------------------------------------------------------------
// FRAGMENT OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out float outFogFactor;
layout(location = 3) out vec3 outWorldPos;

// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::vkVSconstantBufferType exactly.
// Do not reorder fields.
// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER - Alinhamento Corrigido com offsets nativos DX11/std140
// --------------------------------------------------------------------------------------------
layout(binding = 0, std140) uniform VSShaderParametersBuffer
{
    // VS1: Matrizes (Alinhamento nativo de 64 bytes cada)
    mat4 worldMatrix;
    mat4 view;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

    // VS2: Flags Inteiras
    int VShasLight;
    int VShasSpecular;
    int VShasNormMap;
    int VShasFog;

    // VS3: Vetores de Iluminação
    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

    // VS4: Configurações de Fog e Shadow
    float VSfogStart;
    float VSfogEnd;
    int   VShasShadowMap;
    int   VS_USE_WVP;

    // VS5: Projeções Avançadas e Câmara
    mat4  ViewToLightProj;
    mat4  WorldInverseTranspose;
    vec3  VSCameraPosition;
	int   Pad1;

    // VS6: Rotações e Variável do Tempo (Animação da Água)
    float VSrotX;
    float VSrotY;
    float VSrotZ;
    float time;   // FIX: Trancado estritamente no offset 572

    // VS7: Tipos de Shaders e Preenchimento (PAD)
    float VSshaderType;
    int   vsIsSky;
    float vsPAD3;
    float vsPAD4;
} vs;



// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------
void main()
{
    vec4 Pos = vec4(inPosition, 1.0);

    float s_X = 0.0;
    float s_Y = 0.0;
    float s_Z = 0.0;

    // -------------------------------------------------------------------------
    // LEVEL 51: Fog
    // -------------------------------------------------------------------------
    vec4 cameraPosition = vs.WV * vec4(inPosition, 1.0);

    if (vs.VShasFog != 0)
        outFogFactor = clamp((vs.VSfogEnd - cameraPosition.z) /
                             (vs.VSfogEnd - vs.VSfogStart), 0.0, 1.0);
    else
        outFogFactor = 0.0;

    // -------------------------------------------------------------------------
    // Procedural UV
    // -------------------------------------------------------------------------
	float t = vs.time;

    outTexCoord = vec2(Pos.x * 150.0 / 1024.0, Pos.z * 150.0 / 1024.0);
	outTexCoord += vec2(t * 0.15, t * 0.04);

    // -------------------------------------------------------------------------
    // LEVEL 54: Water waves
    // -------------------------------------------------------------------------
    vec2 X0 = vec2(Pos.x, Pos.z);
	
    for (int i = 0; i < 3; i++)
    {
        vec2 K = vec2(1, float(i));			// K is a wave vector
		//vec2 K = normalize(vec2(1.0, float(i)));
        float w = 1.5f * (float(i) + 1);	// w is frequency
        float a = 0.0075f;					// a is amplitude
	
		float phase = dot(K, X0) - (w * t);
	
		// FORMULA: X = X0 - (K/k)(a*sin(K*X0 - w.t);
        vec2 X = X0 - K * a * sin(phase);
		
		// FORMULA: y = a * cos (K*X0 - w.t);
        float y = a * cos(phase);
	
        s_X += X.x  / 16.0f;
        s_Y += X.y  / 16.0f;
        s_Z += y; //water altitude
    }

	//X:  A posição final ou instantânea da partícula no instante (t).
	//X0: A posição de equilíbrio inicial (coordenada Lagrangiana) da partícula.
	//a:  A amplitude da perturbação ou da onda.
	//K:  Um número de onda específico associado à perturbação ou ao envelope da onda.
	//w:  A frequência angular da onda, que dita a velocidade de oscilação no tempo.
	//t:  O tempo.

    Pos.x = s_X;
    Pos.z = s_Y;
    Pos.y = s_Z;

    Pos *= 7.5;
    Pos.w /= 7.5;

    // Set world position AFTER all math, before projection
    outWorldPos = Pos.xyz;

    gl_Position = vs.WVP * Pos;

	// Vulkan clip-space coordinate inversion
	gl_Position.y = -gl_Position.y;	

    // Base water tint
    outColor = vec4(0.9);
    outColor += vec4(0.1, 0.2, 0.5, 1.0);
    outColor /= 1.75;
	outColor.a = 0.5f;

//	//DEBUG ONLY!!!!
//outColor = vec4(
//    fract(X0.x / 8.0),
//    fract(X0.y / 8.0),
//    0.0,
//    1.0);
}
