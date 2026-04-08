#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arvore_binaria_de_busca.h"

// --- FUNÇÕES AUXILIARES PRIVADAS ---

static Ptr_de_no_de_arvore_binaria_de_busca novo_no(Elemento i) {
    Ptr_de_no_de_arvore_binaria_de_busca novo = (Ptr_de_no_de_arvore_binaria_de_busca)malloc(sizeof(Struct_do_no_de_arvore_binaria_de_busca));
    if (novo) {
        novo->esquerda = NULL;
        novo->informacao = i;
        novo->direita = NULL;
    }
    return novo;
}

static unsigned int contar_recursivo(Ptr_de_no_de_arvore_binaria_de_busca no) {
    if (no == NULL) return 0;
    // CORRIGIDO: de 'right' para 'direita'
    return 1 + contar_recursivo(no->esquerda) + contar_recursivo(no->direita);
}

static void liberar_nos_recursivo(Ptr_de_no_de_arvore_binaria_de_busca no) {
    if (no == NULL) return;
    liberar_nos_recursivo(no->esquerda);
    liberar_nos_recursivo(no->direita);
    // IMPORTANTE: Só libere 'informacao' se ela foi alocada dinamicamente
    if (no->informacao) free(no->informacao); 
    free(no);
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO .H ---

unsigned int qtos_elementos_comuns(Arvore_binaria_de_busca* a, Arvore_binaria_de_busca* b) {
    if (a->raiz == NULL || b->raiz == NULL) return 0;
    
    unsigned int comum = tem_na_arvore_binaria_de_busca(b, a->raiz->informacao) ? 1 : 0;
    
    // Criamos cópias da estrutura para navegar sem destruir a árvore original 'a'
    Arvore_binaria_de_busca esq = *a; esq.raiz = a->raiz->esquerda;
    Arvore_binaria_de_busca dir = *a; dir.raiz = a->raiz->direita;
    
    return comum + qtos_elementos_comuns(&esq, b) + qtos_elementos_comuns(&dir, b);
}

void nova_arvore_binaria_de_busca(Arvore_binaria_de_busca* a, int (*c)(Elemento, Elemento), void (*m)(Elemento)) {
    a->raiz = NULL;
    a->comparacao = c;
    a->mostra = m;
}

boolean guarde_na_arvore_binaria_de_busca(Arvore_binaria_de_busca* a, Elemento i) {
    if (a->raiz == NULL) {
        a->raiz = novo_no(i);
        return true;
    }
    
    Ptr_de_no_de_arvore_binaria_de_busca atual = a->raiz;
    for(;;) {
        int comp = a->comparacao(i, atual->informacao);
        if (comp == 0) return false;
        
        if (comp < 0) {
            if (atual->esquerda == NULL) {
                atual->esquerda = novo_no(i);
                return true;
            }
            atual = atual->esquerda;
        } else {
            if (atual->direita == NULL) {
                atual->direita = novo_no(i);
                return true;
            }
            atual = atual->direita;
        }
    }
}

boolean tem_na_arvore_binaria_de_busca(Arvore_binaria_de_busca* a, Elemento i) {
    Ptr_de_no_de_arvore_binaria_de_busca atual = a->raiz;
    while (atual != NULL) {
        int comp = a->comparacao(i, atual->informacao);
        if (comp == 0) return true;
        atual = (comp < 0) ? atual->esquerda : atual->direita;
    }
    return false;
}

unsigned int quantas_info_tem_na_arvore_binaria_de_busca(Arvore_binaria_de_busca* a) {
    return contar_recursivo(a->raiz);
}

void print_pre_ordem(Arvore_binaria_de_busca* a) {
    if (a->raiz == NULL) return;
    a->mostra(a->raiz->informacao);
    printf(" ");
    
    Arvore_binaria_de_busca sub = *a;
    sub.raiz = a->raiz->esquerda; print_pre_ordem(&sub);
    sub.raiz = a->raiz->direita;  print_pre_ordem(&sub);
}

void print_in_ordem(Arvore_binaria_de_busca* a) {
    if (a->raiz == NULL) return;
    Arvore_binaria_de_busca sub = *a;
    
    sub.raiz = a->raiz->esquerda; print_in_ordem(&sub);
    a->mostra(a->raiz->informacao);
    printf(" ");
    sub.raiz = a->raiz->direita;  print_in_ordem(&sub);
}

void print_pos_ordem(Arvore_binaria_de_busca* a) {
    if (a->raiz == NULL) return;
    Arvore_binaria_de_busca sub = *a;
    
    sub.raiz = a->raiz->esquerda; print_pos_ordem(&sub);
    sub.raiz = a->raiz->direita;  print_pos_ordem(&sub);
    a->mostra(a->raiz->informacao);
    printf(" ");
}

boolean remova_da_arvore_binaria_de_busca(Arvore_binaria_de_busca* a, Elemento i) {
    Ptr_de_no_de_arvore_binaria_de_busca atual = a->raiz;
    Ptr_de_no_de_arvore_binaria_de_busca* ligacao = &(a->raiz);

    while (atual != NULL) {
        int comp = a->comparacao(i, atual->informacao);
        if (comp == 0) break;
        
        if (comp < 0) {
            ligacao = &(atual->esquerda);
            atual = atual->esquerda;
        } else {
            ligacao = &(atual->direita);
            atual = atual->direita;
        }
    }

    if (atual == NULL) return false;

    Ptr_de_no_de_arvore_binaria_de_busca a_remover = atual;

    if (atual->esquerda == NULL) {
        *ligacao = atual->direita;
    } else if (atual->direita == NULL) {
        *ligacao = atual->esquerda;
    } else {
        // Caso com dois filhos: encontrar o sucessor (menor da subárvore direita)
        Ptr_de_no_de_arvore_binaria_de_busca* sucessor_ligacao = &(atual->direita);
        Ptr_de_no_de_arvore_binaria_de_busca sucessor = atual->direita;
        
        while (sucessor->esquerda != NULL) {
            sucessor_ligacao = &(sucessor->esquerda);
            sucessor = sucessor->esquerda;
        }
        
        atual->informacao = sucessor->informacao;
        a_remover = sucessor;
        *sucessor_ligacao = sucessor->direita;
    }

    // Libera o nó e a informação (se necessário)
    if (a_remover->informacao) free(a_remover->informacao);
    free(a_remover);
    return true;
}

void free_arvore_binaria_de_busca(Arvore_binaria_de_busca* a) {
    liberar_nos_recursivo(a->raiz);
    a->raiz = NULL;
}

// Balanceamento (Stubs ou lógica simplificada podem ser adicionados aqui)
void balanceie_a_arvore_binaria_de_busca(Arvore_binaria_de_busca* a) {
    // Para um balanceamento real, você precisaria de rotações ou 
    // reconstruir a árvore a partir de um vetor ordenado.
}
