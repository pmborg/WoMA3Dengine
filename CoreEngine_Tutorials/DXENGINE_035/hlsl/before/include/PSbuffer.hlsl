//Note: on DX11 dont use: register()
cbuffer PSShaderParametersBuffer// SYNC: DXshaderClass.h
{
	// BLOCK1:
	float4	pixelColor;
	
	// BLOCK2:
	bool	hasTexture;		// No? Use pixelColor, then.
	bool    hasLight;		// Future Load Obj. Engine Level
	bool	hasSpecular;	// Future Load Obj. Engine Level
	bool	isFont;			// Future Load Obj. Engine Level
	
	// BLOCK3:
	float4	ambientColor;	// LIGHT: Ka
	float4	diffuseColor;	// LIGHT: Kd
	float4	emissiveColor;	// LIGHT: Ke 
	float4	lightDirection;	// LIGHT

	// BLOCK4:
	bool	hasColorMap;		// 66
	float	lightType;			// Future
	float	shaderType;			// Future
	float	shaderTypeParameter;// Future

	// BLOCK5:
	bool	hasAlfaColor;
	float	alfaColor;
	float	fade;			// Fade from 0 to 1
	float	frameTime;		// For animations

	// BLOCK6:
	bool	hasFog;
	bool	isSky;
	bool    hasAlfaMap;
	bool	hasNormMap;

	// BLOCK7:
	float3	cameraPosition;	// NOT USED!
	bool	castShadow;
	float3	specularColor;
	float	nShininess;
};