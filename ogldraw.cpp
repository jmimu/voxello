#include "ogldraw.h"


void ogldraw::cube(long x,long y,long z,unsigned char r,unsigned char g,unsigned char b)
{
    glColor3ub(r,g,b);
    glVertex3d(1+x,1+y,1+z);
    glVertex3d(1+x,1+y,z);
    glVertex3d(x,1+y,+z);
    glVertex3d(x,1+y,1+z);

    glColor3ub(r,g,b);
    glVertex3d(1+x,y,1+z);
    glVertex3d(1+x,y,z);
    glVertex3d(1+x,1+y,z);
    glVertex3d(1+x,1+y,1+z);

    glColor3ub(r,g,b);
    glVertex3d(x,y,1+z);
    glVertex3d(x,y,z);
    glVertex3d(1+x,y,z);
    glVertex3d(1+x,y,1+z);

    glColor3ub(r,g,b);
    glVertex3d(x,1+y,1+z);
    glVertex3d(x,1+y,z);
    glVertex3d(x,y,z);
    glVertex3d(x,y,1+z);

    glColor3ub(r,g,b);
    glVertex3d(1+x,1+y,z);
    glVertex3d(1+x,y,z);
    glVertex3d(x,y,z);
    glVertex3d(x,1+y,z);

    glColor3ub(r,g,b);
    glVertex3d(1+x,y,1+z);
    glVertex3d(1+x,1+y,1+z);
    glVertex3d(x,1+y,1+z);
    glVertex3d(x,y,1+z);

}
