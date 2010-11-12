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
    WESNDU
      5
      |  3
 0 _ _|/_ _ 1
     /|
    / |
   2  4
    OctreeCell *m_at_W,*m_at_E,*m_at_S,*m_at_N,*m_at_D,*m_at_U;//neighbour
    We try to find a neighbour with the same size. If there is not, take a bigger cell.
    Every time a cell is cut, try to find neighbour with the good size on both sides.


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
    OctreeCell(OctreeCell * parent,unsigned short x_min,unsigned short y_min,unsigned short z_min,unsigned short size);
    ~OctreeCell();///affects neigh
    void ogl_render(double m_pos_X,double m_pos_Y,double m_pos_Z,unsigned short x_half_size,unsigned short y_half_size,unsigned short z_half_size,unsigned short min_size);///render via OpenGL
    bool create_sons();
    void set_color(unsigned char col_r,unsigned char col_g,unsigned char col_b,unsigned char col_a);
    void set_colour_to_parent();//at enable lower resolutions

    bool add_voxel(unsigned short x,unsigned short y,unsigned short z,unsigned char r,unsigned char g,unsigned char b,unsigned char a);
    long get_nbr_vox(){return nbr_vox;}
    long count_rendering_cells();
    long count_terminal_cells();
    bool update_covered();//if all neighs are full
protected:
    OctreeCell * m_parent;
    unsigned short m_x_min,m_y_min,m_z_min,m_size;
    unsigned char m_col_r,m_col_g,m_col_b,m_col_a;//color m_col_a==0 => empty
    unsigned char m_flags;///is_full is_covered face_E_covered  face_W_covered...
    OctreeCell *m_neigh[6];

    //for sons, use an other class? (to have sons only for not final cells?
    OctreeCell *m_sons[8];
    static long nbr_vox;

    static short fast_neigh_matrix[8][6][2];//to compute quickly neigh
};

#endif // OCTREECELL_H
