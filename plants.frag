//  Geodesic Fragment Shader

#version 430

//  Colors
const vec3 AmbientMaterial = vec3(53/255.0, 28/255.0, 1/255.0);
const vec3 DiffuseMaterial = vec3(114/255.0, 65/255.0, 12/255.0);
const vec3 red = vec3(1.0,0.0,0.0);
const vec3 brown = vec3(53/255.0, 28/255.0, 1/255.0);
//  Light direction
uniform vec3 LightDir;
uniform mat3 NormalMatrix;
uniform mat4 Modelview;

//Time
uniform int time;

//Lighting
in vec3 gLight;
in vec3 gView;
//Grass
in  vec3  grassNormal;
flat in  int   isGrass;
//Texture
in vec2 gTexCoord;
in vec3 gObjVert;
layout(binding=0) uniform sampler2D terrainTex;
layout(binding=1) uniform sampler2D grassTex;
layout(binding=2) uniform sampler2D groundTex;
//  Output color
out vec4  FragColor;

vec3 getNormal(){
  vec3 myPos = vec3(gTexCoord.x, texture2D(terrainTex, gTexCoord).x*3, gTexCoord.y);
  vec3 pos1 = vec3(gTexCoord.x+0.005, texture2D(terrainTex, vec2(gTexCoord.x+0.005, gTexCoord.y+0.005)).x*3, gTexCoord.y);
  vec3 pos2 = vec3(gTexCoord.x, texture2D(terrainTex, vec2(gTexCoord.x, gTexCoord.y+0.005)).x*3, gTexCoord.y+0.005);
  vec3 a = pos1 - myPos;
  vec3 b = pos2 - myPos;
  vec3 normal = normalize(NormalMatrix * cross(a,b));
  return normal;
}

vec4 blinn()
{
   //  N is the object normal
   vec3 N;
   if(isGrass == 0) N = getNormal();
   else N = normalize(grassNormal);
   //  L is the light vector
   vec3 L = normalize(gLight); //light position - modelview*object vertex
   //  V is the view vector
   vec3 V = normalize(gView);

   //Settings
   float La = 0.7;
   float Ld = 0.7;
   float Ls = 0.001;
   vec4 Ambient = vec4(La,La,La,1.0);
   vec4 Diffuse = vec4(Ld,Ld,Ld,1.0);
   vec4 Specular = vec4(Ls,Ls,Ls,1.0);

   //  Emission and ambient color
   vec4 color = vec4(brown, 1.0);

   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*Diffuse; //gl_FrontLightProduct[0].diffuse;
      //  The half vectors (or reflected vector)
      //vec3 R = reflect(-L, N);
      vec3 H = normalize(V+L);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(H,V);
      //16 = shininess
      if (Is>0.0) color += pow(Is,2)*Specular;
   }

   //  Return sum of color components
   return color;
}

void main()
{
   //  Pixel color
   if(isGrass != 0) {
      vec4 texColor = texture2D(grassTex, gTexCoord);
      vec4 grassColor = blinn() * texColor;
      if(texColor.r > 0.9) grassColor.w = 0.0;
      FragColor = grassColor;
   } else {
     //Calculate distortion
     if(gObjVert.y < -1.0){
        vec2 distCoords = vec2(gTexCoord.x+0.002*sin(gTexCoord.y*300+0.3*time), gTexCoord.y+0.002*sin(gTexCoord.x*300+0.5*time));

        //Fog
        vec4 fogColor = vec4(0,0,0,1);
        float dEnd = -4; //depth at which fog is complete in world coordinates
        float dStart = -1.0; //depth at which fog begins, at the water's surface
        float f = (dEnd - gObjVert.y)/(dEnd - dStart);
        vec4 allColor = blinn()*texture2D(groundTex, distCoords);
        FragColor = f*allColor + (1-f)*fogColor;
     } else {
        FragColor = blinn() * texture2D(groundTex,gTexCoord); //vec4(color, 1.0);
     }
   }
}
