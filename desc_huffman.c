#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Cria um nó que pode ser uma folha (com caractere) ou um nó pai (soma de frequências)
NoHuffman* novo_no_huffman(unsigned char c, int freq, NoHuffman* esq, NoHuffman* dir) {
    NoHuffman* novo = (NoHuffman*)malloc(sizeof(Node));
    novo->informacao = (InfoHuffman*)malloc(sizeof(InfoHuffman));
    
    novo->informacao->caractere = c;
    novo->informacao->frequencia = freq;
    novo->esquerda = esq;
    novo->direita = dir;
    
    return novo;
}

// Limpa a árvore da memória (recursivo como o seu)
void free_arvore_huffman(NoHuffman* raiz) {
    if (raiz == NULL) return;
    free_arvore_huffman(raiz->esquerda);
    free_arvore_huffman(raiz->direita);
    free(raiz->informacao);
    free(raiz);
}
