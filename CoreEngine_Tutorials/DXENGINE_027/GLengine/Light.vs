////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////
#version 400

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 worldMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 projectionMatrix;
uniform mat4 WVP;

/////////////////////
// INPUT VARIABLES //
/////////////////////
layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec2 inputTexCoord;
layout (location = 2) in vec3 inputNormal;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec2 texCoord;
out vec3 normal;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	//gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition, 1.0);	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = WVP * vec4(inputPosition, 1.0);

	texCoord = inputTexCoord;							// Store the texture coordinates for the pixel shader.
	normal = mat3(worldMatrix) * inputNormal;			// Calculate the normal vector against the world matrix only.
	normal = normalize(normal);							// Normalize the normal vector.
}