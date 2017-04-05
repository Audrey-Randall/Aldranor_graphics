//
//  OpenGL Aldranor_opengl Widget
//

#ifndef Aldranor_OPENGL_H
#define Aldranor_OPENGL_H

#include "CUgl.h"
#include <QMatrix4x4>

class Aldranor_opengl : public CUgl
{
Q_OBJECT
private:
   unsigned int cube_tex;       //  Textures
   unsigned int terrain_tex;    //  Height map for terrain
   QGLBuffer cube_buffer;       //  Cube vertex buffer
   QGLBuffer plane_buffer;      //  Plane vertex buffer
   QMatrix4x4 proj;             //  Projection matrix
   QGLFunctions* glf;           //  Silly thing Windows needs to use glActiveTexture
public:
   Aldranor_opengl(QWidget* parent=0);                  //  Constructor
   QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
protected:
   void initializeGL();  //  Initialize widget
   void paintGL();       //  Draw widget
   void doProjection();  //  Projection
};

#endif
