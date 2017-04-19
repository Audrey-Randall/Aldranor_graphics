#version 420

//  Geodesic Geometry Shader. Comes after both tessellation shaders.


//  Normal matrix
uniform mat3 NormalMatrix;
//  Triangles in and out
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
layout(binding=1) uniform sampler2D terrain;

//  Coordinates and weights in and out
in  vec3 tePosition[3];  //Coordinates of the newly created polygon
in  vec3 tePatchDistance[3]; //Coordinates of the original polygon - the patch
in  vec2 teTexCoords[3];
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;

void main()
{
   vec4 hmapped;
   //  Compute normal as cross product
   vec3 A = tePosition[2] - tePosition[0];
   vec3 B = tePosition[1] - tePosition[0];
   //  Transform normal
   gFacetNormal = NormalMatrix * normalize(cross(A, B));

   //You have to set all of these before calling emitVertex since they change per vertex
   //  First vertex
   gPatchDistance = tePatchDistance[0];
   gTriDistance = vec3(1, 0, 0);
   hmapped = gl_in[0].gl_Position;
   //hmapped.z = texture2D(terrain, vec2(teTexCoords[0].s, teTexCoords[0].t)).x;
   //hmapped.z += 0.5;
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   //  Second vertex
   gPatchDistance = tePatchDistance[1];
   gTriDistance = vec3(0, 1, 0);
   hmapped = gl_in[1].gl_Position;
   //hmapped.z = texture2D(terrain, vec2(teTexCoords[1].s, teTexCoords[1].t)).x;
   //hmapped.z += 0.5;
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   //  Third vertex
   gPatchDistance = tePatchDistance[2];
   gTriDistance = vec3(0, 0, 1);
   //gl_Position = gl_in[2].gl_Position;
   hmapped = gl_in[2].gl_Position;
   //hmapped.z = texture2D(terrain, vec2(teTexCoords[2].s, teTexCoords[2].t)).x;
   //hmapped.z += 0.5;
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   //  Done with triangle
   EndPrimitive();
}
