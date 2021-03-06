#version 400 core

//  Transformation matrices
uniform mat4 Modelview;
uniform mat4 Projection;
uniform mat4 LightSource;
uniform sampler2D tex;

//  Vertex attributes (input)
layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 iTexCoords;

//  Output to next shader
out vec3 FrontColor;
out vec3 View;
out vec3 Light;
out vec3 oNormal;
out vec4 Ambient;
out vec2 oTexCoords;


void main(){
    /*//
    //  Lighting values needed by fragment shader
    //
    //  Vertex location in modelview coordinates
    vec3 P = vec3(ModelViewMatrix * Vertex).xyz;
    //  Light position
    Light  = vec3(LightSource[3].xyz) - P;
    //  Normal
    oNormal = vec3(transpose(inverse(ModelViewMatrix)) * vec4(iNormal.x, iNormal.y, iNormal.z, 1.0)).xyz;
    //  Eye position
    View  = -P;
    //  Ambient color
    //  Material emission + light(???)*mat color + mat.ambient*light.ambient
    //  Roughly speaking: gl_FrontLightProduct[i] == gl_FrontMaterial * gl_LightSource[i]
    //Ambient = gl_FrontMaterial.emission + gl_FrontLightProduct[0].ambient + gl_LightModel.ambient*gl_FrontMaterial.ambient;
    Ambient = vec4(0,0,0,1) + LightSource[0].xyzw;

    //  Texture coordinate for fragment shader
    oTexCoords = iTexCoords; //gl_MultiTexCoord0;*/

    //  Set vertex position
    gl_Position = Projection * Modelview * Vertex;
}
