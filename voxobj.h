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

class VoxObj
{
public:
    VoxObj();
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
    void draw_slow(double angleX,double angleY,double angleZ);
    void draw_slow_RLE(double angleX,double angleY,double angleZ);

protected:
    double m_pos_X,m_pos_Y,m_pos_Z;
    unsigned short xsiz, ysiz, zsiz;
    //rotation, translation
    //size
    //name
    //effects
};

#endif // VOXOBJ_H
