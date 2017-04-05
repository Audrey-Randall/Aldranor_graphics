//Plane class: draws plane from triangles

#ifndef PLANE_H
#define PLANE_H


#include "Object.h"

class Plane: public Object
{
private:
   float x, y;  //  Dimensions in X/Y/Z directions
   int segs;    //Number of quads per row (double this for number of polygons, they're triangles)
public:
   Plane(int segments = 50, const QString tex="");                 //  Constructor. Values of parameters are default arguments. So can call it like Plane()?
   void setScale(float dx, float dy);  //  Set scale
   void display();                             //  Render the cube
};

#endif // PLANE_H

