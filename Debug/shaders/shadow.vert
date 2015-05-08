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

out vec3 worldPos;

void main()
{  
   //  Set transformed vertex location
   gl_Position =  shadows.depthMVP * cameraInfo.ModelMatrix * Vertex;
   worldPos = vec3(cameraInfo.ModelMatrix * Vertex);
}
