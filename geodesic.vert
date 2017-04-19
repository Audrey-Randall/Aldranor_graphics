//  Terrain Vertex Shader

#version 420 core

//  Coordinates in and out
layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 TexCoords;
out vec3 vPosition;
out vec2 vTexCoords;

void main()
{

   vPosition = Vertex.xyz;
   vTexCoords = TexCoords;
}
