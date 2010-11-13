#include "compress_tools.h"

#include <stdlib.h>
#include <iostream>

//#define DEBUG_RLE

//compress data into data_RLE, return len of data_RLE
unsigned short Compress_Tools::zip_RLE(Vox * data,unsigned short data_len,Vox_RLE ** data_RLE)
{
#ifdef DEBUG_RLE
    std::cout<<"Compress ";
    for (unsigned short z=0;z<data_len;z++)
        std::cout<<(int)data[z].c<<" ";
    std::cout<<"\ninto: ";
#endif

    unsigned short nbr_RLE=1;
    //first: count how many RLE parts
    for (unsigned short z=1;z<data_len;z++)
        if ((data[z-1].c!=data[z].c)||(data[z-1].f!=data[z].f))
            nbr_RLE++;

    //create array
    (*data_RLE) = (Vox_RLE *)malloc(nbr_RLE*sizeof(Vox_RLE));
    (*data_RLE)[0].vox=data[0];
    (*data_RLE)[0].nbr=1;
    unsigned short n_RLE=0;//RLE counter
    for (unsigned short z=1;z<data_len;z++)
    {
        if (((*data_RLE)[n_RLE].vox.c==data[z].c)&&((*data_RLE)[n_RLE].vox.f==data[z].f))
            (*data_RLE)[n_RLE].nbr++;
        else
        {
            n_RLE++;
            (*data_RLE)[n_RLE].vox=data[z];
            (*data_RLE)[n_RLE].nbr=1;
        }
    }

#ifdef DEBUG_RLE
    for (unsigned short n=0;n<nbr_RLE;n++)
        std::cout<<(*data_RLE)[n].nbr<<"*"<<(int)(*data_RLE)[n].vox.c<<" ";
    std::cout<<"\n"<<std::endl;
#endif

    return nbr_RLE;
}

//uncompress data_RLE into data, return len of data
unsigned short Compress_Tools::unzip_RLE(Vox_RLE * data_RLE,unsigned short data_RLE_len,Vox ** data)
{
}
