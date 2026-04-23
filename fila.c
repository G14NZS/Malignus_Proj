#include <stdlib.h>
#include "fila.h"
boolean nova_fila (Fila* f, unsigned int capacidade)
{
    if (capacidade <= 0) return false;
    f->vetor = (ElementoDeFila*)malloc(capacidade * sizeof(ElementoDeFila));
    if (f->vetor == NULL) return false;
    f->capacidade_inicial = capacidade;
    f->capacidade = capacidade;
    f->qtd_atual = 0;
    f->inicio = 0;
    f->final = 0;
    return true;
}
boolean guarde_na_fila (Fila* f, ElementoDeFila e)
{
    if (f->qtd_atual == f->capacidade)
    {
        unsigned int nova_cap = f->capacidade * 2;
        ElementoDeFila* novo_vetor = (ElementoDeFila*)realloc(f->vetor, nova_cap * sizeof(ElementoDeFila));

        if (novo_vetor == NULL) return false;
        f->vetor = novo_vetor;
        if (f->final <= f->inicio) {
            for (unsigned int i = 0; i < f->final; i++) {
                f->vetor[i + f->capacidade] = f->vetor[i];
            }
            f->final += f->capacidade;
        }
        f->capacidade = nova_cap;
    }
    f->vetor[f->final] = e;
    f->qtd_atual++;
    f->final = (f->final + 1 == f->capacidade) ? 0 : f->final + 1;
    return true;
}
boolean remova_elemento_da_fila (Fila* f)
{
    if (f->qtd_atual == 0) return false;
    f->vetor[f->inicio] = NULL;
    f->inicio = (f->inicio + 1 == f->capacidade) ? 0 : f->inicio + 1;
    f->qtd_atual--;
    if (f->qtd_atual <= f->capacidade / 4 && f->capacidade > f->capacidade_inicial)
    {
        unsigned int metade = f->capacidade / 2;
        ElementoDeFila* novo = (ElementoDeFila*)malloc(metade * sizeof(ElementoDeFila));
        if (novo == NULL) return false;
        unsigned int pVelho = f->inicio;
        for (unsigned int pNovo = 0; pNovo < f->qtd_atual; pNovo++)
        {
            novo[pNovo] = f->vetor[pVelho];
            pVelho = (pVelho + 1) % f->capacidade;
        }

        free(f->vetor);
        f->vetor = novo;
        f->inicio = 0;
        f->final = f->qtd_atual;
        f->capacidade = metade;
    }
    return true;
}
boolean free_fila (Fila* f)
{
    if (f->vetor == NULL) return false;
    free(f->vetor);
    f->vetor = NULL;
    f->qtd_atual = 0;
    return true;
}
boolean recupere_da_fila (Fila f, ElementoDeFila* e) {
    if (f.qtd_atual == 0) return false;
    *e = f.vetor[f.inicio];
    return true;
}
boolean fila_cheia (Fila f) { return f.qtd_atual == f.capacidade; }
boolean fila_vazia (Fila f) { return f.qtd_atual == 0; }
