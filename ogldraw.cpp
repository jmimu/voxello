#include "ogldraw.h"


void ogldraw::cube(double x,double y,double z,long size,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    glColor4ub(r,g,b,a);//North face
    glVertex3d(size+x,size+y,1+z);
    glVertex3d(size+x,size+y,z);
    glVertex3d(x,1+y,+z);
    glVertex3d(x,1+y,1+z);

    glColor4ub(r,g,b,a);//East face
    glVertex3d(size+x,y,size+z);
    glVertex3d(size+x,y,z);
    glVertex3d(size+x,size+y,z);
    glVertex3d(size+x,size+y,size+z);

    glColor4ub(r,g,b,a);//South face
    glVertex3d(x,y,size+z);
    glVertex3d(x,y,z);
    glVertex3d(size+x,y,z);
    glVertex3d(size+x,y,size+z);

    glColor4ub(r,g,b,a);//West face
    glVertex3d(x,size+y,size+z);
    glVertex3d(x,size+y,z);
    glVertex3d(x,y,z);
    glVertex3d(x,y,size+z);

    glColor4ub(r,g,b,a);//Down face
    glVertex3d(size+x,size+y,z);
    glVertex3d(size+x,y,z);
    glVertex3d(x,y,z);
    glVertex3d(x,size+y,z);

    glColor4ub(r,g,b,a);//Up face
    glVertex3d(size+x,y,size+z);
    glVertex3d(size+x,size+y,size+z);
    glVertex3d(x,size+y,size+z);
    glVertex3d(x,y,size+z);

}
