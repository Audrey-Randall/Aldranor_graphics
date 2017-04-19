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

 Texture source: http://forum.worldviz.com/attachment.php?attachmentid=417&d=1302715253
 */
#include "CSCIx239.h"
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int zh=0;         //  Light angle
int shader=0;     //  Shader
int N;            //  Number of faces
int Position;     //  Position VBO
int Inner=3;      //  Tesselation inner level
int Outer=2;      //  Tesselation outer level
int move=1;       //  Move light
float asp=1;      //  Screen aspect ratio
float dim=2;      //  World dimension
float elv=-10;    //  Light elevation
unsigned int tex_terrain;

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   int id;
   //  Modelview angles
   float costh = Cos(th);
   float sinth = Sin(th);
   float cosph = Cos(ph);
   float sinph = Sin(ph);
   //  Orthogonal projection matrix
   float project[16] =
   {
    asp/dim ,   0.0 ,    0.0 , 0.0 ,
        0.0 , 1/dim ,    0.0 , 0.0 ,
        0.0 ,   0.0 , -1/dim , 0.0 ,
        0.0 ,   0.0 ,    0.0 , 1.0 ,
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

   //  Set shader
   glUseProgram(shader);

   //  Controls for tesselation level
   id = glGetUniformLocation(shader,"Inner");
   if (id>=0) glUniform1f(id,Inner);
   id = glGetUniformLocation(shader,"Outer");
   if (id>=0) glUniform1f(id,Outer);

   //  Lighting parameters
   id = glGetUniformLocation(shader,"LightDir");
   if (id>=0) glUniform3f(id,Cos(zh),0.1*elv,Sin(zh));

   //  Set transformation matrixes
   id = glGetUniformLocation(shader,"Projection");
   if (id>=0) glUniformMatrix4fv(id,1,0,project);
   id = glGetUniformLocation(shader,"Modelview");
   if (id>=0) glUniformMatrix4fv(id,1,0,modelview);
   id = glGetUniformLocation(shader,"NormalMatrix");
   if (id>=0) glUniformMatrix3fv(id,1,0,normal);

   // Render the scene
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glClearColor(0.2,0.2,0.2,1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPatchParameteri(GL_PATCH_VERTICES,3);
   glDrawElements(GL_PATCHES,N,GL_UNSIGNED_INT,0);

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
      Inner = 3;
      Outer = 2;
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
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
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
   glBindAttribLocation(prog,Position,"Position");
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

static void CreateIcosahedron()
{
    unsigned int vao,verts,faces;
    const int Faces[] = {
        2, 1, 0,    3, 2, 0,    4, 3, 0,    5, 4, 0,    1, 5, 0,
       11, 6, 7,   11, 7, 8,   11, 8, 9,   11, 9,10,   11,10, 6,
        1, 2, 6,    2, 3, 7,    3, 4, 8,    4, 5, 9,    5, 1,10,
        2, 7, 6,    3, 8, 7,    4, 9, 8,    5,10, 9,    1, 6,10};
    const float Verts[] = {
         0.000, 0.000, 1.000,
         0.894, 0.000, 0.447,
         0.276, 0.851, 0.447,
        -0.724, 0.526, 0.447,
        -0.724,-0.526, 0.447,
         0.276,-0.851, 0.447,
         0.724, 0.526,-0.447,
        -0.276, 0.851,-0.447,
        -0.894, 0.000,-0.447,
        -0.276,-0.851,-0.447,
         0.724,-0.526,-0.447,
         0.000, 0.000,-1.000};
    N = sizeof(Faces)/sizeof(int);

    // Create the VAO:
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    // Create the VBO for positions:
    glGenBuffers(1,&verts);
    glBindBuffer(GL_ARRAY_BUFFER,verts);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Verts),Verts,GL_STATIC_DRAW);
    glEnableVertexAttribArray(Position);
    glVertexAttribPointer(Position,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    // Create the VBO for facet indices:
    glGenBuffers(1,&faces);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Faces),Faces,GL_STATIC_DRAW);
}

static void CreatePlane()
{
    unsigned int vao,verts,faces;
    const int Faces[] = {
      0, 1, 3,
      0, 3, 2
    };
    const float Verts[] = {
      -1.0, -1.0, 0.0,
      -1.0,  1.0, 0.0,
       1.0, -1.0, 0.0,
       1.0,  1.0, 0.0,
    };
    N = sizeof(Faces)/sizeof(int);

    // Create the VAO:
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    // Create the VBO for positions:
    glGenBuffers(1,&verts);
    glBindBuffer(GL_ARRAY_BUFFER,verts);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Verts),Verts,GL_STATIC_DRAW);
    glEnableVertexAttribArray(Position);
    glVertexAttribPointer(Position,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    // Create the VBO for facet indices:
    glGenBuffers(1,&faces);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Faces),Faces,GL_STATIC_DRAW);
}

//Check for errors
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

//Print error and exit
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Reverse n bytes
 */
static void Reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

/*
 *  Load texture from BMP file
 */
unsigned int LoadTexBMP(const char* file)
{
   unsigned int   texture;    // Texture name
   FILE*          f;          // File pointer
   unsigned short magic;      // Image magic
   unsigned int   dx,dy,size; // Image dimensions
   unsigned short nbp,bpp;    // Planes and bits per pixel
   unsigned char* image;      // Image data
   unsigned int   off;        // Image offset
   unsigned int   k;          // Counter
   int            max;        // Maximum texture dimensions

   //  Open file
   f = fopen(file,"rb");
   if (!f) Fatal("Cannot open file %s\n",file);
   //  Check image magic
   if (fread(&magic,2,1,f)!=1) Fatal("Cannot read magic from %s\n",file);
   if (magic!=0x4D42 && magic!=0x424D) Fatal("Image magic not BMP in %s\n",file);
   //  Read header
   if (fseek(f,8,SEEK_CUR) || fread(&off,4,1,f)!=1 ||
       fseek(f,4,SEEK_CUR) || fread(&dx,4,1,f)!=1 || fread(&dy,4,1,f)!=1 ||
       fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
     Fatal("Cannot read header from %s\n",file);
   //  Reverse bytes on big endian hardware (detected by backwards magic)
   if (magic==0x424D)
   {
      Reverse(&off,4);
      Reverse(&dx,4);
      Reverse(&dy,4);
      Reverse(&nbp,2);
      Reverse(&bpp,2);
      Reverse(&k,4);
   }
   //  Check image parameters
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
   if (dx<1 || dx>max) Fatal("%s image width %d out of range 1-%d\n",file,dx,max);
   if (dy<1 || dy>max) Fatal("%s image height %d out of range 1-%d\n",file,dy,max);
   if (nbp!=1)  Fatal("%s bit planes is not 1: %d\n",file,nbp);
   if (bpp!=24) Fatal("%s bits per pixel is not 24: %d\n",file,bpp);
   if (k!=0)    Fatal("%s compressed files not supported\n",file);
#ifndef GL_VERSION_2_0
   //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
   for (k=1;k<dx;k*=2);
   if (k!=dx) Fatal("%s image width not a power of two: %d\n",file,dx);
   for (k=1;k<dy;k*=2);
   if (k!=dy) Fatal("%s image height not a power of two: %d\n",file,dy);
#endif

   //  Allocate image memory
   size = 3*dx*dy;
   image = (unsigned char*) malloc(size);
   if (!image) Fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
   //  Seek to and read image
   if (fseek(f,off,SEEK_SET) || fread(image,size,1,f)!=1) Fatal("Error reading data from image %s\n",file);
   fclose(f);
   //  Reverse colors (BGR -> RGB)
   for (k=0;k<size;k+=3)
   {
      unsigned char temp = image[k];
      image[k]   = image[k+2];
      image[k+2] = temp;
   }

   //  Sanity check
   ErrCheck("LoadTexBMP");
   //  Generate 2D texture
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   //  Copy image
   glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
   if (glGetError()) Fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
   //  Scale linearly when image size doesn't match
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   //  Free image memory
   free(image);
   //  Return texture name
   return texture;
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

   //  Shader program
   CreatePlane();
   glBindTexture(GL_TEXTURE_2D, tex_terrain);
   glActiveTexture(GL_TEXTURE1);
   tex_terrain = LoadTexBMP("Qt/terrain_will.bmp");
   shader = CreateShaderProgTess();
   ErrCheck("init");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
