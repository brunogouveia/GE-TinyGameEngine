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

// uniform sampler2D text;
uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDepth;
uniform sampler2D gNoise;
uniform vec2 dimension;
uniform vec3 kernel[128];

const float uRadius = 0.8;
const float uPower = 2.0;

//  Input from previous shader
in vec3 IPosition;
in vec3 INormal;
in vec2 ITextCoord;

uniform sampler2D text;
uniform sampler2D depthText;

//  Fragment color
layout (location=0) out vec4 Fragcolor;


void main()
{
 //   	// Compute texture coordinates
	// vec2 coord = gl_FragCoord.xy/dimension;
	// // Compute position
	// vec3 origin = IPosition;
	// // Compute normal
	// vec3 n = normalize(INormal);
	
	// if (origin.z >= texture(gDepth, coord).r)
	// 	discard;
 //   	vec2 noiseTexCoords = vec2(textureSize(gDepth, 0)) / vec2(textureSize(gNoise, 0));
	// noiseTexCoords *= (gl_FragCoord.xy)/dimension;

	// // Compute the TBN matrix
	// vec3 rvec = normalize(vec3(texture(gNoise,noiseTexCoords).xy,0.0));
	// vec3 tangent = normalize(rvec - n * dot(rvec, n));
	// vec3 bitangent = cross(n, tangent);
	// mat3 tbn = mat3(tangent, bitangent, n);


	// float occlusion = 0.0;
	// for (int i = 0; i < 64; ++i) {
	// 	// get sampleP position:
	//    	vec3 sampleP = tbn * kernel[i];
	//    	// if (dot(sampleP,n) <= 0)
	//    	// 	sampleP = -sampleP;
	//    		sampleP =  sampleP * uRadius/2 + origin;
	  
	// 	// project sampleP position:
	//    	vec4 offset = vec4(sampleP, 1.0);
	//     offset = cameraInfo.ProjectionMatrix * offset;
	//     offset.xy /= offset.w;
	//     offset.xy = offset.xy * 0.5 + 0.5;

	//     float sampleDepth = texture(gPositionMap, offset.xy).b;

	//    	// range check & accumulate:
	//    	float rangeCheck = smoothstep(0.0, 1.0, uRadius / abs(origin.z - sampleDepth));
	// 	occlusion += rangeCheck * step(sampleP.z,sampleDepth);
	//    // float rangeCheck= abs(origin.z - sampleDepth) < uRadius ? 1.0 : 0.0;
 //    //         occlusion += step(sampleP.z,sampleDepth) / (1 + rangeCheck * rangeCheck);

	// }
	  
	// occlusion = 1-(occlusion / 64);

	Fragcolor = texture(text, ITextCoord.st) * (globalLight.global + material.emissive + globalLight.ambient * material.ambient);
}
