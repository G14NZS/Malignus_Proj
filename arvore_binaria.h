#ifndef ARVORE_BINARIA_DE_BUSCA
#define ARVORE_BINARIA_DE_BUSCA

#include "basics.h"

typedef void* Elemento;

typedef struct Struct_do_no_de_arvore_binaria_de_busca {
    struct Struct_do_no_de_arvore_binaria_de_busca* esquerda;
    Elemento informacao;
    struct Struct_do_no_de_arvore_binaria_de_busca* direita;
} Struct_do_no_de_arvore_binaria_de_busca;

typedef Struct_do_no_de_arvore_binaria_de_busca* Ptr_de_no_de_arvore_binaria_de_busca;

typedef struct
{
    Ptr_de_no_de_arvore_binaria_de_busca raiz;
    int (*comparacao) (Elemento x, Elemento y);
    void (*mostra) (Elemento x);
} Arvore_binaria_de_busca;

void unsigned int qtos_elementos_comuns (Arvore_binaria_de_busca* a, Arvore_binaria_de_busca* b);
void nova_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, int (*c) (Elemento x, Elemento y), void (*m) (Elemento x));
boolean guarde_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i);
boolean tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i); // nao precisava passar a arvore por referencia; foi só pão-durismo
unsigned int quantas_info_tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a); // nao precisava passar a arvore por referencia; foi só pão-durismo
void boolean print_pre_ordem (Arvore_binaria_de_busca* a); // nao precisava passar a arvore por referencia; foi só pão-durismo
void boolean print_in_ordem (Arvore_binaria_de_busca* a); // nao precisava passar a arvore por referencia; foi só pão-durismo
void boolean print_pos_ordem (Arvore_binaria_de_busca* a); // nao precisava passar a arvore por referencia; foi só pão-durismo
boolean remova_da_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i);
void balanceie_a_arvore_binaria_de_busca (Arvore_binaria_de_busca* a);
void free_arvore_binaria_de_busca (Arvore_binaria_de_busca* a);

#endif // ARVORE_BINARIA_DE_BUSCA