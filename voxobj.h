#ifndef VOXOBJ_H
#define VOXOBJ_H

#include <string>

/**
  Object's frame of reference

  Up (z)
  |
  |   / North (y)
  |  /
  | /
  L _ _ _ _ East (x)

  scale depends on octree cell's level
  */

#include "compress_tools.h"

#define VOX_FILE 1
#define KV6_FILE 2
#define KVX_FILE 3

class VoxObj
{
public:
    VoxObj(std::string filnam,int file_format);
    ~VoxObj();
    //load form voxlap file
    //bool load_from_KV6 (char *filnam);///load Ken Silverman's Voxlap5 KV6 format
    bool load_from_VOX (std::string filnam);///load Ken Silverman's Voxlap5 KV6 format


    Column_RLE * obj;//2D array of RLE cols

    //for each column, compare to 4 close columns
    void update_flags();

    //temp:
    unsigned char *voxels;
    unsigned char palette[256][3];
    void draw_slow();
    void draw_slow_RLE();
    void set_pos(double x,double y,double z){m_pos_X=x;m_pos_Y=y;m_pos_Z=z;}
protected:
    double m_pos_X,m_pos_Y,m_pos_Z;
    unsigned short xsiz, ysiz, zsiz;
    //rotation, translation
    //size
    //name
    //effects
};

#endif // VOXOBJ_H
