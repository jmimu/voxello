#include "kv6format.h"

#include <stdlib.h>
#include <stdio.h>

void savekv6 (const char *filnam, kv6data *kv)
{
        FILE *fil;
        long i;

        if (fil = fopen(filnam,"wb"))
        {
                i = 0x6c78764b; fwrite(&i,4,1,fil); //Kvxl
                fwrite(&kv->xsiz,4,1,fil); fwrite(&kv->ysiz,4,1,fil); fwrite(&kv->zsiz,4,1,fil);
                fwrite(&kv->xpiv,4,1,fil); fwrite(&kv->ypiv,4,1,fil); fwrite(&kv->zpiv,4,1,fil);
                fwrite(&kv->numvoxs,4,1,fil);
                fwrite(kv->vox,kv->numvoxs*sizeof(kv6voxtype),1,fil);
                fwrite(kv->xlen,kv->xsiz*sizeof(long),1,fil);
                fwrite(kv->ylen,kv->xsiz*kv->ysiz*sizeof(short),1,fil);
                fclose(fil);
        }
}


/*static kv6data *loadkv6 (const char *filnam)
{
        FILE *fil;
        kv6data tk, *newkv6;
        long i;

        if (!kzopen(filnam))
        {
                        //File not found, but allocate a structure anyway
                        //   so it can keep track of the filename
                if (!(newkv6 = (kv6data *)malloc(sizeof(kv6data)))) return(0);
                newkv6->leng = sizeof(kv6data);
                newkv6->xsiz = newkv6->ysiz = newkv6->zsiz = 0;
                newkv6->xpiv = newkv6->ypiv = newkv6->zpiv = 0;
                newkv6->numvoxs = 0;
                newkv6->namoff = 0;
                newkv6->lowermip = 0;
                newkv6->vox = (kv6voxtype *)(((long)newkv6)+sizeof(kv6data));
                newkv6->xlen = (unsigned long *)newkv6->vox;
                newkv6->ylen = (unsigned short *)newkv6->xlen;
                return(newkv6);
        }

        kzread((void *)&tk,32);

        i = tk.numvoxs*sizeof(kv6voxtype) + tk.xsiz*4 + tk.xsiz*tk.ysiz*2;
        newkv6 = (kv6data *)malloc(i+sizeof(kv6data));
        if (!newkv6) { kzclose(); return(0); }
        if (((long)newkv6)&3) evilquit("getkv6 malloc not 32-bit aligned!");

        newkv6->leng = i+sizeof(kv6data);
        memcpy(&newkv6->xsiz,&tk.xsiz,28);
        newkv6->namoff = 0;
        newkv6->lowermip = 0;
        newkv6->vox = (kv6voxtype *)(((long)newkv6)+sizeof(kv6data));
        newkv6->xlen = (unsigned long *)(((long)newkv6->vox)+tk.numvoxs*sizeof(kv6voxtype));
        newkv6->ylen = (unsigned short *)(((long)newkv6->xlen) + tk.xsiz*4);

        kzread((void *)newkv6->vox,i);
        kzclose();
        return(newkv6);
}*/
