#ifndef KV6FORMAT_H
#define KV6FORMAT_H

//form Voxlap engine by Ken Silverman (http://advsys.net/ken)

//Sprite structures:
typedef struct { long col; unsigned short z; char vis, dir; } kv6voxtype;

typedef struct kv6data
{
    long leng, xsiz, ysiz, zsiz;
    float xpiv, ypiv, zpiv;
    unsigned long numvoxs;
    long namoff;
    kv6data *lowermip;
    kv6voxtype *vox;      //numvoxs*sizeof(kv6voxtype)
    unsigned long *xlen;  //xsiz*sizeof(long)
    unsigned short *ylen; //xsiz*ysiz*sizeof(short)
} kv6data;

void savekv6 (const char *filnam, kv6data *kv);
//static kv6data *loadkv6 (const char *filnam);

#endif // KV6FORMAT_H
