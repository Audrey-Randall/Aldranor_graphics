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
   //grassNormal = NormalMatrix*normalize(cross(C, D));

   vec4 p0 = gl_in[0].gl_Position;
   vec4 p1 = gl_in[0].gl_Position + height*up[0];
   vec4 p2 = gl_in[1].gl_Position;
   vec4 p3 = gl_in[1].gl_Position + height*up[0];
   vec3 v1 = vec3(p1-p0);
   vec3 v2 = vec3(p2-p0);
   grassNormal = normalize(cross(v1,v2));

   if(teObjVert[0].y > -1 && teObjVert[1].y > -1 && teObjVert[2].y > -1 && grassNormal.z<0) {
     //  Compute normal of grass as cross product
     vec3 gPatchDistance;
     vec3 gTriDistance;

     //  First vertex
     isGrass = 1;
     P = vec3(Modelview * p0).xyz;
     gLight = LightDir - P;
     gView = -P;
     gPatchDistance = tePatchDistance[0];
     gTriDistance = vec3(1, 0, 0);
     gTexCoord = vec2(0.0,0.0);
     gl_Position = p0;
     EmitVertex();

     //  Second vertex
     isGrass = 1;
     P = vec3(Modelview * p1).xyz;
     gLight = LightDir - P;
     gView = -P;
     gPatchDistance = tePatchDistance[1];
     gTriDistance = vec3(0, 1, 0);
     gTexCoord = vec2(0.0,1.0);
     gl_Position = p1;
     EmitVertex();

     //  Third vertex
     isGrass = 1;
     P = vec3(Modelview * p2).xyz;
     gLight = LightDir - P;
     gView = -P;
     gPatchDistance = tePatchDistance[2];
     gTriDistance = vec3(0, 0, 1);
     gTexCoord = vec2(1.0,0.0);
     gl_Position = p2; //gl_in[1].gl_Position;
     EmitVertex();

     //  Done with triangle
     EndPrimitive();

     //And again:
     //  First vertex
     isGrass = 1;
     P = vec3(Modelview * p1).xyz;
     gLight = LightDir - P;
     gView = -P;
     gPatchDistance = tePatchDistance[0];
     gTriDistance = vec3(1, 0, 0);
     gTexCoord = vec2(0.0,1.0);
     gl_Position = p1; //gl_in[0].gl_Position + height*up[0];
     EmitVertex();

     //  Second vertex
     isGrass = 1;
     P = vec3(Modelview * p3).xyz;
     gLight = LightDir - P;
     gView = -P;
     gPatchDistance = tePatchDistance[1];
     gTriDistance = vec3(0, 1, 0);
     gTexCoord = vec2(1.0,1.0);
     gl_Position = p3; //gl_in[1].gl_Position + height*up[0];
     EmitVertex();

     //  Third vertex
     isGrass = 1;
     P = vec3(Modelview * p2).xyz;
     gLight = LightDir - P;
     gView = -P;
     gPatchDistance = tePatchDistance[2];
     gTriDistance = vec3(0, 0, 1);
     gTexCoord = vec2(1.0,0.0);
     gl_Position = p2; //gl_in[1].gl_Position;
     EmitVertex();

     //  Done with triangle
     EndPrimitive();
   }
}
