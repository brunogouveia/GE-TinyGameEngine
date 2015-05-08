#version 400 core

//  CameraInfo
uniform CameraInfos {
	// Projection Matrix
	mat4 ProjectionMatrix;
	// Model Matrix
	mat4 ModelMatrix;
	// View Matrix
	mat4 ViewMatrix;
	// Normal matrix;
	mat4 NormalMatrix;
} cameraInfo;

// Material
uniform Material {
	// Emissive
	vec4 emissive;
	// Ambient
	vec4 ambient;
	// Diffuse
	vec4 diffuse;
	// Specular
	vec4 specular;
} material;

uniform Light {
	// Position
	vec4 position;
	
	// Colors
	vec4 diffuse;
	vec4 specular;
} light;

//  Shadows matrices
uniform Shadows {
	// Projection Matrix
	mat4 depthMVP;
	// Model Matrix
	mat4 DepthBiasMVP;
	float zNear;
	float zFar;
} shadows;

//  Vertex attributes (input)
layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 TextCoord;

//  Output to next shader
out vec3 IPosition;
out vec3 INormal;
out vec3 FrontColor;
out vec2 ITextCoord;

void main()
{ 
	//  Pass all attributes to fragment shader (will be interpolated)
	vec4 P = cameraInfo.ViewMatrix * cameraInfo.ModelMatrix * Vertex;
	IPosition = P.xyz/P.w;
	INormal = normalize(mat3(cameraInfo.NormalMatrix) * Normal);
	FrontColor = Color;
	ITextCoord = TextCoord;

	gl_Position =  cameraInfo.ProjectionMatrix * cameraInfo.ViewMatrix * cameraInfo.ModelMatrix  * Vertex;
}