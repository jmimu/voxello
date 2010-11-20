#ifndef OGLDRAW_H
#define OGLDRAW_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "compress_tools.h"


#define RES_X 640
#define RES_Y 480


namespace ogldraw
{
    void init_OGL();
    void begin_draw(double angleX,double angleY,double angleZ);
    void end_draw();

    void cube(double x,double y,double z,unsigned short size,unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);
    void rect(double x,double y,double z,unsigned short zsize,unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);

    void rect_flag(double x,double y,double z,unsigned short zsize,unsigned char f,unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);

}

#endif // OGLDRAW_H
