#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include "basics.h"

// Esta é a estrutura que irá dentro de cada nó
typedef struct {
    unsigned char caractere;
    int frequencia;
} InfoHuffman;

typedef struct Node {
    struct Node* esquerda;
    InfoHuffman* informacao;
    struct Node* direita;
} Node;

typedef Node* Ptr_Node;

// Funções essenciais para Huffman
Ptr_Node novo_no_huffman(unsigned char c, int freq, Ptr_Node esq, Ptr_Node dir);
void free_arvore_huffman(Ptr_Node raiz);

#endif
