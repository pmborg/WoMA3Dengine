cbuffer VSShaderParametersBuffer// SYNC: DXshaderClass.h
{
	// BLOCK: VS1
	matrix	worldMatrix;	//worldMatrix
	matrix  WV;				//worldMatrix+viewMatrix
	matrix  WVP;			//worldMatrix+viewMatrix+projectionMatrix

	// 23 BLOCK: VS2
	bool	VShasLight;
	bool	VShasSpecular;
	bool	VShasNormMap;
	bool	VShasFog;

	// 23 BLOCK: VS3
    float3	VSlightDirection;	// LIGHT
	float   VSPad1;
	float4	VSambientColor;		// LIGHT
	float4	VSdiffuseColor;		// LIGHT
	float4	VSemissiveColor;	// LIGHT: Ke

	// 31 BLOCK: VS4
	float	VSfogStart;
	float	VSfogEnd;
	bool	VShasShadowMap; 
	float	VSpad2;

	// 45 BLOCK: VS5
	matrix	ViewToLightProj;
};