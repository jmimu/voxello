#include "ogldraw.h"

#include <iostream>

void ogldraw::init_OGL()
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(RES_X, RES_Y, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)RES_X/RES_Y,1,1000);

    glEnable(GL_DEPTH_TEST);
}

void ogldraw::begin_draw(double angleX,double angleY,double angleZ)
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //to enable transparency
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode( GL_MODELVIEW );

    glLoadIdentity( );

    gluLookAt(0,-50,0,0,0,0,0,0,1);//z is down, look in y direction

    glRotated(angleY,0,1,0);
    glRotated(angleZ,0,0,1);
    glRotated(angleX,1,0,0);

}

void ogldraw::end_draw()
{
    glFlush();
    SDL_GL_SwapBuffers();
}


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

//TODO: compute which facse are oriented to the camera, to hide the 3 others
void ogldraw::rect(double x,double y,double z,unsigned short zsize,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    //std::cout<<"ogl "<<x<<" "<<y<<" "<<z<<"  "<<size<<" "<<r*1000+g<<std::endl;
    glColor4ub(r,g,b,a);//North face
    glVertex3d(1+x,1+y,zsize+z);
    glVertex3d(1+x,1+y,      z);
    glVertex3d(  x,1+y,      z);
    glVertex3d(  x,1+y,zsize+z);

    //glColor4ub(r,g,b,a);//East face
    glVertex3d(1+x,  y,zsize+z);
    glVertex3d(1+x,  y,      z);
    glVertex3d(1+x,1+y,      z);
    glVertex3d(1+x,1+y,zsize+z);

    //glColor4ub(r,g,b,a);//South face
    glVertex3d(  x,  y,zsize+z);
    glVertex3d(  x,  y,      z);
    glVertex3d(1+x,  y,      z);
    glVertex3d(1+x,  y,zsize+z);

    //glColor4ub(r,g,b,a);//West face
    glVertex3d(  x,1+y,zsize+z);
    glVertex3d(  x,1+y,      z);
    glVertex3d(  x,  y,      z);
    glVertex3d(  x,  y,zsize+z);

    //glColor4ub(r,g,b,a);//Down face
    glVertex3d(1+x,1+y,      z);
    glVertex3d(1+x,  y,      z);
    glVertex3d(  x,  y,      z);
    glVertex3d(  x,1+y,      z);

    //glColor4ub(r,g,b,a);//Up face
    glVertex3d(1+x,  y,zsize+z);
    glVertex3d(1+x,1+y,zsize+z);
    glVertex3d(  x,1+y,zsize+z);
    glVertex3d(  x,  y,zsize+z);

}

void ogldraw::rect_flag(double x,double y,double z,unsigned short zsize,unsigned char f,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    //std::cout<<"ogl "<<x<<" "<<y<<" "<<z<<"  "<<size<<" "<<r*1000+g<<std::endl;
    glColor4ub(r,g,b,a);
    if (!(f&FACE_N_COVERED))
    {
        //glColor4ub(r,g,b,a);//North face
        glVertex3d(1+x,1+y,zsize+z);
        glVertex3d(1+x,1+y,      z);
        glVertex3d(  x,1+y,      z);
        glVertex3d(  x,1+y,zsize+z);
    }

    if (!(f&FACE_E_COVERED))
    {
        //glColor4ub(r,g,b,a);//East face
        glVertex3d(1+x,  y,zsize+z);
        glVertex3d(1+x,  y,      z);
        glVertex3d(1+x,1+y,      z);
        glVertex3d(1+x,1+y,zsize+z);
    }

    if (!(f&FACE_S_COVERED))
    {
        //glColor4ub(r,g,b,a);//South face
        glVertex3d(  x,  y,zsize+z);
        glVertex3d(  x,  y,      z);
        glVertex3d(1+x,  y,      z);
        glVertex3d(1+x,  y,zsize+z);
    }

    if (!(f&FACE_W_COVERED))
    {
        //glColor4ub(r,g,b,a);//West face
        glVertex3d(  x,1+y,zsize+z);
        glVertex3d(  x,1+y,      z);
        glVertex3d(  x,  y,      z);
        glVertex3d(  x,  y,zsize+z);
    }

    if (!(f&FACE_D_COVERED))
    {
        //glColor4ub(r,g,b,a);//Down face
        glVertex3d(1+x,1+y,      z);
        glVertex3d(1+x,  y,      z);
        glVertex3d(  x,  y,      z);
        glVertex3d(  x,1+y,      z);
    }

    if (!(f&FACE_U_COVERED))
    {
        //glColor4ub(r,g,b,a);//Up face
        glVertex3d(1+x,  y,zsize+z);
        glVertex3d(1+x,1+y,zsize+z);
        glVertex3d(  x,1+y,zsize+z);
        glVertex3d(  x,  y,zsize+z);
    }

}


