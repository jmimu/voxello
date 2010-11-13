#ifndef COMPRESS_TOOLS_H
#define COMPRESS_TOOLS_H

//if RLE_8bits, Vox_RLE size is up to 255. else up to 65535
//#define RLE_8bits

//for flags
#define FACE_D_COVERED 0x01
#define FACE_U_COVERED 0x02
#define FACE_S_COVERED 0x04
#define FACE_N_COVERED 0x08
#define FACE_W_COVERED 0x10
#define FACE_E_COVERED 0x20
#define ALL_FACES_COVERED 0x3F
#define IS_COVERED 0x40


struct Vox
{
    unsigned char c;//colour
    unsigned char f;//flags

    //constructor doesn't seem to work...
    //Vox():f(0){}
};

//version 256+
struct Vox_RLE
{
#ifdef RLE_8bits
    unsigned char nbr;
#else
    unsigned short nbr;
#endif
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
