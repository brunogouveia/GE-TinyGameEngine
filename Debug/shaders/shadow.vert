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
//  Vertex attributes (input)
layout(location = 0) in vec4 Vertex;

out float dep;

void main()
{  
   //  Set transformed vertex location
   gl_Position =  shadows.depthMVP * tranformations.ModelMatrix * Vertex;
   gl_Position /= gl_Position.w;
   dep = gl_Position.z;
}
