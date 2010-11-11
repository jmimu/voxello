#include "octreecell.h"

OctreeCell::OctreeCell(OctreeCell * parent) :
        m_parent(parent),
        m_flags(0x0)
{
    for (unsigned int i=0;i<6;i++)
        m_neigh[i]=0;
    for (unsigned int i=0;i<8;i++)
        m_sons[i]=0;

    if (!parent)
        m_level=0;
    else m_level=parent->m_level+1;
}

OctreeCell::~OctreeCell()///affects neigh
{

}

void OctreeCell::ogl_render()///render via OpenGL
{

}

bool OctreeCell::create_sons()
{
    return true;
}

void OctreeCell::set_color(unsigned short col_r,unsigned short col_g,unsigned short col_b,unsigned short col_a)
{
    m_col_r=col_r;
    m_col_g=col_g;
    m_col_b=col_b;
    m_col_a=col_a;

    if (m_sons[0])
        for (unsigned int i=0;i<8;i++)
        {
            m_sons[i]->set_color(col_r,col_g,col_b,col_a);
        }
}
