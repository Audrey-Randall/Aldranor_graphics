#version 430

in vec3 normal, viewVector;
layout(binding=3) uniform sampler2D normal_tex;
uniform int frame;
in vec2 texCoords;
in vec3 pos, lightVec;

layout(location=0) out vec4 FragColor;

void main()
{
    vec3 n,halfV,lightDir, viewDir;
    float NdotL,NdotHV;
    vec4 fogColor = vec4(0,0,0,1);

    float La = 0.3;
    float Ld = 1.0;
    vec4 ambient = vec4(La,La,La,1.0);
    vec4 diffuse = vec4(Ld,Ld,Ld,1.0);

    lightDir = (vec3(lightVec - viewVector))/6;
    viewDir = (viewVector);

    float lLen = sqrt(lightDir.x*lightDir.x + lightDir.y*lightDir.y+lightDir.z*lightDir.z);
    float vLen = sqrt(viewDir.x*viewDir.x + viewDir.y*viewDir.y+viewDir.z*viewDir.z);

    float t = -1.0*pos.y/viewVector.y;
    vec3 intersect = vec3(pos.x+t*viewVector.x, pos.y + t*viewVector.y, pos.z + t*viewVector.z);
    vec3 distVec = intersect - pos;
    float depth = sqrt((distVec.x*distVec.x)+(distVec.y*distVec.y)+(distVec.z*distVec.z));

    vec4 color = ambient;

    vec2 horizCoords = vec2(texCoords.x, texCoords.y + (1./180.* frame));
    vec2 vertCoords = vec2(texCoords.x + (1./180.*frame), texCoords.y);
    n = normalize(((texture2D(normal_tex, horizCoords).xyz + texture2D(normal_tex, vertCoords).xyz)/2));//*normal);

    /* compute the dot product between normal and ldir */
    vec3 lDir = vec3(lightDir.x, -1*lightDir.y, lightDir.z);
    NdotL = max(dot(n,lDir),0.0);

    if (NdotL > 0.0) {
        color += diffuse * NdotL;
        halfV = normalize(lightDir + viewDir);
        NdotHV = max(dot(n,halfV),0.0);
        vec4 specular = vec4(0.9, 0.9, 0.9, 1.0);
        color += specular*pow(NdotHV, 16);
    }
    //The range of light intensities is super high, need to up the ambient and decrease specular
    //vec4 texture = texture2D(tex, texCoords).xyzw;
    //texture.w = 1;
    vec4 allColor = color * vec4(41/255.0,48/255.0,61/255.0,1.0);
    allColor.w = 0.3;
    FragColor = allColor;
}
