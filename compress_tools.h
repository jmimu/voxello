#ifndef COMPRESS_TOOLS_H
#define COMPRESS_TOOLS_H

struct Vox
{
    unsigned char c;//colour
    unsigned char f;//flags

    //constructor doesn't seem to work...
    //Vox():f(0){}
};

struct Vox_RLE
{
    unsigned short nbr;
    Vox vox;
};

struct Column_RLE
{
    unsigned short nbr_data;
    Vox_RLE * data;
};




namespace Compress_Tools
{

    //compress data into data_RLE, return len of data_RLE
    unsigned short zip_RLE(Vox * data,unsigned short data_len,Vox_RLE ** data_RLE);

    //uncompress data_RLE into data, return len of data
    unsigned short unzip_RLE(Vox_RLE * data_RLE,unsigned short data_RLE_len,Vox ** data);
};

#endif // COMPRESS_TOOLS_H
