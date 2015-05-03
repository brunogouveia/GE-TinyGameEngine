#version 330

in vec3 IPosition;
in vec3 INormal;
in vec3 FrontColor;
in vec2 ITextCoord;

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut; 

uniform sampler2D gColorMap; 

void main() 
{ 
    WorldPosOut = IPosition; 
    DiffuseOut = FrontColor; 
    NormalOut = normalize(INormal); 
    TexCoordOut = vec3(ITextCoord, 0.0); 
}
