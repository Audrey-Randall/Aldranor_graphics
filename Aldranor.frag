#version 400 core
//uniforms
uniform mat4 LightSource;
uniform sampler2D tex;

//  Input from previous shader
in vec3 FrontColor;
in vec3 View;
in vec3 Light;
in vec3 oNormal;
in vec4 Ambient;
in vec2 oTexCoords;

//  Fragment color
layout (location=0) out vec4 Fragcolor;


vec4 blinn()
{
   //  N is the object normal
   vec3 N = normalize(oNormal);
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
   Fragcolor = blinn() * texture2D(tex,oTexCoords);
}
