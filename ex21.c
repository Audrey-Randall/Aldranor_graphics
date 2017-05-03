/*
 *  Geodesic Sphere
 *  This program requires OpenGL 4.0 or above
 *
 *  Demonstrate a tesselation shader by drawing a sphere
 *  from an icosahedron.
 *
 *  Based on The Little Grashopper tutorial
 *  http://prideout.net/blog/?p=48
 *
 *  Key bindings:
 *  +/-        Increase/decrease inner and outer
 *  i/I        Increase/decrease inner
 *  o/O        Increase/decrease outer
 *  r          Reset inner/outer levels
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit

 Texture sources: http://forum.worldviz.com/attachment.php?attachmentid=417&d=1302715253
				  http://www.clipartbest.com/cliparts/7ia/Lpk/7iaLpkGxT.png
 */
#include "CSCIx239.h"
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int zh=0;         //  Light angle
int terrain_shader=0;     //  Shader
int water_shader=0;
int default_shader=0;
int plant_shader=0;
int N;            //  Number of faces
int Position0;     //  Position VBO for terrain
int Position1;     //  Position VBO for water
unsigned int vao0; //  VAO for terrain
unsigned int vao1; //  VAO for water
int Inner=64;      //  Tesselation inner level
int Outer=64;      //  Tesselation outer level
int move=1;       //  Move light
float asp=1;      //  Screen aspect ratio
float dim=2;      //  World dimension
float elv=-20.0;    //  Light elevation
int frame=0;
int frameCont=0; //continuous frame count, doesn't reset to 0 at 180
float w = 1.0;
float h = 1.0;
unsigned int tex_terrain;
unsigned int tex_grass;
unsigned int tex_ground;
unsigned int tex_water_norm;

//frame buffer
GLuint fbuf;
GLuint depthRenderBuffer;
GLenum DrawBuffers[1];
GLuint fbufTex;

void DrawWater(){
  float h = 0.5;
  float verts[18] =
  {
    -1.0,  h, -1.0,
     1.0,  h,  1.0,
     1.0,  h, -1.0,
    -1.0,  h, -1.0,
    -1.0,  h,  1.0,
     1.0,  h,  1.0
  };
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, verts);
  glDrawArrays(GL_TRIANGLES, 0, 2);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void drawTerrain(float* project, float* modelview, float* normal){
  int id;
  //  Set shader
  glUseProgram(terrain_shader);

  //  Controls for tesselation level
  id = glGetUniformLocation(terrain_shader,"Inner");
  if (id>=0) glUniform1f(id,Inner);
  id = glGetUniformLocation(terrain_shader,"Outer");
  if (id>=0) glUniform1f(id,Outer);

  //  Lighting parameters
  id = glGetUniformLocation(terrain_shader,"LightDir");
  if (id>=0) glUniform3f(id,Cos(zh),0.1*elv,Sin(zh));

  //  Set transformation matrixes
  id = glGetUniformLocation(terrain_shader,"Projection");
  if (id>=0) glUniformMatrix4fv(id,1,0,project);
  id = glGetUniformLocation(terrain_shader,"Modelview");
  if (id>=0) glUniformMatrix4fv(id,1,0,modelview);
  id = glGetUniformLocation(terrain_shader,"NormalMatrix");
  if (id>=0) glUniformMatrix3fv(id,1,0,normal);
  id = glGetUniformLocation(terrain_shader,"terrainTex");
  if (id>=0) glUniform1i(id,0);
  id = glGetUniformLocation(terrain_shader,"grassTex");
  if (id>=0) glUniform1i(id,1);
  id = glGetUniformLocation(terrain_shader,"time");
  if (id>=0) glUniform1i(id,frameCont);

  // Render the scene
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glClearColor(41/255.0, 48/255.0, 61/255.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPatchParameteri(GL_PATCH_VERTICES,3);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //glEnableVertexAttribArray(Position);
  //glVertexAttribPointer(Position,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);
  glBindVertexArray(vao0);
  if(vao0==0) Fatal("VAO0 was null\n");
  glDrawElements(GL_PATCHES,N,GL_UNSIGNED_INT,0);
  //glDisableVertexAttribArray(Position);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
}

void drawWater(float* project, float* modelview, float* normal, int current){
     int id;
     glUseProgram(current);
     glEnable(GL_BLEND);
     glDisable(GL_CULL_FACE);
     glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

     //Set uniforms
     id = glGetUniformLocation(current,"Projection");
     if (id>=0) glUniformMatrix4fv(id,1,0,project);
     id = glGetUniformLocation(current,"Modelview");
     if (id>=0) glUniformMatrix4fv(id,1,0,modelview);
     id = glGetUniformLocation(current,"NormalMatrix");
     if (id>=0) glUniformMatrix3fv(id,1,0,normal);
     id = glGetUniformLocation(current,"normal_tex");
     if (id>=0) glUniform1i(id,3);
     id = glGetUniformLocation(current,"LightPos");
     if (id>=0) glUniform4f(id,Cos(zh),0.1*elv,Sin(zh), 1.0);
     id = glGetUniformLocation(current,"frame");
     if (id>=0) glUniform1i(id,frame);

     //DrawWater();
     /*glBindVertexArray(vao1);
     if(vao1==0) Fatal("VAO1 was null\n");
     glDrawElements(GL_TRIANGLES,N,GL_UNSIGNED_INT,0);*/
     glBindVertexArray(vao1);
     glDrawElements(GL_TRIANGLES,N,GL_UNSIGNED_INT,0);
     glDisable(GL_BLEND);
     glEnable(GL_CULL_FACE);
}

void drawPlants(float* project, float* modelview, float* normal, int current){
  int id;
  glUseProgram(current);
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glPatchParameteri(GL_PATCH_VERTICES,3);

  //  Controls for tesselation level
  id = glGetUniformLocation(current,"Inner");
  if (id>=0) glUniform1f(id,Inner);
  id = glGetUniformLocation(current,"Outer");
  if (id>=0) glUniform1f(id,Outer);

  //  Lighting parameters
  id = glGetUniformLocation(current,"LightDir");
  if (id>=0) glUniform3f(id,Cos(zh),0.1*elv,Sin(zh));

  //  Set transformation matrixes
  id = glGetUniformLocation(current,"Projection");
  if (id>=0) glUniformMatrix4fv(id,1,0,project);
  id = glGetUniformLocation(current,"Modelview");
  if (id>=0) glUniformMatrix4fv(id,1,0,modelview);
  id = glGetUniformLocation(current,"NormalMatrix");
  if (id>=0) glUniformMatrix3fv(id,1,0,normal);
  id = glGetUniformLocation(current,"terrainTex");
  if (id>=0) glUniform1i(id,0);
  id = glGetUniformLocation(current,"grassTex");
  if (id>=0) glUniform1i(id,1);
  id = glGetUniformLocation(current,"time");
  if (id>=0) glUniform1i(id,frameCont);

  //DrawWater();
  /*glBindVertexArray(vao1);
  if(vao1==0) Fatal("VAO1 was null\n");
  glDrawElements(GL_TRIANGLES,N,GL_UNSIGNED_INT,0);*/
  glBindVertexArray(vao0);
  if(vao0==0) Fatal("VAO0 was null\n");
  glDrawElements(GL_PATCHES,N,GL_UNSIGNED_INT,0);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Modelview angles
   float costh = Cos(th);
   float sinth = Sin(th);
   float cosph = Cos(ph);
   float sinph = Sin(ph);
   //  Orthogonal projection matrix
   /*float project[16] =
   {
    asp/dim ,   0.0 ,    0.0 , 0.0 ,
        0.0 , 1/dim ,    0.0 , 0.0 ,
        0.0 ,   0.0 , -1/dim , 0.0 ,
        0.0 ,   0.0 ,    0.0 , 1.0 ,
   };*/
   //Perspective projection
   float n = 1.0; //near clipping plane
   float f = 3.0; //far clipping plane
   //FOV =  tan(toRadians(FOV/2))
   float y_scale = 1.0/tan(0.26);
   float x_scale = y_scale/asp;
   float frustum_len = f-n;
   float project[16] =
   {
     x_scale, 0.0, 0.0, 0.0,
     0.0, y_scale, 0.0, 0.0,
     0.0, 0.0, -(f+n)/frustum_len, -2*f*n/frustum_len,
     0.0, 0.0, -1.0, 0.0
   };
   //  Modelview matrix
   float modelview[16] =
   {
          costh ,   0.0 ,        sinth , 0.0 ,
    sinph*sinth , cosph , -costh*sinph , 0.0 ,
   -sinth*cosph , sinph ,  costh*cosph , 0.0 ,
            0.0 ,   0.0 ,          0.0 , 1.0 ,
   };
   //  Normal matrix (same as modelview because pure rotation)
   float normal[9] =
   {
          costh ,   0.0 ,        sinth ,
    sinph*sinth , cosph , -costh*sinph ,
   -sinth*cosph , sinph ,  costh*cosph ,
   };

   drawTerrain(project, modelview, normal);
   int current = water_shader;
   drawWater(project, modelview, normal, current);
   current = plant_shader;
   drawPlants(project, modelview, normal, current);

   //  Unset shader
   glUseProgram(0);


   //  Display parameters
   glColor3f(1,1,1);
   glWindowPos2i(5,5);
   Print("FPS=%d Angle=%d,%d Inner=%d Outer=%d",
      FramesPerSecond(),th,ph,Inner,Outer);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

int renderToFrameBuf(){
  fbuf = 0;
	glGenFramebuffers(1, &fbuf);
	glBindFramebuffer(GL_FRAMEBUFFER, fbuf);

  glActiveTexture(GL_TEXTURE7);
  glGenTextures(1, &fbufTex);
	glBindTexture(GL_TEXTURE_2D, fbufTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbufTex, 0);

	// Set the list of draw buffers.
	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return -1;
	else return 0;
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle movement
   else if (ch == 's' || ch == 'S')
      move = 1-move;
   //  Move light
   else if (ch == '[')
      zh += 5;
   else if (ch == ']')
      zh -= 5;
   //  Move light
   else if (ch == '{')
      elv++;
   else if (ch == '}')
      elv--;
   //  Reset levels
   else if (ch == 'r')
   {
      Inner = 1;
      Outer = 1;
   }
   //  Both levels
   else if (ch == '+')
   {
      Inner++;
      Outer++;
   }
   else if (ch == '-')
   {
      if (Inner>1) Inner--;
      if (Outer>1) Outer--;
   }
   //  Inner level
   else if (ch == 'i')
      Inner++;
   else if (ch == 'I' && Inner>1)
      Inner--;
   //  Outer level
   else if (ch == 'o')
      Outer++;
   else if (ch == 'O' && Outer>1)
      Outer--;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (width>0) ? (double)height/width : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   w = width;
   h = height;
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
   if(frame < 180) frame++;
   else frame = 0;
   frameCont++;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

//
//  Create Shader Program
//
int CreateShaderProgTess(void)
{
   //  Create program
   int prog = glCreateProgram();
   //  Compile shaders
   CreateShader(prog,GL_VERTEX_SHADER         ,"geodesic.vert");
   CreateShader(prog,GL_TESS_CONTROL_SHADER   ,"geodesic.tcs");
   CreateShader(prog,GL_TESS_EVALUATION_SHADER,"geodesic.tes");
   CreateShader(prog,GL_GEOMETRY_SHADER       ,"geodesic.geom");
   CreateShader(prog,GL_FRAGMENT_SHADER       ,"geodesic.frag");
   //  Associate Position with VBO
   glBindAttribLocation(prog,Position0,"Position");
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

int CreateShaderProgTessGrass(void)
{
   //  Create program
   int prog = glCreateProgram();
   //  Compile shaders
   CreateShader(prog,GL_VERTEX_SHADER         ,"plants.vert");
   CreateShader(prog,GL_TESS_CONTROL_SHADER   ,"plants.tcs");
   CreateShader(prog,GL_TESS_EVALUATION_SHADER,"plants.tes");
   CreateShader(prog,GL_GEOMETRY_SHADER       ,"plants.geom");
   CreateShader(prog,GL_FRAGMENT_SHADER       ,"plants.frag");
   //  Associate Position with VBO
   glBindAttribLocation(prog,Position0,"Position");
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

static void CreatePlane()
{
    unsigned int verts0, verts1, faces0,faces1;
    int h = -1.0;
    const int Faces[] = {
      0, 1, 3,
      0, 3, 2
    };
    const float Verts[] = {
      -5.0, -1.50, -5.0,
      -5.0, -1.50, 5.0,
       5.0, -1.50, -5.0,
       5.0, -1.50, 5.0,
    };
    const float VertsH2O[] = {
      -5.0, h, -5.0,
      -5.0, h, 5.0,
       5.0, h, -5.0,
       5.0, h, 5.0,
    };
    N = sizeof(Faces)/sizeof(int);

    // Create the VAO:
    glGenVertexArrays(1,&vao0);
    glBindVertexArray(vao0);

    // Create the VBO for positions:
    glGenBuffers(1,&verts0);
    glBindBuffer(GL_ARRAY_BUFFER,verts0);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Verts),Verts,GL_STATIC_DRAW);
    glEnableVertexAttribArray(Position0);
    glVertexAttribPointer(Position0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    // Create the VBO for facet indices:
    glGenBuffers(1,&faces0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,faces0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Faces),Faces,GL_STATIC_DRAW);

    //Plane #2 (for water surface):
    glGenVertexArrays(1,&vao1);
    glBindVertexArray(vao1);

    // Create the VBO for positions:
    glGenBuffers(1,&verts1);
    glBindBuffer(GL_ARRAY_BUFFER,verts1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(VertsH2O),VertsH2O,GL_STATIC_DRAW);
    glEnableVertexAttribArray(Position1);
    glVertexAttribPointer(Position1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    // Create the VBO for facet indices:
    glGenBuffers(1,&faces1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,faces1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Faces),Faces,GL_STATIC_DRAW);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Aldranor");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   CreatePlane();

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, tex_terrain);
   tex_terrain = LoadTexBMP("Qt/terrain_will_2.bmp");

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, tex_grass);
   tex_grass = LoadTexBMP("grass.bmp");

   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, tex_ground);
   tex_ground = LoadTexBMP("mud.bmp");

   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, tex_water_norm);
   tex_water_norm = LoadTexBMP("water_normals.bmp");

   glActiveTexture(GL_TEXTURE0);

   terrain_shader = CreateShaderProgTess();
   plant_shader = CreateShaderProgTessGrass();
   water_shader = CreateShaderProg("water_surf.vert","water_surf.frag");
   //default_shader = CreateShaderProg("Qt/Aldranor.vert", "Qt/Aldranor.frag");

   //if(renderToFrameBuf() < 0) Fatal("frame buffer error\n");

   ErrCheck("init");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
