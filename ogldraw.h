#ifndef OGLDRAW_H
#define OGLDRAW_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace ogldraw
{
    void cube(double x,double y,double z,unsigned short size,unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);
    void rect(double x,double y,double z,unsigned short zsize,unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);
}

#endif // OGLDRAW_H
