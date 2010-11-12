#include "ogldraw.h"

#include <iostream>

//TODO: compute which facse are oriented to the camera, to hide the 3 others
void ogldraw::cube(double x,double y,double z,unsigned short size,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    //std::cout<<"ogl "<<x<<" "<<y<<" "<<z<<"  "<<size<<" "<<r*1000+g<<std::endl;
    glColor4ub(r,g,b,a);//North face
    glVertex3d(size+x,size+y,size+z);
    glVertex3d(size+x,size+y,     z);
    glVertex3d(     x,size+y,     z);
    glVertex3d(     x,size+y,size+z);

    //glColor4ub(r,g,b,a);//East face
    glVertex3d(size+x,     y,size+z);
    glVertex3d(size+x,     y,     z);
    glVertex3d(size+x,size+y,     z);
    glVertex3d(size+x,size+y,size+z);

    //glColor4ub(r,g,b,a);//South face
    glVertex3d(     x,     y,size+z);
    glVertex3d(     x,     y,     z);
    glVertex3d(size+x,     y,     z);
    glVertex3d(size+x,     y,size+z);

    //glColor4ub(r,g,b,a);//West face
    glVertex3d(     x,size+y,size+z);
    glVertex3d(     x,size+y,     z);
    glVertex3d(     x,     y,     z);
    glVertex3d(     x,     y,size+z);

    //glColor4ub(r,g,b,a);//Down face
    glVertex3d(size+x,size+y,     z);
    glVertex3d(size+x,     y,     z);
    glVertex3d(     x,     y,     z);
    glVertex3d(     x,size+y,     z);

    //glColor4ub(r,g,b,a);//Up face
    glVertex3d(size+x,     y,size+z);
    glVertex3d(size+x,size+y,size+z);
    glVertex3d(     x,size+y,size+z);
    glVertex3d(     x,     y,size+z);

}
