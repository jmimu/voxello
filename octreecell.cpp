#include "octreecell.h"

#include <iostream>

#include "ogldraw.h"


long OctreeCell::nbr_vox=0;

OctreeCell::OctreeCell(OctreeCell * parent,unsigned short x_min,unsigned short y_min,unsigned short z_min,unsigned short size) :
        m_parent(parent),
        m_flags(0x0),
        m_x_min(x_min),m_y_min(y_min),m_z_min(z_min),m_size(size),
        m_col_r(rand()%256),m_col_g(rand()%256),m_col_b(rand()%256),m_col_a(00)
{
    nbr_vox++;
#ifdef DEBUG_OctreeCell
    std::cout<<"Create OctreeCell num "<<nbr_vox<<
            " p "<<m_x_min<<" "<<m_y_min<<" "<<m_z_min<<" "<<m_size<<std::endl;
#endif //DEBUG_OctreeCell

    for (unsigned int i=0;i<6;i++)
        m_neigh[i]=0;
    for (unsigned int i=0;i<8;i++)
        m_sons[i]=0;

}

OctreeCell::~OctreeCell()///affects neigh
{
    nbr_vox--;
#ifdef DEBUG_OctreeCell
    //std::cout<<"Delete OctreeCell "<<m_x_min<<" "<<m_y_min<<" "<<m_z_min<<" "<<m_size<<std::endl;
#endif
    //call son's destructor
    if (m_sons[0])
        for (unsigned int i=0;i<8;i++)
        {
            delete m_sons[i];
        }
}


void OctreeCell::ogl_render(double m_pos_X,double m_pos_Y,double m_pos_Z,unsigned short x_half_size,unsigned short y_half_size,unsigned short z_half_size,unsigned short min_size)///render via OpenGL
{
    if ((m_sons[0])&&(m_size>min_size))
    //if (m_sons[0])
        for (unsigned int i=0;i<8;i++)
        {
            m_sons[i]->ogl_render(m_pos_X, m_pos_Y, m_pos_Z, x_half_size, y_half_size, z_half_size,min_size);
        }
    else
        if (m_col_a!=0)
            ogldraw::cube(m_x_min-x_half_size+m_pos_X,m_y_min-y_half_size+m_pos_Y,
                          m_z_min-z_half_size+m_pos_Z,m_size,m_col_r,m_col_g,m_col_b,m_col_a);
}

bool OctreeCell::create_sons()
{
/*
    =>  /6 7/
       /4 5/
       ------
        /2 3/
       /0 1/
       */
    unsigned short new_size=m_size/2;
    unsigned short x_mid=m_x_min+new_size;
    unsigned short y_mid=m_y_min+new_size;
    unsigned short z_mid=m_z_min+new_size;

    m_sons[0]=new OctreeCell(this,m_x_min,m_y_min,m_z_min,new_size);
    m_sons[1]=new OctreeCell(this,x_mid,m_y_min,m_z_min,new_size);
    m_sons[2]=new OctreeCell(this,m_x_min,y_mid,m_z_min,new_size);
    m_sons[3]=new OctreeCell(this,x_mid,y_mid,m_z_min,new_size);

    m_sons[4]=new OctreeCell(this,m_x_min,m_y_min,z_mid,new_size);
    m_sons[5]=new OctreeCell(this,x_mid,m_y_min,z_mid,new_size);
    m_sons[6]=new OctreeCell(this,m_x_min,y_mid,z_mid,new_size);
    m_sons[7]=new OctreeCell(this,x_mid,y_mid,z_mid,new_size);


    //TODO: compute neigh

    return true;
}

//used to give colour to parent
long OctreeCell::count_rendering_cells()
{
    long n=0;
    if (m_sons[0])
    {
        for (unsigned int i=0;i<8;i++)
        {
            n+=m_sons[i]->count_rendering_cells();
        }
        return n;
    }
    else
    {
        if (m_col_a!=0)
        {
            set_colour_to_parent();//used to give colour to parent
            return 1;
        }
        else
            return 0;
    }
}


long OctreeCell::count_terminal_cells()
{
    long n=0;
    if (m_sons[0])
    {
        for (unsigned int i=0;i<8;i++)
        {
            n+=m_sons[i]->count_terminal_cells();
        }
        return n;
    }
    else
    {
        return 1;
    }
}

void OctreeCell::set_color(unsigned char col_r,unsigned char col_g,unsigned char col_b,unsigned char col_a)
{
    m_col_r=col_r;
    m_col_g=col_g;
    m_col_b=col_b;
    m_col_a=col_a;

    //bad idea
  /*  if (m_sons[0])
        for (unsigned int i=0;i<8;i++)
        {
            m_sons[i]->set_color(col_r,col_g,col_b,col_a);
            m_sons[i]=0;
        }*/
}

//it may be better to give outside son's colour, not random...
void OctreeCell::set_colour_to_parent()
{
    if (m_parent)
    {
        m_parent->set_color(m_col_r,m_col_g,m_col_b,m_col_a);
        m_parent->set_colour_to_parent();
    }
}

//
bool OctreeCell::add_voxel(unsigned short x,unsigned short y,unsigned short z,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    if ((x<m_x_min)||(x>m_x_min+m_size)||(y<m_y_min)||(y>m_y_min+m_size)||(z<m_z_min)||(z>m_z_min+m_size)||(m_size==0))
    {
        std::cerr<<"Pos error! "<<x<<" "<<y<<" "<<z<<std::endl;
        return false;
    }

    if (m_size==1)
    {
#ifdef DEBUG_OctreeCell
        std::cout<<"Add voxel to "<<x<<" "<<y<<" "<<z<<std::endl;
#endif
        set_color(r,g,b,a);
        return true;
    }

    if (!(m_sons[0]))
    {
        create_sons();
        //set_color(200,200,0,a);
    }


    //find which son
    short i=0;
    if (z>=m_z_min+m_size/2) i+=4;
    if (y>=m_y_min+m_size/2) i+=2;
    if (x>=m_x_min+m_size/2) i+=1;

    m_sons[i]->add_voxel(x,y,z,r,g,b,a);


    //check if all sons are alike
    bool all_alike=true;
    for (unsigned int i=1;i<8;i++)
    {
        if ((m_sons[i]->m_col_r!=m_sons[0]->m_col_r)||(m_sons[i]->m_col_g!=m_sons[0]->m_col_g)||
            (m_sons[i]->m_col_b!=m_sons[0]->m_col_b)||(m_sons[i]->m_col_a!=m_sons[0]->m_col_a))
        {
            all_alike=false;
            break;
        }
    }
    if (all_alike)
    {
        m_col_r=m_sons[0]->m_col_r;
        m_col_g=m_sons[0]->m_col_g;
        m_col_b=m_sons[0]->m_col_b;
        m_col_a=m_sons[0]->m_col_a;
        for (unsigned int i=0;i<8;i++)
        {
            delete m_sons[i];m_sons[i]=0;
        }
    }
}

