#ifndef HUFFMAN
#define HUFFMAN

#include "codigo.h"

typedef struct no {
    unsigned char c;
    int freq;
    struct no *esq, *dir;
} No;

No* cria_no(unsigned char c, int f);
No* monta_arvore(No** lista, int n);
void gera_codigos(No* raiz, Codigo atual, Codigo tabela[256]);
boolean compacta_texto(char* texto, Codigo tabela[256], Codigo* saida);
char* descompacta(Codigo c, No* raiz);
void free_arvore(No* r);

#endif