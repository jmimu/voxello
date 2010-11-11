#include "voxobj.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "ogldraw.h"


VoxObj::VoxObj() :
    m_pos_X(0.0),m_pos_Y(0.0),m_pos_Z(0.0),xsiz(0), ysiz(0), zsiz(0)
{
    //DESKLAMP  dopefish  duke  globe  pawn  strongbad
    load_from_VOX("data/strongbad.vox");
}


///load Ken Silverman's VOX format
//todo: bad to load the whole file in one time
long VoxObj::load_from_VOX (char *filnam)
{
    FILE *fil;
    //unsigned char *voxels;


    fil = fopen(filnam,"rb"); if (!fil) return(-1);
    fread(&xsiz,4,1,fil); //size
    fread(&ysiz,4,1,fil); //size
    fread(&zsiz,4,1,fil); //size

    voxels = (unsigned char *)malloc(xsiz*ysiz*zsiz);

    fread(voxels,xsiz*ysiz*zsiz,1,fil); //The 3-D array itself!

    fread(palette,768,1,fil);          //VGA palette (values range from 0-63)
    fclose(fil);
    //colour 255 is for empty space

    //change palette form 18 to 24 bits
    for (int i=0;i<255;i++)
    {
        palette[i][0]=palette[i][0]<<2;
        palette[i][1]=palette[i][1]<<2;
        palette[i][2]=palette[i][2]<<2;
    }

    std::cout<<filnam<<": got "<<xsiz<<"*"<<ysiz<<"*"<<zsiz<<"voxels"<<std::endl;
    return 0;
}

void VoxObj::draw_slow(double angleX,double angleY,double angleZ)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(0,-zsiz,0,0,0,0,0,0,1);//z is up, look in y direction

    glRotated(angleZ,0,0,1);
    glRotated(angleY,0,1,0);
    glRotated(angleX,1,0,0);

    glBegin(GL_QUADS);

    unsigned char v;
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
            for (long z=0;z<zsiz;z++)
            {
                v=voxels[x*ysiz*zsiz+y*zsiz+z];
                if (v!=255)
                    ogldraw::cube(x-xsiz/2,y-ysiz/2,-z+zsiz/2,palette[v][0],palette[v][1],palette[v][2]);
            }


    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();
}


#ifdef KV6

/*
//Called at least once for every voxel of the board
//issolid: 0:air, 1:solid
void setgeom (long x, long y, long z, long issolid)
{
    //your code here
}

//Called only for surface voxels
//A surface voxel is any solid voxel with at least 1 air voxel
//  on one of its 6 sides. All solid voxels at z=0 are automatically
//  surface voxels, but this is not true for x=0, x=1023, y=0, y=1023,
//  z=255 (I believe)
//argb: 32-bit color, high byte is used for shading scale (can be ignored)
void setcol (long x, long y, long z, long argb)
{
    //printf("%d %d %d: %08x\n",x,y,z,argb);
}
*/

///load Ken Silverman's Voxlap5 KV6 format
long VoxObj::load_from_KV6 (char *filnam)
{
    FILE *fil;
    long leng, xsiz, ysiz, zsiz;
    float xpiv, ypiv, zpiv;
    unsigned long numvoxs;
    long namoff;
    //kv6data *lowermip;
    kv6voxtype *vox;      //numvoxs*sizeof(kv6voxtype)
    unsigned long *xlen;  //xsiz*sizeof(long)
    unsigned short *ylen; //xsiz*ysiz*sizeof(short)

    long i;
    unsigned char *v, *vbuf;

    fil = fopen(filnam,"rb"); if (!fil) return(-1);

    //to get file size...
    fseek(fil, 0, SEEK_END); // seek to end of file
    long file_size = ftell(fil); // get current file pointer
    fseek(fil, 0, SEEK_SET); // seek back to beginning of file


    fread(&i,4,1,fil); if (i != 0x6c78764b) return(-1);
    fread(&xsiz,4,1,fil); //size
    fread(&ysiz,4,1,fil); //size
    fread(&zsiz,4,1,fil); //size
    fread(&xpiv,4,1,fil); //piv
    fread(&ypiv,4,1,fil); //piv
    fread(&zpiv,4,1,fil); //piv

    fread(&numvoxs,4,1,fil); //numvoxs

    struct struc{ int a;int b; };
    struc *t;
    t=(struc*)malloc(5*sizeof(struc));
    t[0].a=1;
    t[0].b=2;
    t[4].a=3;
    t[1].b=4;//meme si le debugger affiche qu'un truc, il y en a bien 5

    i = numvoxs*sizeof(kv6voxtype);
    vox=(kv6voxtype*)malloc(i);
    vbuf = (unsigned char *)malloc(i);
    if (!vbuf)
    {
        fclose(fil);
        return(-1);
    }
    fread(vbuf,i,1,fil);

    //vox=(kv6voxtype*)vbuf;


    i = xsiz*sizeof(long);
    xlen = (unsigned long *)malloc(i);
    if (!xlen)
    {
        fclose(fil);
        return(-1);
    }
    fread(xlen,i,1,fil);

    i = xsiz*ysiz*sizeof(short);
    ylen = (unsigned short *)malloc(i);
    if (!ylen)
    {
        fclose(fil);
        return(-1);
    }
    fread(ylen,i,1,fil);

    fclose(fil);
    return 0;


/*    typedef struct { double x, y, z; } dpoint3d;
    dpoint3d ipos, istr, ihei, ifor;

   FILE *fil;
   long i, x, y, z;
   unsigned char *v, *vbuf;

   fil = fopen(filnam,"rb"); if (!fil) return(-1);

   //to get file size...
   fseek(fil, 0, SEEK_END); // seek to end of file
   long file_size = ftell(fil); // get current file pointer
   fseek(fil, 0, SEEK_SET); // seek back to beginning of file


   fread(&i,4,1,fil); if (i != 0x09072000) return(-1);
   fread(&i,4,1,fil); if (i != 1024) return(-1);
   fread(&i,4,1,fil); if (i != 1024) return(-1);
   fread(&ipos,24,1,fil); //camera position
   fread(&istr,24,1,fil); //unit right vector
   fread(&ihei,24,1,fil); //unit down vector
   fread(&ifor,24,1,fil); //unit forward vector

      //Allocate huge buffer and load rest of file into it...


   i = file_size-ftell(fil);
   vbuf = (unsigned char *)malloc(i); if (!vbuf) { fclose(fil); return(-1); }
   fread(vbuf,i,1,fil);
   fclose(fil);

      //Set entire board to solid
   for(z=0;z<256;z++)
      for(y=0;y<1024;y++)
         for(x=0;x<1024;x++)
            setgeom(x,y,z,1);

   v = vbuf;
   for(y=0;y<1024;y++)
      for(x=0;x<1024;x++)
      {
         z = 0;
         while (1)
         {
            for(i=z;i<v[1];i++) setgeom(x,y,i,0);
            for(z=v[1];z<=v[2];z++) setcol(x,y,z,*(long *)&v[(z-v[1]+1)<<2]);
            if (!v[0]) break; z = v[2]-v[1]-v[0]+2; v += v[0]*4;
            for(z+=v[3];z<v[3];z++) setcol(x,y,z,*(long *)&v[(z-v[3])<<2]);
         }
         v += ((((long)v[2])-((long)v[1])+2)<<2);
      }

   free(vbuf);
*/
}
#endif

