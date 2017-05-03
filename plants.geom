//  Geodesic Geometry Shader. Comes after both tessellation shaders.

#version 400

//  Normal matrix
uniform mat3 NormalMatrix;
//  Light direction
uniform vec3 LightDir;
uniform mat4 Modelview;

//  Triangles in and out
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;
//  Coordinates and weights in and out
in  vec3 tePosition[3];  //Coordinates of the newly created polygon
in  vec3 tePatchDistance[3]; //Coordinates of the original polygon - the patch
in  vec2 teTexCoord[3];
in  vec4 up[]; //all of these are the same
in  vec3 teObjVert[];

out vec3 grassNormal;
out vec3 gLight;
out vec3 gView;
out vec2 gTexCoord;
out vec3 gObjVert;
flat out int  isGrass;

void main()
{
   vec3 P;
   float height = 0.5;
   vec3 gFacetNormal;
   //  Compute normal as cross product
   vec3 A = tePosition[2] - tePosition[0];
   vec3 B = tePosition[1] - tePosition[0];
   //  Transform normal
   gFacetNormal = NormalMatrix * normalize(cross(A, B));

   vec3 vert3other = tePosition[0] + (tePosition[1]-tePosition[0])/2 + height*vec3(gFacetNormal.x, gFacetNormal.y, abs(gFacetNormal.z));
   vec3 C = (tePosition[0]- vert3other.xyz).xyz;
   vec3 D = (tePosition[1] - vert3other.xyz).xyz;
   //  Transform normal
   grassNormal = NormalMatrix*normalize(cross(C, D));

   if(teObjVert[0].y > -1 && teObjVert[1].y > -1 && teObjVert[2].y > -1 && grassNormal.z>0.0) {
     //  Compute normal of grass as cross product
     vec3 gPatchDistance;
     vec3 gTriDistance;

     //  First vertex
     isGrass = 1;
     gPatchDistance = tePatchDistance[0];
     gTriDistance = vec3(1, 0, 0);
     gTexCoord = vec2(0.0,0.0);
     gl_Position = gl_in[0].gl_Position;
     EmitVertex();

     //  Second vertex
     isGrass = 1;
     gPatchDistance = tePatchDistance[1];
     gTriDistance = vec3(0, 1, 0);
     gTexCoord = vec2(0.0,1.0);
     gl_Position = gl_in[0].gl_Position + height*up[0];
     EmitVertex();

     //  Third vertex
     isGrass = 1;
     gPatchDistance = tePatchDistance[2];
     gTriDistance = vec3(0, 0, 1);
     gTexCoord = vec2(1.0,0.0);
     gl_Position = gl_in[1].gl_Position;
     EmitVertex();

     //  Done with triangle
     EndPrimitive();

     //And again:
     //  First vertex
     isGrass = 1;
     gPatchDistance = tePatchDistance[0];
     gTriDistance = vec3(1, 0, 0);
     gTexCoord = vec2(0.0,1.0);
     gl_Position = gl_in[0].gl_Position + height*up[0];
     EmitVertex();

     //  Second vertex
     isGrass = 1;
     gPatchDistance = tePatchDistance[1];
     gTriDistance = vec3(0, 1, 0);
     gTexCoord = vec2(1.0,1.0);
     gl_Position = gl_in[1].gl_Position + height*up[0];
     EmitVertex();

     //  Third vertex
     isGrass = 1;
     gPatchDistance = tePatchDistance[2];
     gTriDistance = vec3(0, 0, 1);
     gTexCoord = vec2(1.0,0.0);
     gl_Position = gl_in[1].gl_Position;
     EmitVertex();

     //  Done with triangle
     EndPrimitive();
   }
}
