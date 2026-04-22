#include "huffman.h"
#include <stdio.h>
#include <string.h>

No* cria_no(unsigned char c, int f) {
    No* n = malloc(sizeof(No));
    n->c = c;
    n->freq = f;
    n->esq = n->dir = NULL;
    return n;
}

int compara(const void* a, const void* b) {
    return (*(No**)a)->freq - (*(No**)b)->freq;
}

No* monta_arvore(No** lista, int n) {
    while (n > 1) {
        qsort(lista, n, sizeof(No*), compara);

        No* esq = lista[0];
        No* dir = lista[1];

        No* pai = cria_no(0, esq->freq + dir->freq);
        pai->esq = esq;
        pai->dir = dir;

        lista[0] = pai;
        lista[1] = lista[n-1];
        n--;
    }
    return lista[0];
}

void gera_codigos(No* raiz, Codigo atual, Codigo tabela[256]) {
    if (!raiz) return;

    if (!raiz->esq && !raiz->dir) {
        if (atual.tamanho == 0)
            adiciona_bit(&atual, 0);

        clone(atual, &tabela[raiz->c]);
        return;
    }

    adiciona_bit(&atual, 0);
    gera_codigos(raiz->esq, atual, tabela);
    
    atual.tamanho--;
    
    adiciona_bit(&atual, 1);
    gera_codigos(raiz->dir, atual, tabela);
    
    atual.tamanho--;
}

boolean compacta_texto(char* texto, Codigo tabela[256], Codigo* saida) {
    novo_codigo(saida);

    for (int i = 0; texto[i]; i++) {
        Codigo cod = tabela[(unsigned char)texto[i]];

        for (U64 j = 0; j < cod.tamanho; j++) {
            U64 b = j / 8;
            U8 bit = 7 - (j % 8);
            U8 val = (cod.byte[b] >> bit) & 1;
            adiciona_bit(saida, val);
        }
    }
    return true;
}

char* descompacta(Codigo c, No* raiz) {
    char* out = malloc(1000);
    int pos = 0;

    No* atual = raiz;

    for (U64 i = 0; i < c.tamanho; i++) {
        U64 b = i / 8;
        U8 bit = 7 - (i % 8);
        U8 val = (c.byte[b] >> bit) & 1;

        atual = val ? atual->dir : atual->esq;

        if (!atual->esq && !atual->dir) {
            out[pos++] = atual->c;
            atual = raiz;
        }
    }

    out[pos] = '\0';
    return out;
}

void free_arvore(No* r) {
    if (!r) return;
    free_arvore(r->esq);
    free_arvore(r->dir);
    free(r);
}