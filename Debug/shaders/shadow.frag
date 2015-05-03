#version 400 core

//  Transformation matrices
uniform Tranformations {
   // Projection Matrix
   mat4 ProjectionMatrix;
   // Model Matrix
   mat4 ModelMatrix;
   // View Matrix
   mat4 ViewMatrix;
   // Normal matrix;
   mat4 NormalMatrix;
} tranformations;

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
} shadows;

in float dep;

// Ouput data
layout(location = 0) out float fragmentdepth;
 
void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
}