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

//  Normal
in  vec3  gFacetNormal;
//  Distance to edge of patch and triange
in  vec3  gTriDistance;
in  vec3  gPatchDistance;
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
   if(isGrass == 0) N = getNormal(); //normalize(gFacetNormal);
   else N = normalize(grassNormal);
   //  L is the light vector
   vec3 L = normalize(gLight); //light position - modelview*object vertex
   //  V is the view vector
   vec3 V = normalize(gView);

   //Settings
   float La = 0.3;
   float Ld = 1.0;
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
      FragColor = vec4(1.0,0,0,1);//grassColor;
   } else {
     //Calculate distortion
     if(gObjVert.y < -1.0){
        vec2 distCoords = vec2(gTexCoord.x+0.002*sin(gTexCoord.y*300+0.3*time), gTexCoord.y+0.002*sin(gTexCoord.x*300+0.5*time));
        FragColor = blinn()*texture2D(groundTex, distCoords);
     } else {
        FragColor = blinn() * texture2D(groundTex,gTexCoord); //vec4(color, 1.0);
     }
   }
}
