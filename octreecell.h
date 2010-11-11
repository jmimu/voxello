#ifndef OCTREECELL_H
#define OCTREECELL_H

#define FACE_D_COVERED 0x01
#define FACE_U_COVERED 0x02
#define FACE_S_COVERED 0x04
#define FACE_N_COVERED 0x08
#define FACE_W_COVERED 0x10
#define FACE_E_COVERED 0x20
#define IS_COVERED 0x40
#define IS_FULL 0x80

/**
  Neighbour order :
    EWNSUD
    OctreeCell *m_at_E,*m_at_W,*m_at_N,*m_at_S,*m_at_U,*m_at_D;//neighbour

  Sons order :
    OctreeCell *m_son_WSD,*m_son_ESD,*m_son_WND,*m_son_END;
    OctreeCell *m_son_WSU,*m_son_ESU,*m_son_WNU,*m_son_ENU;

    =>  /6 7/
       /4 5/
       ------
        /2 3/
       /0 1/

  */

class OctreeCell
{
public:
    OctreeCell(OctreeCell * parent,long x_min,long y_min,long z_min,long size);
    ~OctreeCell();///affects neigh
    void ogl_render(double m_pos_X,double m_pos_Y,double m_pos_Z,long x_total_size,long y_total_size,long z_total_size);///render via OpenGL
    bool create_sons();
    void set_color(unsigned short col_r,unsigned short col_g,unsigned short col_b,unsigned short col_a);

    bool add_voxel(long x,long y,long z,unsigned short r,unsigned short g,unsigned short b,unsigned short a);
    long get_nbr_vox(){return nbr_vox;}
    long count_rendering_cells();
protected:
    OctreeCell * m_parent;
    long m_x_min,m_y_min,m_z_min,m_size;
    unsigned char m_col_r,m_col_g,m_col_b,m_col_a;//color m_col_a==0 => empty
    unsigned char m_flags;///is_full is_covered face_E_covered  face_W_covered...
    OctreeCell *m_neigh[6];

    //for sons, use an other class? (to have sons only for not final cells?
    OctreeCell *m_sons[8];
    static long nbr_vox;
};

#endif // OCTREECELL_H
