////////////////////////////////////////////////////////////////////////////////
// Filename: texture.vs
////////////////////////////////////////////////////////////////////////////////
#version 400

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 inputPosition;
in vec2 inputTexCoord;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec2 texCoord;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
//uniform mat4 worldMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 projectionMatrix;

uniform mat4 WVP;

vec4 column0 = vec4(1.24942219f, 0.000000001f, 0.000000002f, 0.000000003f);
vec4 column1 = vec4(0.000000006f, 2.35672379f, 0.523716450f, -0.523716271f);
vec4 column2 = vec4(0.000000007f, -0.216972843f, 0.976377785f, 8.14494896f);
vec4 column3 = vec4(0.000000008f, -0.216930464f, 0.976187050f, 8.24335766f);
mat4 WVP_ = mat4(column0, column1, column2, column3); // sets columns of matrix n


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	//gl_Position = worldMatrix * vec4(inputPosition, 1.0f);
	//gl_Position = viewMatrix * gl_Position;
	//gl_Position = projectionMatrix * gl_Position;

	//OP1:
	gl_Position = WVP * vec4(inputPosition, 1.0f);

	//OP2: DEBUG
	//gl_Position = transpose(WVP_) * vec4(inputPosition, 1.0f);

	// Store the texture coordinates for the pixel shader.
	texCoord = inputTexCoord;
}