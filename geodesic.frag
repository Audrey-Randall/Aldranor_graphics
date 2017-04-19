//  Geodesic Fragment Shader

#version 400

//  Colors
const vec3 AmbientMaterial = vec3(53/255.0, 28/255.0, 1/255.0);
const vec3 DiffuseMaterial = vec3(114/255.0, 65/255.0, 12/255.0);
const vec3 red = vec3(1.0,0.0,0.0);
const vec3 brown = vec3(53/255.0, 28/255.0, 1/255.0);
//  Light direction
uniform vec3 LightDir;
//  Normal
in  vec3  gFacetNormal;
//  Distance to edge of patch and triange
in  vec3  gTriDistance;
in  vec3  gPatchDistance;
in  vec3  grassNormal;
flat in  int   isGrass;
//  Output color
out vec4  FragColor;

void main()
{
   //  Diffuse only lighting
   vec3 N;
   if(isGrass == 0) N = normalize(gFacetNormal);
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
   if(isGrass==0) color = mix(brown,color,step(0.02,d1) * step(0.01,d2));

   //  Pixel color
   FragColor = vec4(color, 1.0);
}
