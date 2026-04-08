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

// --- Protótipos das Funções ---

// Retorna a quantidade de elementos presentes em ambas as árvores
unsigned int qtos_elementos_comuns (Arvore_binaria_de_busca* a, Arvore_binaria_de_busca* b);

// Inicializa a estrutura da árvore
void nova_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, int (*c) (Elemento x, Elemento y), void (*m) (Elemento x));

// Insere um elemento na árvore. Retorna false se o elemento já existir
boolean guarde_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i);

// Verifica se um elemento existe na árvore
boolean tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i);

// Retorna o total de nós da árvore
unsigned int quantas_info_tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a);

// Funções de percurso e exibição (Retorno alterado para void)
void print_pre_ordem (Arvore_binaria_de_busca* a);
void print_in_ordem (Arvore_binaria_de_busca* a);
void print_pos_ordem (Arvore_binaria_de_busca* a);

// Remove um elemento específico
boolean remova_da_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i);

// Reorganiza a árvore para que fique balanceada
void balanceie_a_arvore_binaria_de_busca (Arvore_binaria_de_busca* a);

// Libera toda a memória alocada pela árvore
void free_arvore_binaria_de_busca (Arvore_binaria_de_busca* a);

#endif // ARVORE_BINARIA_DE_BUSCA
