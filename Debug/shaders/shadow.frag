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

in vec3 worldPos;

// Ouput data
layout(location = 0) out vec3 fragmentdepth;
 
void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = vec3(gl_FragCoord.z);
    fragmentdepth = vec3(clamp((length(worldPos - vec3(light.position)) - 1)/(49),0,1));
}