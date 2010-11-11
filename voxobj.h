#ifndef VOXOBJ_H
#define VOXOBJ_H

#include "octreecell.h"

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



class VoxObj
{
public:
    VoxObj();
    //load form voxlap file
    //long load_from_KV6 (char *filnam);///load Ken Silverman's Voxlap5 KV6 format
    long load_from_VOX (char *filnam);///load Ken Silverman's Voxlap5 KV6 format

    //temp:
    unsigned char *voxels;
    unsigned char palette[256][3];
    void draw_slow(double angleX,double angleY,double angleZ);

protected:
    double m_pos_X,m_pos_Y,m_pos_Z;
    long xsiz, ysiz, zsiz;
    //rotation, translation
    //size
    OctreeCell *m_octree;///octree entree
    //name
    //effects
};

#endif // VOXOBJ_H
