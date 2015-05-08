#version 400 core

#extension GL_NV_shadow_samplers_cube : enable

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

uniform GlobalLight {
	// Colors
	vec4 global;
	vec4 ambient;
} globalLight;

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
 

//  Input from previous shader
in vec3 IPosition;
in vec3 INormal;
in vec2 ITextCoord;
in vec4 PosModelCoord;

uniform sampler2D text;
uniform samplerCube depthText;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

vec4 phong()
{
	// Compute PosModelCoord in light coordinates
	vec4 ShadowCoord = shadows.DepthBiasMVP * PosModelCoord;
	ShadowCoord /= ShadowCoord.w;

	// float shadowFactor = 0.0;
	// for (float x = -1.5; x <= 1.5; ++x) {
	// 	for (float y = -1.5; y <= 1.5; ++y)	{
	// 		shadowFactor += textureProj(depthText, ShadowCoord + (1.0/2048.0)*vec4(x,y,0.0,0.0));
	// 	}
	// }
	// shadowFactor = shadowFactor/16.0;
	vec3 LightDirection = (vec3(PosModelCoord) - vec3(light.position));

	// return vec4((length(LightDirection)-1)/(49));
	// return vec4(light.position);
	// return vec4(texture(depthText, vec3(LightDirection.x,LightDirection.y,LightDirection.z)));

	if ((length(LightDirection)-1)/(49) < texture(depthText, LightDirection).r + 0.00001) {
	// if (textureProj(depthText, ShadowCoord) == 1.0 && length(ShadowCoord.xy - vec2(0.5)) <= 0.5) {
	// Position in eye coordinates
	vec3 pos = IPosition;

	// Normal in eye coordinates
	vec3 N = normalize(INormal);

	// Light vector
	vec3 L = normalize(vec3(cameraInfo.ViewMatrix * light.position) - pos);

	// Reflection vector
	vec3 R = reflect(-L, N);

	// View vector in eye coordinates
	vec3 V = normalize(-pos);

	// Diffuse light intensity
	float Id = max(0.0, dot(N, L));

	// Specular light intensity
	float Is = (Id > 0.0) ? pow(max(0.0, dot(R, V)), material.specular[3]) : 0.0;

	// return vec4(factor);
	return (Id*light.diffuse*material.diffuse + Is*light.specular*vec4(vec3(material.specular),1.0));
	} else {
		return vec4(0.0);
	}
}


void main()
{
	// Set color
	Fragcolor = phong();
}
