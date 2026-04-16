#ifndef HUFFMAN
#define HUFFMAN

#include "meustiposonlinegdb.h"
#include "fila.h"
#include "codigo.h"

typedef struct no_huffman {
    char caractere;
    int frequencia;
    struct no_huffman *esquerda;
    struct no_huffman *direita;
} NoHuffman;

NoHuffman* cria_no_folha(char c, int freq);
NoHuffman* cria_no_interno(NoHuffman* esq, NoHuffman* dir);
NoHuffman* monta_arvore(Fila* fil);
void imprime_arvore(NoHuffman* raiz, int nivel);
void free_arvore(NoHuffman* raiz);
void gera_codigos(NoHuffman* raiz, Codigo atual, Codigo tabela[256]);
boolean compacta_texto(char* texto, Codigo tabela[256], Codigo* saida);
char* descompacta_texto(Codigo compactado, NoHuffman* raiz);

#endif