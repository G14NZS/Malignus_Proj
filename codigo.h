#ifndef CODIGO
#define CODIGO

#include <stdlib.h>

typedef unsigned char U8;
typedef unsigned long long U64;
typedef int boolean;

#define true 1
#define false 0

typedef struct {
    U8* byte;
    U64 capacidade; // bits
    U64 tamanho;    // bits
} Codigo;

boolean novo_codigo(Codigo* c);
void free_codigo(Codigo* c);
boolean adiciona_bit(Codigo* c, U8 valor);
char* toString(Codigo c);
boolean clone(Codigo original, Codigo* copia);

#endif