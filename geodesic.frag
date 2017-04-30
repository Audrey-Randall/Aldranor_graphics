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
layout(binding=1) uniform sampler2D terrainTex;
layout(binding=2) uniform sampler2D grassTex;
//  Output color
out vec4  FragColor;

vec3 getNormal(){
  vec3 myPos = vec3(gTexCoord.x, texture2D(terrainTex, gTexCoord).x, gTexCoord.y);
  vec3 pos1 = vec3(gTexCoord.x+0.05, texture2D(terrainTex, vec2(gTexCoord.x+0.05, gTexCoord.y)).x, gTexCoord.y);
  vec3 pos2 = vec3(gTexCoord.x, texture2D(terrainTex, vec2(gTexCoord.x, gTexCoord.y+0.05)).x, gTexCoord.y+0.05);
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
   vec3 L = normalize(gLight);
   //  V is the view vector
   vec3 V = normalize(gView);

   //Settings
   float La = 0.3;
   float Ld = 1.0;
   float Ls = 0.05;
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
      //  The half vectors
      vec3 H = normalize(V+L);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(H,N);
      //16 = shininess
      if (Is>0.0) color += pow(Is,64)*Specular;
   }

   //  Return sum of color components
   return color;
}

void main()
{

   //  Diffuse only lighting
   vec3 N;
   if(isGrass == 0) N = getNormal(); //normalize(gFacetNormal);
   else N = normalize(grassNormal);
   vec3 L = normalize(LightDir);
   vec3 color;
   if(isGrass == 0) color = AmbientMaterial + max(dot(N,L),0.0) * DiffuseMaterial;
   else color = vec3(0, 0.05, 0) + max(dot(N,L),0.0) * vec3(0, 0.5, 0);

   //  Draw mesh in red, heavy for patches, light for triangles
   float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
   float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
   //Step returns either 0 or 1 depending on if d1 is less than 0.02: 0 if d1 < 0.02, 1 otherwise
   //This is a clever and simple way to color the lines red based on the barymetric coordinates of the vertices
   //if(isGrass==0) color = mix(brown,color,step(0.02,d1) * step(0.01,d2));

   //  Pixel color
   //if(isGrass != 0) FragColor = blinn() * texture2D(grassTex, gTexCoord);
   FragColor = blinn();// * texture2D(terrainTex,gTexCoord); //vec4(color, 1.0);
   //FragColor = vec4(N, 1);
}
