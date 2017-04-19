//  Geodesic Geometry Shader. Comes after both tessellation shaders.

#version 400

//  Normal matrix
uniform mat3 NormalMatrix;
//  Triangles in and out
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;
//  Coordinates and weights in and out
in  vec3 tePosition[3];  //Coordinates of the newly created polygon
in  vec3 tePatchDistance[3]; //Coordinates of the original polygon - the patch
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;
out vec3 grassNormal;
flat out int  isGrass;

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main()
{
   float height = rand(gl_in[0].gl_Position.xy)/9.0;
   //  Compute normal as cross product
   vec3 A = tePosition[2] - tePosition[0];
   vec3 B = tePosition[1] - tePosition[0];
   //  Transform normal
   gFacetNormal = NormalMatrix * normalize(cross(A, B));

   //  Compute normal of grass as cross product
   vec3 vert3other = tePosition[0] + (tePosition[1]-tePosition[0])/2 + height*vec3(gFacetNormal.x, gFacetNormal.y, abs(gFacetNormal.z));
   vec3 C = (tePosition[0]- vert3other.xyz).xyz;
   vec3 D = (tePosition[1] - vert3other.xyz).xyz;
   //  Transform normal
   grassNormal = NormalMatrix*normalize(cross(C, D));

   //You have to set all of these before calling emitVertex since they change per vertex
   //  First vertex
   isGrass = 0;
   gPatchDistance = tePatchDistance[0];
   gTriDistance = vec3(1, 0, 0);
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   //  Second vertex
   isGrass = 0;
   gPatchDistance = tePatchDistance[1];
   gTriDistance = vec3(0, 1, 0);
   gl_Position = gl_in[1].gl_Position;
   EmitVertex();

   //  Third vertex
   isGrass = 0;
   gPatchDistance = tePatchDistance[2];
   gTriDistance = vec3(0, 0, 1);
   gl_Position = gl_in[2].gl_Position;
   EmitVertex();

   //  Done with triangle
   EndPrimitive();

   //New triangle (blade of grass): vertices are tePosition[0], tePosition[1], tePosition[0] + (tePosition[1]-tePosition[0])/2 + 0.5*gFacetNormal
   //  First vertex
   isGrass = 1;
   gPatchDistance = tePatchDistance[0];
   gTriDistance = vec3(1, 0, 0);
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   //  Second vertex
   isGrass = 1;
   gPatchDistance = tePatchDistance[1];
   gTriDistance = vec3(0, 1, 0);
   gl_Position = gl_in[1].gl_Position;
   EmitVertex();

   //  Third vertex
   isGrass = 1;
   gPatchDistance = tePatchDistance[2];
   gTriDistance = vec3(0, 0, 1);
   vec4 midway = gl_in[0].gl_Position + (gl_in[1].gl_Position-gl_in[0].gl_Position)/2;
   vec4 vert3 = midway - height*vec4(gFacetNormal.x, gFacetNormal.y, abs(gFacetNormal.z), 1.0);
   gl_Position = vert3;
   EmitVertex();

   //  Done with triangle
   EndPrimitive();
}
