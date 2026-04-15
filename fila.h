#ifndef FILA
#define FILA

#include "basics.h"

typedef void* ElementoDeFila;

typedef struct {
    ElementoDeFila* vetor; // Elemento vetor [];
    unsigned int capacidade_inicial, capacidade, qtd_atual,
                 inicio, final;
} Fila;

boolean nova_fila (Fila* p, unsigned int capacidade);
boolean guarde_na_fila (Fila* p, ElementoDeFila e);
boolean recupere_da_fila (Fila p, ElementoDeFila* e);
boolean remova_elemento_da_fila (Fila* p);
boolean fila_cheia (Fila p);
boolean fila_vazia (Fila p);
boolean free_fila (Fila* p);

#endif // FILA
