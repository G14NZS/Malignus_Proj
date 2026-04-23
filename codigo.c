#include <stdlib.h>
#include "codigo.h"
boolean novo_codigo (Codigo* c)
{
    c->byte=(U8*)malloc(1*sizeof(U8));
    if (c->byte==NULL) return false;
    c->byte[0]=0; /* zera para evitar lixo nos bits altos se o byte ficar parcial */
    c->capacidade=8;
    c->tamanho=0;
    return true;
}
void free_codigo (Codigo* c)
{
    if (c->byte!=NULL) free(c->byte);
    c->byte=NULL;
    c->capacidade=0;
    c->tamanho=0;
}
boolean adiciona_bit (Codigo* c, U8 valor)
{
    if (c->tamanho==c->capacidade)
    {
        U8* novo=(U8*)malloc((c->capacidade/8+1)*sizeof(U8));
        if (novo==NULL) return false;
        for(U64 i=0;i<c->capacidade/8;i++)
            novo[i]=c->byte[i];
        novo[c->capacidade/8]=0; /* zera o novo byte */
        free(c->byte);
        c->byte=novo;
        c->capacidade+=8;
    }
    c->byte[c->capacidade/8-1]<<=1;
    if (valor==1)
        c->byte[c->capacidade/8-1]|=1;
    c->tamanho++;
    return true;
}
boolean joga_fora_bit (Codigo* c)
{
    if (c->tamanho==0) return false;
    c->byte[c->capacidade/8-1]>>=1;
    c->tamanho--;
    if (c->capacidade>8 && c->capacidade-c->tamanho==8)
    {
        U8* novo=(U8*)malloc((c->capacidade/8-1)*sizeof(U8));
        if (novo==NULL) return false;
        for(U64 i=0;i<c->tamanho/8;i++)
            novo[i]=c->byte[i];
        free(c->byte);
        c->byte=novo;
        c->capacidade-=8;
    }
    return true;
}
boolean clone (Codigo original, Codigo* copia)
{
    copia->byte=(U8*)malloc(original.capacidade/8*sizeof(U8));
    if (copia->byte==NULL) return false;
    for(U64 i=0;i<original.capacidade/8;i++)
        copia->byte[i]=original.byte[i];
    copia->capacidade=original.capacidade;
    copia->tamanho=original.tamanho;
    return true;
}
char* toString (Codigo c)
{
    char* retorno = (char*)malloc((c.tamanho+3)*sizeof(char));
    if (retorno == NULL) return NULL;
    int bits_ultimo_byte = c.tamanho % 8;
    if (bits_ultimo_byte == 0) bits_ultimo_byte = 8;
    int pos = 0;
    for (U64 b = 0; b < c.capacidade/8; b++)
    {
        int bits_neste_byte = (b < c.capacidade/8-1) ? 8 : bits_ultimo_byte;
        for (int i = bits_neste_byte-1; i >= 0; i--)
            retorno[pos++] = ((c.byte[b] >> i) & 1) ? '1' : '0';
    }
    retorno[c.tamanho] = '\0';
    return retorno;
}
