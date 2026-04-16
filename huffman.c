#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

NoHuffman* cria_no_folha(char c, int freq) {
    NoHuffman* novo = (NoHuffman*)malloc(sizeof(NoHuffman));
    if (novo == NULL) return NULL;
    novo->caractere = c;
    novo->frequencia = freq;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

NoHuffman* cria_no_interno(NoHuffman* esq, NoHuffman* dir) {
    NoHuffman* novo = (NoHuffman*)malloc(sizeof(NoHuffman));
    if (novo == NULL) return NULL;
    novo->caractere = '\0';
    novo->frequencia = esq->frequencia + dir->frequencia;
    novo->esquerda = esq;
    novo->direita = dir;
    return novo;
}

void insere_ordenado(Fila* fil, NoHuffman* novo) {
    int tam = 0;
    NoHuffman* temp[256];

    // Esvazia a fila inteira para o array
    while (!fila_vazia(*fil)) {
        NoHuffman* atual;
        recupere_da_fila(*fil, (ElementoDeFila*)&atual);
        remova_elemento_da_fila(fil);
        temp[tam++] = atual;
    }

    // Reinsere tudo, colocando o novo na posição certa
    int inserido = 0;
    for (int i = 0; i < tam; i++) {
        if (!inserido && novo->frequencia <= temp[i]->frequencia) {
            guarde_na_fila(fil, (ElementoDeFila)novo);
            inserido = 1;
        }
        guarde_na_fila(fil, (ElementoDeFila)temp[i]);
    }
    // Se o novo é o maior de todos, vai no final
    if (!inserido) {
        guarde_na_fila(fil, (ElementoDeFila)novo);
    }
}

NoHuffman* monta_arvore(Fila* fil) {
    while (1) {
        // Tira o menor
        NoHuffman* esq;
        recupere_da_fila(*fil, (ElementoDeFila*)&esq);
        remova_elemento_da_fila(fil);

        // Se a fila esvaziou, esse é a raiz
        if (fila_vazia(*fil)) {
            return esq;
        }

        // Tira o segundo menor
        NoHuffman* dir;
        recupere_da_fila(*fil, (ElementoDeFila*)&dir);
        remova_elemento_da_fila(fil);

        // Junta os dois num nó pai
        NoHuffman* pai = cria_no_interno(esq, dir);

        // Reinsere na posição correta
        insere_ordenado(fil, pai);
    }
}

void imprime_arvore(NoHuffman* raiz, int nivel) {
    if (raiz == NULL) return;
    imprime_arvore(raiz->direita, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("    ");
    if (raiz->caractere != '\0')
        printf("[%c:%d]\n", raiz->caractere, raiz->frequencia);
    else
        printf("(%d)\n", raiz->frequencia);
    imprime_arvore(raiz->esquerda, nivel + 1);
}

void free_arvore(NoHuffman* raiz) {
    if (raiz == NULL) return;
    free_arvore(raiz->esquerda);
    free_arvore(raiz->direita);
    free(raiz);
}

void gera_codigos(NoHuffman* raiz, Codigo atual, Codigo tabela[256]) {
    if (raiz == NULL) return;
 
    // É folha? Salva o código na tabela
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        unsigned char idx = (unsigned char)raiz->caractere;
        clone(atual, &tabela[idx]);
        return;
    }
 
    // Vai para esquerda: adiciona bit 0
    adiciona_bit(&atual, 0);
    gera_codigos(raiz->esquerda, atual, tabela);
    joga_fora_bit(&atual);
 
    // Vai para direita: adiciona bit 1
    adiciona_bit(&atual, 1);
    gera_codigos(raiz->direita, atual, tabela);
    joga_fora_bit(&atual);
}

boolean compacta_texto(char* texto, Codigo tabela[256], Codigo* saida) {
    novo_codigo(saida);
    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char c = (unsigned char)texto[i];
        if (tabela[c].byte == NULL) continue;
        
        // Converte o código em string de bits e adiciona cada um
        char* bits = toString(tabela[c]);
        for (int j = 0; bits[j] != '\0'; j++) {
            adiciona_bit(saida, bits[j] == '1' ? 1 : 0);
        }
        free(bits);
    }
    return true;
}

char* descompacta_texto(Codigo compactado, NoHuffman* raiz) {
    // Aloca espaço para o texto descompactado
    // No pior caso, cada bit é um caractere
    char* resultado = (char*)malloc((compactado.tamanho + 1) * sizeof(char));
    if (resultado == NULL) return NULL;
 
    int pos = 0;
    NoHuffman* atual = raiz;
 
    // Converte os bits para string para facilitar a leitura
    char* bits = toString(compactado);
    if (bits == NULL) {
        free(resultado);
        return NULL;
    }
 
    for (U64 i = 0; i < compactado.tamanho; i++) {
        // 0 = esquerda, 1 = direita
        if (bits[i] == '0')
            atual = atual->esquerda;
        else
            atual = atual->direita;
 
        // Chegou numa folha = encontrou um caractere
        if (atual->esquerda == NULL && atual->direita == NULL) {
            resultado[pos++] = atual->caractere;
            atual = raiz;  // volta para a raiz
        }
    }
 
    resultado[pos] = '\0';
    free(bits);
    return resultado;
}