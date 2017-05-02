#version 430

uniform mat4 Modelview;
uniform mat4 Projection;
uniform mat3 NormalMatrix;
uniform vec4 LightPos;

layout(location = 0) in vec4 Vertex;


out vec4 diffuse, ambient;
out vec3 normal, halfVector, viewVector;
out vec2 texCoords;
out vec3 pos, lightVec;
out vec4 vPosition;

void main()
{
    normal = normalize(NormalMatrix * vec3(0.0,1.0,0.0));

    viewVector = vec3(0,0,0) - vec3(Modelview * Vertex);
    lightVec = vec3(Modelview*LightPos);

    vPosition = Projection * Modelview * Vertex;
    texCoords = (vec2(Vertex.x, Vertex.z)+1.0)/2.0;
    gl_Position = Projection*Modelview*Vertex;

}
