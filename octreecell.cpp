#include "octreecell.h"

#include <iostream>

#include "ogldraw.h"

//if DRAW_EMPTY defined, empty cells are rendered with alpha = DRAW_EMPTY
//#define DRAW_EMPTY 100
//#define DEBUG_OctreeCell

//to compute quickly neigh among sons :
//dim 1: number of the son
//dim 2: num of the neigh
//dim 3: 0: parent's neig (-1 if own parent); 1: number of the son of the neigh of the parent
short OctreeCell::fast_neigh_matrix[8][6][2]=
{{ { 0,1}, {-1,1}, { 2,2}, {-1,2}, { 4,4}, {-1,4} },
 { {-1,0}, { 1,0}, { 2,3}, {-1,3}, { 4,5}, {-1,5} },
 { { 0,3}, {-1,3}, {-1,0}, { 3,0}, { 4,6}, {-1,6} },
 { {-1,2}, { 1,2}, {-1,1}, { 3,1}, { 4,7}, {-1,7} },
 { { 0,5}, {-1,5}, { 2,6}, {-1,6}, {-1,0}, { 5,0} },
 { {-1,4}, { 1,4}, { 2,7}, {-1,7}, {-1,1}, { 5,1} },
 { { 0,7}, {-1,7}, {-1,4}, { 3,4}, {-1,2}, { 5,2} },
 { {-1,6}, { 1,6}, {-1,5}, { 3,5}, {-1,3}, { 5,3} }};

long OctreeCell::nbr_vox=0;

OctreeCell::OctreeCell(OctreeCell * parent,unsigned short x_min,unsigned short y_min,unsigned short z_min,unsigned short size) :
        m_parent(parent),
        m_x_min(x_min),m_y_min(y_min),m_z_min(z_min),m_size(size),
        m_col_r(rand()%256),m_col_g(rand()%256),m_col_b(rand()%256),m_col_a(00),
        m_flags(0x0),vox_num(nbr_vox++)
{
#ifdef DEBUG_OctreeCell
    std::cout<<"Create OctreeCell num "<<nbr_vox<<
            " p "<<m_x_min<<" "<<m_y_min<<" "<<m_z_min<<" "<<m_size<<std::endl;
#endif //DEBUG_OctreeCell

    for (unsigned int i=0;i<6;i++)
        m_neigh[i]=0;
    for (unsigned int i=0;i<8;i++)
        m_sons[i]=0;

}

void OctreeCell::dump()//write info
{
    std::cout<<"Cell "<<vox_num<<" "<<m_x_min<<" "<<m_y_min<<" "<<m_z_min<<": "<<m_size<<", f"<<(m_flags&IS_FULL)<<" c"<<(m_flags&IS_COVERED)<<"\n    ";
    std::cout<<"parent: ";
    if (m_parent)
        std::cout<<m_parent->vox_num<<"  ";
    else
        std::cout<<"    ";
    for (unsigned int i=0;i<6;i++)
    {
        if (m_neigh[i])
            std::cout<<"*"<<m_neigh[i]->vox_num;
        else
            std::cout<<"*?";
    }
    std::cout<<std::endl;
}


OctreeCell::~OctreeCell()///affects neigh
{
    //nbr_vox--;
#ifdef DEBUG_OctreeCell
    //std::cout<<"Delete OctreeCell "<<m_x_min<<" "<<m_y_min<<" "<<m_z_min<<" "<<m_size<<std::endl;
    //dump();
#endif
    //call son's destructor
    if (m_sons[0])
        for (unsigned int i=0;i<8;i++)
        {
            delete m_sons[i];
        }

    //update neigh
    for (unsigned int dir=0;dir<3;dir++)
        for (unsigned int way=0;way<2;way++)
        {
            if (m_neigh[(dir<<1)+way])
            {
                /*std::cout<<"update neigh "<<m_neigh[(dir<<1)+way]->vox_num<<" "
                        <<m_neigh[(dir<<1)+way]->m_neigh[(dir<<1)+(1-way)]->vox_num<<"  => "
                        <<m_parent->vox_num<<std::endl;*/
                if (m_neigh[(dir<<1)+way]->m_parent!=m_parent)//change neigh only for far neighs!
                    m_neigh[(dir<<1)+way]->m_neigh[(dir<<1)+(1-way)]=m_parent;//the neigh of the neigh is the parent
            }
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
    {
        if ((m_col_a!=0)&&(!(m_flags&IS_COVERED)))
            ogldraw::cube(m_x_min-x_half_size+m_pos_X,m_y_min-y_half_size+m_pos_Y,
                          m_z_min-z_half_size+m_pos_Z,m_size,m_col_r,m_col_g,m_col_b,m_col_a);
#ifdef DRAW_EMPTY
        if ((m_col_a==0))//&&(m_flags&IS_COVERED))
            ogldraw::cube(m_x_min-x_half_size+m_pos_X,m_y_min-y_half_size+m_pos_Y,
                          m_z_min-z_half_size+m_pos_Z,m_size,m_col_r,m_col_g,m_col_b,DRAW_EMPTY);
#endif
    }

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



    //update neigh
    for (unsigned int son=0;son<8;son++)
    {
        for (unsigned int neigh=0;neigh<6;neigh++)
        {
            if (fast_neigh_matrix[son][neigh][0]==-1)//close neigh, no need to update invert
                m_sons[son]->m_neigh[neigh]=m_sons[fast_neigh_matrix[son][neigh][1]];
            else//check neig of the parent
            {
                if (this->m_neigh[fast_neigh_matrix[son][neigh][0]]) //parent's neig exists
                    if (this->m_neigh[fast_neigh_matrix[son][neigh][0]]->m_sons[0]) //it has sons
                    {
                        m_sons[son]->m_neigh[neigh]=this->m_neigh[fast_neigh_matrix[son][neigh][0]]->m_sons[fast_neigh_matrix[son][neigh][1]];
                        //check is size is the same for both
                        //invert of neigh is neigh^1!
                        if (m_sons[son]->m_neigh[neigh]->m_size == m_sons[son]->m_size) //same size, update invert neigh
                        {
                            m_sons[son]->m_neigh[neigh]->m_neigh[ neigh^1 ]=m_sons[son];
                            //and check its covered
                            //m_sons[son]->m_neigh[neigh]->m_neigh[ neigh^1 ]->update_covered();
                        }
                    }
                    else //no sons => neigh is parent's neigh (not same size)
                        m_sons[son]->m_neigh[neigh]=this->m_neigh[fast_neigh_matrix[son][neigh][0]];
                else //no parent's neig
                    m_sons[son]->m_neigh[neigh]=0;
            }
        }
        //m_sons[son]->update_covered();
    }


    return true;
}

//call to the parent only when a leaf is not empty
void OctreeCell::update_parent_is_full()//recursive
{
    //don't check if has sons. Supposed to be called from a son
    bool is_full=false;
    bool full_status_changed=false;
    for (unsigned int son=0;son<8;son++)
        if (!(m_sons[son]->m_flags&IS_FULL))
        {
            is_full=false;
            break;
        }
    full_status_changed = ((m_flags&IS_FULL)!=is_full);//status has changed
    if (is_full)
      m_flags|=IS_FULL;
    else
      m_flags&=~IS_FULL;
    if ((m_parent)&&(full_status_changed))
        m_parent->update_parent_is_full();
}

bool OctreeCell::update_covered()//if all neighs are full
{
    bool covered=true;
    bool covered_status_changed=false;
    for (unsigned int neigh=0;neigh<6;neigh++)
        if ((!m_neigh[neigh])||(!((m_neigh[neigh]->m_flags)&IS_FULL)))
        {
            covered=false;
            break;
        }

    covered_status_changed = ((m_flags&IS_COVERED)!=covered);//status has changed
    if (covered)
      m_flags|=IS_COVERED;
    else
      m_flags&=~IS_COVERED;
    if ((m_parent)&&(covered_status_changed))
        m_parent->update_covered();

    return covered;
}

//used to give colour to parent
//and to update covered status
long OctreeCell::count_rendering_cells()
{
#ifdef DEBUG_OctreeCell
        dump();
#endif
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
        update_covered();
        if (m_col_a!=0)
        {
            if (!(m_flags&IS_COVERED))//don't put covered son colour to parent
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

bool OctreeCell::del_voxel(unsigned short x,unsigned short y,unsigned short z)
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
        set_color(m_col_r,m_col_g,m_col_b,0);
        m_flags&=~IS_FULL;
        if (m_parent)
            m_parent->update_parent_is_full();
        return true;
    }

    if (!(m_sons[0]))
        std::cerr<<"Impossible to remove non-existing voxel: "<<x<<" "<<y<<" "<<z<<std::endl;
    //find which son
    short i=0;
    if (z>=m_z_min+m_size/2) i+=4;
    if (y>=m_y_min+m_size/2) i+=2;
    if (x>=m_x_min+m_size/2) i+=1;

    m_sons[i]->del_voxel(x,y,z);

    //check if all sons are alike
    check_all_alike();

    return true;
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
        if (a!=0)
            m_flags|=IS_FULL;
        else
            m_flags&=~IS_FULL;
        if (m_parent)
            m_parent->update_parent_is_full();
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
    check_all_alike();

    return true;
}

//check if all sons are alike
bool OctreeCell::check_all_alike()
{
    if (!m_sons)
        return true;

    //check if all sons are alike and have no sons! (not supposed do destroy sons with sons)
    bool all_alike=true;
    if (m_sons[0]->m_col_a==0)
    {//for empty cells, colour is taken into account
        for (unsigned int i=0;i<8;i++)
        {
            if ((m_sons[i]->m_col_a!=0)||(m_sons[i]->m_sons[0]))
            {
                all_alike=false;
                break;
            }
        }
    }else{
        for (unsigned int i=0;i<8;i++)
        {
            if ((m_sons[i]->m_col_r!=m_sons[0]->m_col_r)||(m_sons[i]->m_col_g!=m_sons[0]->m_col_g)||
                (m_sons[i]->m_col_b!=m_sons[0]->m_col_b)||(m_sons[i]->m_col_a!=m_sons[0]->m_col_a)||(m_sons[i]->m_sons[0]))
            {
                all_alike=false;
                break;
            }
        }
    }
    if (all_alike)
    {
        //normally don't have to check if is full...
        if (m_sons[0]->m_col_a!=0) //all alike but not empty
            m_flags|=IS_FULL;
        else
            m_flags&=~IS_FULL;

        m_col_r=m_sons[0]->m_col_r;
        m_col_g=m_sons[0]->m_col_g;
        m_col_b=m_sons[0]->m_col_b;
        m_col_a=m_sons[0]->m_col_a;
        //std::cout<<"Will delete sons for ";
        //dump();
        for (unsigned int i=0;i<8;i++)
        {
            delete m_sons[i];m_sons[i]=0;
        }
        //std::cout<<"Deleted sons for ";
        //dump();

    }
    return all_alike;
}
