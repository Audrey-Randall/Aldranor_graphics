#ifndef Aldranor_V_H
#define Aldranor_V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "Aldranor_opengl.h"

class Aldranor_viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*     Lpos;
   QSlider*     Zpos;
   QPushButton* light;
   Aldranor_opengl*  ogl;
private slots:
   void lmove();        //  Light movement
public:
    Aldranor_viewer();
};

#endif
