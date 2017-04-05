//
//  Plane class
//
#include "plane.h"

//
//  Constructor
//
Plane::Plane(int segments, const QString tex)
{
   x = 1;
   y = 1;
   segs = segments;
   if (tex.length()>0) setTexture(tex);
   setScale(6.0, 6.0);
   setRotate(90, 0,0,1);
}

//
//  Set scaling
//
void Plane::setScale(float dx,float dy)
{
   x = dx;
   y = dy;
}

//
//  Display the plane
//
void Plane::display()
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   useTransform(x,y,1.0);
   //  Texture on
   EnableTex();
   //Increment i by segment size
   float segsize = 1.0/segs;

   //  Bottom
   useColor(Color(1,1,1));
   glBegin(GL_TRIANGLES);
   glNormal3f( 0, 1, 0);
   for(float i = -0.5; i < 0.5; i+= segsize) {
       for(float j = -0.5; j < 0.5; j+= segsize) {
           glTexCoord2f(i+0.5, j+0.5); glVertex3f(i, j, 0.01);
           glTexCoord2f(i+0.5, j+0.5+segsize); glVertex3f(i, j+segsize, 0.01);
           glTexCoord2f(i+0.5+segsize, j+0.5); glVertex3f(i+segsize, j, 0.01);
           glTexCoord2f(i+0.5+segsize, j+0.5); glVertex3f(i+segsize, j, 0.01);
           glTexCoord2f(i+0.5, j+0.5+segsize); glVertex3f(i, j+segsize, 0.01);
           glTexCoord2f(i+0.5+segsize, j+0.5+segsize); glVertex3f(i+segsize, j+segsize, 0.01);

       }
   }
   glEnd();

   //Lines
   /*useColor(Color(0,0,0));
   glBegin(GL_LINES);
   glNormal3f( 0, 1, 0);
   for(float i = -0.5; i < 0.5; i+= segsize) {
       for(float j = -0.5; j < 0.5; j+= segsize) {
           glTexCoord2f(i+0.5, j+0.5); glVertex3f(i, j, 0);
           glTexCoord2f(i+0.5, j+0.5+segsize); glVertex3f(i, j+segsize, 0);
           glTexCoord2f(i+0.5+segsize, j+0.5); glVertex3f(i+segsize, j, 0);
           glTexCoord2f(i+0.5+segsize, j+0.5); glVertex3f(i+segsize, j, 0);
           glTexCoord2f(i+0.5, j+0.5+segsize); glVertex3f(i, j+segsize, 0);
           glTexCoord2f(i+0.5+segsize, j+0.5+segsize); glVertex3f(i+segsize, j+segsize, 0);
       }
   }
   glEnd();
   */

   //  Texture off
   DisableTex();
   //  Undo transofrmations
   glPopMatrix();
}

