//  Terrain Vertex Shader

#version 420 core

//  Coordinates in and out
layout(location = 0) in vec4 Vertex;

out vec3 vPosition;
out vec2 vTexCoords;

void main()
{

   vPosition = Vertex.xyz;
   //vTexCoords = TexCoords;
}
