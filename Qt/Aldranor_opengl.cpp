//
//  OpenGL widget
//
#include "Aldranor_opengl.h"

//
//  Constructor
//
Aldranor_opengl::Aldranor_opengl(QWidget* parent)
    : CUgl(parent)
{
}

const int plane_size = PLANE_SEGS*PLANE_SEGS*6;
void Aldranor_opengl::fillPlaneData(){
    float color[] = {1.0,1.0,1.0};
    float segsize = 2.0/PLANE_SEGS;
    int x = 0;
    int y = 0;
    for(float i = -1; i < 1; i += segsize){
        for(float j = -1; j < 1; j += segsize) {
            //data for this mini plane starts at x*y*12*6
            int start = (x*PLANE_SEGS+y)*12*6;
            //std::cout<<"Start is "<<start<<std::endl;

            //Aaaagh there must be an easier way
            plane_data[start] = i;
            plane_data[start+1] = j;
            plane_data[start+12] = i+segsize;
            plane_data[start+13] = j+segsize;
            plane_data[start+24] = i+segsize;
            plane_data[start+25] = j;
            plane_data[start+36] = i;
            plane_data[start+37] = j;
            plane_data[start+48] = i;
            plane_data[start+49] = j+segsize;
            plane_data[start+60] = i+segsize;
            plane_data[start+61] = j+segsize;

            for(int k = start; k < start+72; k+=12){
                plane_data[k+2] = 0;
                plane_data[k+3] = 1;
                plane_data[k+4] = 0;
                plane_data[k+5] = 0;
                plane_data[k+6] = 1;
                plane_data[k+7] = 1.0;
                plane_data[k+8] = 1.0;
                plane_data[k+9] = 1.0;
                plane_data[k+10] = (plane_data[k]+1)/2;
                plane_data[k+11] = (plane_data[k+1]+1)/2;
            }
            y++;
        }
        y=0;
        x++;
    }
}

//
//  Cube Vertexes
//
const int cube_size=36;
const float cube_data[] =  // Vertex data
{
//  X  Y  Z  W   Nx Ny Nz    R G B   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   1,0,0,  1,1,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,  1,0,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,  1,0,
   -1,-1,+1,+1,   0, 0,+1,   1,0,0,  0,0,
   //  Back
   -1,-1,-1,+1,   0, 0,-1,   0,0,1,  1,0,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,  1,1,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,  1,1,
   +1,+1,-1,+1,   0, 0,-1,   0,0,1,  0,1,
   //  Right
   +1,+1,+1,+1,  +1, 0, 0,   1,1,0,  0,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,  1,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,  1,1,
   +1,-1,-1,+1,  +1, 0, 0,   1,1,0,  1,0,
   //  Left
   -1,+1,+1,+1,  -1, 0, 0,   0,1,0,  1,1,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,  1,0,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,  1,0,
   -1,-1,-1,+1,  -1, 0, 0,   0,1,0,  0,0,
   //  Top
   +1,+1,+1,+1,   0,+1, 0,   0,1,1,  1,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,  0,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,  0,0,
   -1,+1,-1,+1,   0,+1, 0,   0,1,1,  0,1,
   //  Bottom
   -1,-1,-1,+1,   0,-1, 0,   1,0,1,  0,0,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,  0,1,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,  0,1,
   +1,-1,+1,+1,   0,-1, 0,   1,0,1,  1,1,
   };

//
//  Initialize
//
void Aldranor_opengl::initializeGL()
{
   //Set up for textures
   glf = new QGLFunctions(QGLContext::currentContext());

   //Fill plane data
   fillPlaneData();

   // Textures
   QPixmap crate(":/crate.png");
   glf->glActiveTexture(GL_TEXTURE0);
   cube_tex = bindTexture(crate,GL_TEXTURE_2D);
   QPixmap terrain(":/terrain_will.jpg");
   glf->glActiveTexture(GL_TEXTURE1);
   terrain_tex = bindTexture(terrain, GL_TEXTURE_2D);

   //  Load shaders
   addShader(":/Aldranor.vert",":/Aldranor.frag");
   addShader5(":/terrain.vert", ":/terrain.tcs", ":/terrain.tes", ":/terrain.geom", ":/terrain.frag");

   //  Cube vertex buffer object
   //  Copy data to vertex buffer object
   cube_buffer.create();
   cube_buffer.bind();
   cube_buffer.setUsagePattern(QGLBuffer::StaticDraw);
   cube_buffer.allocate(sizeof(cube_data));
   cube_buffer.write(0,cube_data,sizeof(cube_data));
   //  Unbind this buffer
   cube_buffer.release();

   //Plane vertex buffer object
   plane_buffer.create();
   plane_buffer.bind();
   plane_buffer.setUsagePattern(QGLBuffer::StaticDraw);
   plane_buffer.allocate(sizeof(plane_data));
   plane_buffer.write(0,plane_data,sizeof(plane_data));
   plane_buffer.release();
}

//
//  Draw the window
//
void Aldranor_opengl::paintGL()
{
   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glClearColor(0.2,0.2,0.2,1.0);

   //  Set projection
   doProjection();
   //  Set view
   doView();
   //  Set light - will generate warning since lpos is not used (yet)
   LightSource ls;
   QVector3D lpos = doLight(&ls);
   QMatrix4x4 lsMat = QMatrix4x4(
           ls.ambient[0], ls.diffuse[0], ls.specular[0], ls.position[0],
           ls.ambient[1], ls.diffuse[1], ls.specular[1], ls.position[1],
           ls.ambient[2], ls.diffuse[2], ls.specular[2], ls.position[2],
           ls.ambient[3], ls.diffuse[3], ls.specular[3], ls.position[3]);

   //  Fixed pipeline
   if (mode==0)
   {
      //  Enable textures
      glEnable(GL_TEXTURE_2D);
      glf->glActiveTexture(GL_TEXTURE1); //necessary?
      glBindTexture(GL_TEXTURE_2D,terrain_tex);

      //  Enabe arrays
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      //  Set pointers
      
      glVertexPointer  (4,GL_FLOAT,12*sizeof(GLfloat),plane_data);
      glNormalPointer  (  GL_FLOAT,12*sizeof(GLfloat),plane_data+4);
      glColorPointer   (3,GL_FLOAT,12*sizeof(GLfloat),plane_data+7);
      glTexCoordPointer(2,GL_FLOAT,12*sizeof(GLfloat),plane_data+10);

      //  Draw the cube, or plane.
      glPushMatrix();
      glRotated(90, 1,0,0);
      glScaled(10,1,10);
      glDrawArrays(GL_TRIANGLES,0,plane_size);
      glPopMatrix();

      //  Disable arrays
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);

      //  Disable textures and lighting
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_LIGHTING);
   }
   //  OpenGL 4 style shaders
   else
   {
      glf->glActiveTexture(GL_TEXTURE1); //necessary?
      glBindTexture(GL_TEXTURE_2D,terrain_tex); //neeeecesssaaaarrrry???
      //  Create Modelview matrix, initialized to identity
      QMatrix4x4 mv;
      if (fov) mv.translate(0,0,-2*dim);
      mv.rotate(ph,1,0,0);
      mv.rotate(th,0,1,0);
      //for plane
      mv.rotate(90, 1,0,0);
      mv.scale(5,5,5);

      // Enable shader
      shader[mode]->bind();
      //  Set Modelview and Projection Matrix
      shader[mode]->setUniformValue("ProjectionMatrix",proj);
      shader[mode]->setUniformValue("ModelViewMatrix",mv);
      shader[mode]->setUniformValue("NormalMatrix", mv.normalMatrix());
      shader[mode]->setUniformValue("LightSource", lsMat);
      shader[mode]->setUniformValue("Inner", 3);
      shader[mode]->setUniformValue("Outer", 2);
      shader[mode]->setPatchVertexCount(3); //replaces glPatchParameteri

      //  Select plane buffer
      plane_buffer.bind();
      //   Attribute 0: vertex coordinate (vec4) at offset 0
      shader[mode]->enableAttributeArray(0);
      shader[mode]->setAttributeBuffer(0,GL_FLOAT,0,4,12*sizeof(float));
      //   Attribute 1: normals (vec3) at offset 4 floats
      shader[mode]->enableAttributeArray(1);
      shader[mode]->setAttributeBuffer(1,GL_FLOAT,4*sizeof(float),3,12*sizeof(float));
      //   Attribute 2: vertex color (vec3) offset 7 floats
      shader[mode]->enableAttributeArray(2);
      shader[mode]->setAttributeBuffer(2,GL_FLOAT,7*sizeof(float),3,12*sizeof(float));
      //   Attribute 3: texture coordinates at offset 10 floats
      shader[mode]->enableAttributeArray(3);
      shader[mode]->setAttributeBuffer(3,GL_FLOAT,10*sizeof(float),2,12*sizeof(float));



      //  Draw the cube, or plane.
      glPushMatrix();
      glRotated(90, 1,0,0);
      glScaled(10,1,10);
      if(mode ==2 )glDrawArrays(GL_PATCHES,0,plane_size);
      else glDrawArrays(GL_TRIANGLES,0,plane_size);
      glPopMatrix();

      //  Disable vertex arrays
      shader[mode]->disableAttributeArray(0);
      shader[mode]->disableAttributeArray(1);

      //  Unbind this buffer
      plane_buffer.release();

      // Back to fixed pipeline
      shader[mode]->release();
   }

   //  Axes for reference
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3f(0,0,0);
   glVertex3f(2,0,0);
   glVertex3f(0,0,0);
   glVertex3f(0,2,0);
   glVertex3f(0,0,0);
   glVertex3f(0,0,2);
   glEnd();
   glDisable(GL_DEPTH_TEST);
   renderText(2,0,0,"X");
   renderText(0,2,0,"Y");
   renderText(0,0,2,"Z");

   GLenum err;
   while((err = glGetError()) != GL_NO_ERROR)
   {
     printf("%x\n", err);
   }
}

//
//  Set OpenGL projection
//
void Aldranor_opengl::doProjection()
{
   //  Set fixed pipeline transformation
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (fov)
   {
      float zmin = dim/4;
      float zmax = 4*dim;
      float ydim = zmin*tan(fov*M_PI/360);
      float xdim = ydim*asp;
      glFrustum(-xdim,+xdim,-ydim,+ydim,zmin,zmax);
   }
   else
      glOrtho(-dim*asp, +dim*asp, -dim, +dim, -dim, +dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //  Set GL4 transformation
   proj.setToIdentity();
   if (fov)
      proj.perspective(fov,asp,dim/4,4*dim);
   else
      proj.ortho(-dim*asp, +dim*asp, -dim, +dim, -dim, +dim);
}
