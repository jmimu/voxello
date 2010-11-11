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
    OctreeCell *m_son_ENU,*m_son_ESU,*m_son_WNU,*m_son_WSU;
    OctreeCell *m_son_END,*m_son_ESD,*m_son_WND,*m_son_WSD;
  */

class OctreeCell
{
public:
    OctreeCell(OctreeCell * parent);
    ~OctreeCell();///affects neigh
    void ogl_render();///render via OpenGL
    bool create_sons();
    void set_color(unsigned short col_r,unsigned short col_g,unsigned short col_b,unsigned short col_a);
protected:
    OctreeCell * m_parent;
    unsigned m_level;///level in octree (coord are exprimed with it)
    unsigned short m_col_r,m_col_g,m_col_b,m_col_a;//color
    unsigned short m_flags;///is_full is_covered face_E_covered  face_W_covered...
    OctreeCell *m_neigh[6];

    //for sons, use an other class? (to have sons only for not final cells?
    OctreeCell *m_sons[8];
};

#endif // OCTREECELL_H
