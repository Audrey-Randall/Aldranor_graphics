#version 420 core

//uniforms
uniform mat4 LightSource;

//  Input from previous shader
in vec3 FrontColor;
in vec3 View;
in vec3 Light;
in vec4 Ambient;
in vec2 gTexCoords;
in float  height;

//  Normal
in  vec3  gFacetNormal;
//  Distance to edge of patch and triangle
in  vec3  gTriDistance;
in  vec3  gPatchDistance;

//Color
vec4 red = vec4(1.0,0,0,1.0);

//  Fragment color
layout (location=0) out vec4 Fragcolor;
layout (binding=1) uniform sampler2D terrainTex;


vec4 blinn()
{
   //  N is the object normal
   vec3 N = normalize(gFacetNormal);
   //  L is the light vector
   vec3 L = normalize(Light);
   //  V is the view vector
   vec3 V = normalize(View);

   //  Emission and ambient color
   vec4 color = Ambient;

   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*LightSource[1].xyzw; //gl_FrontLightProduct[0].diffuse;
      //  The half vectors
      vec3 H = normalize(V+L);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(H,N);
      //16 = shininess
      if (Is>0.0) color += pow(Is,16)*LightSource[2].xyzw;
   }

   //  Return sum of color components
   return color;
}


void main()
{
    float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
    float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
    //Step returns either 0 or 1 depending on if d1 is less than 0.02: 0 if d1 < 0.02, 1 otherwise
    //This is a clever and simple way to color the lines red based on the barymetric coordinates of the vertices
    vec4 color = mix(red,blinn(),step(0.02,d1) * step(0.01,d2));
    Fragcolor = vec4(1,1,1,1); //color;
}
