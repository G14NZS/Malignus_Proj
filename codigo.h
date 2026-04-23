#ifndef CODIGO
#define CODIGO
#include "meustiposonlinegdb.h"
typedef struct
{
    U8* byte;
    U64 capacidade;
    U64 tamanho;
} Codigo;
boolean novo_codigo (Codigo* c);
void free_codigo (Codigo* c);
boolean adiciona_bit (Codigo* c, U8 valor);
boolean joga_fora_bit (Codigo* c);
char* toString (Codigo c);
boolean clone (Codigo original, Codigo* copia);
#endif
