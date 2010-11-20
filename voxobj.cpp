#include "voxobj.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "ogldraw.h"

//#define DRAW_EMPTY 10

VoxObj::VoxObj(std::string filnam,int file_format) :
        voxels(0),
        m_pos_X(0.0),m_pos_Y(0.0),m_pos_Z(0.0),xsiz(0), ysiz(0), zsiz(0)
{
    //DESKLAMP  dopefish  duke  globe  pawn  strongbad CHAIR1
    //load_from_VOX("data/duke.vox");
    switch (file_format)
    {
    case VOX_FILE:
        load_from_VOX(filnam);
        break;
    default:
        std::cout<<"Bad file format: "<<filnam<<" format "<<file_format<<std::endl;
    }

    std::cout<<"VoxObj created."<<std::endl;
    std::cout<<"Vox_RLE size: "<<sizeof(Vox_RLE)<<std::endl;
}


VoxObj::~VoxObj()
{
    if (voxels)
        free(voxels);
}

///load Ken Silverman's VOX format
//todo: bad to load the whole file in one time
bool VoxObj::load_from_VOX (std::string filnam)
{
    FILE *fil;
    //unsigned char *voxels;


    fil = fopen(filnam.c_str(),"rb"); if (!fil) return(false);
    fread(&xsiz,4,1,fil); //size
    fread(&ysiz,4,1,fil); //size
    fread(&zsiz,4,1,fil); //size

    voxels = (unsigned char *)malloc(xsiz*ysiz*zsiz*sizeof(unsigned char));

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
    //just for tests: change black (inside) to yellow
    //palette[0][0]=200;    palette[0][1]=200;    palette[0][2]=00;


    std::cout<<filnam<<": got "<<xsiz<<"*"<<ysiz<<"*"<<zsiz<<"voxels"<<std::endl;

    long nbr_vox=0;

    unsigned char v;
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
            for (long z=0;z<zsiz;z++)
            {
                v=voxels[x*ysiz*zsiz+y*zsiz+z];
                if (v!=255)
                {
                    //std::cout<<"Try to add voxel "<<x<<" "<<y<<" "<<z<<std::endl;
                    nbr_vox++;
                }
                //else
                //    m_octree->add_voxel(x,y,z,palette[v][0],palette[v][1],palette[v][2],100);
            }

    std::cout<<"Total: "<<xsiz*ysiz*zsiz<<" voxels"<<std::endl;

    std::cout<<"Simple: "<<nbr_vox<<" rendered voxels"<<std::endl;

    long total_voxRLE=0;

    //create RLE object
    obj = (Column_RLE*)malloc(xsiz*ysiz*sizeof(Column_RLE));
    //now fill the column matrix
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
        {
            //create the voxel column
            Vox * column;
            column = (Vox *)malloc(zsiz*sizeof(Vox));
            for (long z=0;z<zsiz;z++)
            {
                v=voxels[x*ysiz*zsiz+y*zsiz+z];
                column[z].c=v;
                column[z].f=0;
            }
            //then compress it
            Vox_RLE * data_RLE;
            unsigned short size_RLE=Compress_Tools::zip_RLE(column,zsiz,&data_RLE);
            free(column);
            obj[x+y*xsiz].data=data_RLE;
            obj[x+y*xsiz].nbr_data=size_RLE;
            total_voxRLE+=size_RLE;
        }

    std::cout<<"Number of Vox_RLE: "<<total_voxRLE<<std::endl;

    //TODO: update vox flags
    update_flags();

    return true;
}


void VoxObj::update_flags()
{
    //std::cout<<"Begin update flags\n";

    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
        { //TODO: optimize not unzipping all every time?
            //std::cout<<"   "<<x<<" "<<y<<" ";

            Vox * column;
            Vox * west_column=0;
            Vox * east_column=0;
            Vox * south_column=0;
            Vox * north_column=0;

            //unzip the 4 columns
            Compress_Tools::unzip_RLE(obj[x+y*xsiz].data,obj[x+y*xsiz].nbr_data,&column);
            if (x>0)
                Compress_Tools::unzip_RLE(obj[x-1+y*xsiz].data,obj[x-1+y*xsiz].nbr_data,&west_column);
            if (x<xsiz-1)
                Compress_Tools::unzip_RLE(obj[x+1+y*xsiz].data,obj[x+1+y*xsiz].nbr_data,&east_column);
            if (y>0)
                Compress_Tools::unzip_RLE(obj[x+(y-1)*xsiz].data,obj[x+(y-1)*xsiz].nbr_data,&south_column);
            if (y<ysiz-1)
                Compress_Tools::unzip_RLE(obj[x+(y+1)*xsiz].data,obj[x+(y+1)*xsiz].nbr_data,&north_column);

            //can be optimized if not working on empty voxels
            for (long z=0;z<zsiz;z++)
            {
                Vox * w_vox=0;
                Vox * e_vox=0;
                Vox * s_vox=0;
                Vox * n_vox=0;
                Vox * d_vox=0;
                Vox * u_vox=0;

                if(west_column)
                    w_vox=&west_column[z];
                if(east_column)
                    e_vox=&east_column[z];
                if(south_column)
                    s_vox=&south_column[z];
                if(north_column)
                    n_vox=&north_column[z];

                if (z>0)
                    d_vox=&column[z-1];
                if (z<zsiz-1)
                    u_vox=&column[z+1];

                //we have every neigh; just have to check
                unsigned short f=0;
                if ((w_vox) && (w_vox->c!=255))
                    f|=FACE_W_COVERED;
                if ((e_vox) && (e_vox->c!=255))
                    f|=FACE_E_COVERED;
                if ((s_vox) && (s_vox->c!=255))
                    f|=FACE_S_COVERED;
                if ((n_vox) && (n_vox->c!=255))
                    f|=FACE_N_COVERED;
                if ((d_vox) && (d_vox->c!=255))
                    f|=FACE_D_COVERED;
                if ((u_vox) && (u_vox->c!=255))
                    f|=FACE_U_COVERED;

                column[z].f=f;
            }
            free(obj[x+y*xsiz].data);//delete previous RLE column
            //compress the column
            obj[x+y*xsiz].nbr_data=Compress_Tools::zip_RLE(column,zsiz,&(obj[x+y*xsiz].data));

/*            if (east_column)
                std::cout<<"east_column ";
            if (west_column)
                std::cout<<"west_column ";
            if (south_column)
                std::cout<<"south_column ";
            if (north_column)
                std::cout<<"north_column ";
            std::cout<<std::endl;*/

            //delete all that
            free(column);
            if (east_column)
                free(east_column);
            if (west_column)
                free(west_column);
            if (south_column)
                free(south_column);
            if (north_column)
                free(north_column);
        }
}



void VoxObj::draw_slow()
{
/*    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //to enable transparency
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(0,-zsiz,0,0,0,0,0,0,1);//z is down, look in y direction

    glRotated(angleY,0,1,0);
    glRotated(angleZ,0,0,1);
    glRotated(angleX,1,0,0);
*/
    glBegin(GL_QUADS);


    unsigned char v;
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
            for (long z=0;z<zsiz;z++)
            {
                v=voxels[x*ysiz*zsiz+y*zsiz+z];
                if (v!=255)
                    ogldraw::cube(x-xsiz/2,y-ysiz/2,-z+zsiz/2,1,palette[v][0],palette[v][1],palette[v][2]);
#ifdef DRAW_EMPTY
                else
                    ogldraw::cube(x-xsiz/2,y-ysiz/2,-z+zsiz/2,1,palette[v][0],palette[v][1],palette[v][2],DRAW_EMPTY);
#endif
            }


    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();

}


void VoxObj::draw_slow_RLE()
{
    //glLoadIdentity( );

    //gluLookAt(0,-zsiz,0,0,0,0,0,0,1);//z is down, look in y direction

    //glRotated(angleY,0,1,0);
    //glRotated(angleZ,0,0,1);
    //glRotated(angleX,1,0,0);

    glBegin(GL_QUADS);

    unsigned char v;//value = color of voxel
    unsigned char f;//value = flags of voxel
    unsigned short n;//number of same voxels in a raw = rectangle height

    //long nbr_rendered=0;
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
        {
            long z=0;
            for (long i=0;i<obj[x+y*xsiz].nbr_data;i++)
            {
                v=obj[x+y*xsiz].data[i].vox.c;
                f=obj[x+y*xsiz].data[i].vox.f;
                n=obj[x+y*xsiz].data[i].nbr;
                if ((v!=255)&&((f&ALL_FACES_COVERED)!=ALL_FACES_COVERED)) //simplify this test with an other flag?
                {
                    ogldraw::rect_flag(x-xsiz/2+m_pos_X,y-ysiz/2+m_pos_Y,z-zsiz/2+m_pos_Z,n,f,palette[v][0],palette[v][1],palette[v][2]);
                    //nbr_rendered++;
                }
#ifdef DRAW_EMPTY
                else
                    ogldraw::rect(x-xsiz/2+m_pos_X,y-ysiz/2+m_pos_Y,z-zsiz/2+m_pos_Z,n,palette[v][0],palette[v][1],palette[v][2],DRAW_EMPTY);
#endif
                z+=n;
            }
        }


    glEnd();
    //std::cout<<"rendered: "<<nbr_rendered<<std::endl;


}



#ifdef KV6

/*
//Called at least once for every voxel of the board
//issolid: 0:air, 1:solid
void setgeom (unsigned short x, unsigned short y, unsigned short z, unsigned short issolid)
{
    //your code here
}

//Called only for surface voxels
//A surface voxel is any solid voxel with at least 1 air voxel
//  on one of its 6 sides. All solid voxels at z=0 are automatically
//  surface voxels, but this is not true for x=0, x=1023, y=0, y=1023,
//  z=255 (I believe)
//argb: 32-bit color, high byte is used for shading scale (can be ignored)
void setcol (unsigned short x, unsigned short y, unsigned short z, unsigned short argb)
{
    //printf("%d %d %d: %08x\n",x,y,z,argb);
}
*/

///load Ken Silverman's Voxlap5 KV6 format
bool VoxObj::load_from_KV6 (char *filnam)
{
    FILE *fil;
    unsigned short leng, xsiz, ysiz, zsiz;
    float xpiv, ypiv, zpiv;
    unsigned unsigned short numvoxs;
    unsigned short namoff;
    //kv6data *lowermip;
    kv6voxtype *vox;      //numvoxs*sizeof(kv6voxtype)
    unsigned unsigned short *xlen;  //xsiz*sizeof(unsigned short)
    unsigned short *ylen; //xsiz*ysiz*sizeof(short)

    unsigned short i;
    unsigned char *v, *vbuf;

    fil = fopen(filnam,"rb"); if (!fil) return(false);

    //to get file size...
    fseek(fil, 0, SEEK_END); // seek to end of file
    unsigned short file_size = ftell(fil); // get current file pointer
    fseek(fil, 0, SEEK_SET); // seek back to beginning of file


    fread(&i,4,1,fil); if (i != 0x6c78764b) return(false);
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


    i = xsiz*sizeof(unsigned short);
    xlen = (unsigned unsigned short *)malloc(i);
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
    return true;


/*    typedef struct { double x, y, z; } dpoint3d;
    dpoint3d ipos, istr, ihei, ifor;

   FILE *fil;
   unsigned short i, x, y, z;
   unsigned char *v, *vbuf;

   fil = fopen(filnam,"rb"); if (!fil) return(-1);

   //to get file size...
   fseek(fil, 0, SEEK_END); // seek to end of file
   unsigned short file_size = ftell(fil); // get current file pointer
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
            for(z=v[1];z<=v[2];z++) setcol(x,y,z,*(unsigned short *)&v[(z-v[1]+1)<<2]);
            if (!v[0]) break; z = v[2]-v[1]-v[0]+2; v += v[0]*4;
            for(z+=v[3];z<v[3];z++) setcol(x,y,z,*(unsigned short *)&v[(z-v[3])<<2]);
         }
         v += ((((unsigned short)v[2])-((unsigned short)v[1])+2)<<2);
      }

   free(vbuf);
*/
}
#endif

