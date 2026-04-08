#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arvore_binaria.h"

void unsigned int qtos_elementos_comuns (Arvore_binaria_de_busca* a, Arvore_binaria_de_busca* b)
{
    if (a->raiz==NULL || b->raiz==NULL) return 0;

    Ptr_de_no_de_arvore_binaria_de_busca backup = a->raiz;
    a->raiz=backup->esquerda;
    unsigned int qtdNaEsq = qtos_elementos_comuns(a,b);
    a->raiz=backup->direita;
    unsigned int qtdNaDir = qtos_elementos_comuns(a,b);
    a->raiz=backup;

    if (tem_na_arvore_binaria_de_busca(b,a->raiz->informacao))
        return 1+qtdNaEsq+qtdNaDir;
    else
        return qtdNaEsq+qtdNaDir;

}

static Ptr_de_no_de_arvore_binaria_de_busca novo_no_de_arvore_binaria_de_busca (Ptr_de_no_de_arvore_binaria_de_busca e, Elemento i, Ptr_de_no_de_arvore_binaria_de_busca d)
{
    Ptr_de_no_de_arvore_binaria_de_busca novo = (Ptr_de_no_de_arvore_binaria_de_busca)malloc(sizeof(Struct_do_no_de_arvore_binaria_de_busca));

    novo->esquerda=e;
    novo->informacao=i;
    novo->direita=d;

    return novo;
}

void nova_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, int (*c) (Elemento x, Elemento y), void (*m) (Elemento x))
{
    a->raiz=NULL;
    a->comparacao=c;
    a->mostra=m;
}

boolean guarde_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i)
{
    if (a->raiz==NULL)
    {
        a->raiz=novo_no_de_arvore_binaria_de_busca(NULL,i,NULL);
        return true;
    }
    
    Ptr_de_no_de_arvore_binaria_de_busca atual=a->raiz;
        
    for(;;) // forever
    {
        int comp=*(a->comparacao)(i,atual->informacao);

        if (comp==0) return false;
        
        if (comp<0)
        {
            if (atual->esquerda==NULL)
            {
                atual->esquerda=novo_no_de_arvore_binaria_de_busca(NULL,i,NULL);
                return true;
            }
            else
                atual=atual->esquerda;
        }
        else // comp>0
        {
            if (atual->direita==NULL)
            {
                atual->direita=novo_no_de_arvore_binaria_de_busca(NULL,i,NULL);
                return true;
            }
            else
                atual=atual->direita;
        }
    }
}
/*
boolean tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i) // nao precisava passar a arvore por referencia; foi só pão-durismo
{
    if (a->raiz==NULL) return false;

    int comp=*(a->comparacao)(i,a->raiz->informacao);
    if (comp==0) return true;

    Ptr_de_no_de_arvore_binaria_de_busca bkp_raiz=a->raiz;

    if (comp<0)
        a->raiz=a->raiz->esquerda;
    else // comp>0
        a->raiz=a->raiz->direita;

    boolean ret=tem_na_arvore_binaria_de_busca(a,i);
    a->raiz=bkp_raiz;

    return ret;
}
*/
/*
boolean tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca a, Elemento i) // se escolher essa versao, altere o .h para passar a arvore por valor (nao por referencia)
{
    if (a.raiz==NULL) return false;

    int comp=*(a->comparacao)(i,a.raiz->informacao);
    if (comp==0) return true;

    if (comp<0)
        a.raiz=a.raiz->esquerda;
    else // comp>0
        a.raiz=a.raiz->direita;

    return tem_na_arvore_binaria_de_busca(a,i);
}
*/
boolean tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i) // nao precisava passar a arvore por referencia; foi só pão-durismo
{
    Ptr_de_no_de_arvore_binaria_de_busca atual = a->raiz;

    while (atual!=NULL)
    {
        int comp=*(a->comparacao)(i,atual->informacao);

        if (comp==0) return true;

        if (comp<0)
            atual=atual->esquerda;
        else // comp>0
            atual=atual->direita;
    }

    return false;
}

unsigned int quantas_info_tem_na_arvore_binaria_de_busca (Arvore_binaria_de_busca* a) // nao precisava passar a arvore por referencia; foi só pão-durismo
{
    if (a->raiz==NULL) return 0;

    Ptr_de_no_de_arvore_binaria_de_busca bkp_raiz=a->raiz;
    a->raiz=a->raiz->esquerda;
    unsigned int qtsEsq=quantas_info_tem_na_arvore_binaria_de_busca(a);

    a->raiz=bkp_raiz;
    a->raiz=a->raiz->direita;
    unsigned int qtsDir=quantas_info_tem_na_arvore_binaria_de_busca(a);

    a->raiz=bkp_raiz;
    return qtsEsq+1+qtsDir;
}

void print_pre_ordem (Arvore_binaria_de_busca* a) // nao precisava passar a arvore por referencia; foi só pão-durismo
{
    if (a->raiz==NULL) return;

    *(a->mostra)(a->raiz->informacao);
    printf(" ");

    Ptr_de_no_de_arvore_binaria_de_busca bkp_raiz=a->raiz;

    a->raiz=a->raiz->esquerda;
    print_pre_ordem(a);
    a->raiz=bkp_raiz;

    a->raiz=a->raiz->direita;
    print_pre_ordem(a);
    a->raiz=bkp_raiz;
}

void print_in_ordem (Arvore_binaria_de_busca* a) // nao precisava passar a arvore por referencia; foi só pão-durismo
{
    if (a->raiz==NULL) return;

    Ptr_de_no_de_arvore_binaria_de_busca bkp_raiz=a->raiz;

    a->raiz=a->raiz->esquerda;
    print_pre_ordem(a);
    a->raiz=bkp_raiz;

    *(a->mostra)(a->raiz->informacao);
    printf(" ");

    a->raiz=a->raiz->direita;
    print_pre_ordem(a);
    a->raiz=bkp_raiz;
}

void boolean print_pos_ordem (Arvore_binaria_de_busca* a) // nao precisava passar a arvore por referencia; foi só pão-durismo
{
    if (a->raiz==NULL) return;

    Ptr_de_no_de_arvore_binaria_de_busca bkp_raiz=a->raiz;

    a->raiz=a->raiz->esquerda;
    print_pre_ordem(a);
    a->raiz=bkp_raiz;

    a->raiz=a->raiz->direita;
    print_pre_ordem(a);
    a->raiz=bkp_raiz;

    *(a->mostra)(a->raiz->informacao);
    printf(" ");
}

boolean remova_da_arvore_binaria_de_busca (Arvore_binaria_de_busca* a, Elemento i)
{
    Ptr_de_no_de_arvore_binaria_de_busca pai=NULL, atual=a->raiz, *ligacao_do_atual_com_pai;

    for(;;) // forever
    {
        if (atual==NULL) return false;

        int comp=*(a->comparacao)(i,atual->informacao);

        if (comp==0)
        {
            if (atual->esquerda==NULL && atual->direita==NULL)
            {
                free(atual->informacao);
                free(atual);
                if (pai==NULL) // a informacao a ser removida foi encontrada na raiz
                    a->raiz=NULL;
                else
                    *ligacao_do_atual_com_o_pai=NULL;
            }
            else if (atual->esquerda!=NULL && atual->direita==NULL)
            {
                *ligacao_do_atual_com_o_pai=atual->esquerda;
                free(atual->informacao);
                free(atual);
            }
            else if (atual->esquerda==NULL && atual->direita!=NULL)
            {
                *ligacao_do_atual_com_o_pai=atual->direita;
                free(atual->informacao);
                free(atual);
            }
            else
            {
                Ptr_de_no_de_arvore_binaria_de_busca a_excluir=atual;
                free(a_excluir->informacao);
                unsigned long int aleatorio=rand()%2;
                if (aleatorio==0) // vou p a esq, procurar a extrema dir
                {
                    if (atual->esquerda->direita!=NULL)
                    {
                        pai=atual;
                        atual=atual->esquerda;
                        ligacao_do_atual_com_o_pai=&atual->esquerda;
                        while (atual->direita!=NULL)
                        {
                            pai=atual;
                            atual=atual->direita;
                            ligacao_do_atual_com_o_pai=&atual->direita;
                        }
                        a_excluir->informacao=atual->informacao;
                        *ligacao_do_atual_com_o_pai=atual->esquerda;
                        free(atual);
                    }
                } 
                else // (aleatorio==1)  vou p a dir, procurar a extrema esq
                {
                    if (atual->direita->esquerda!=NULL)
                    {
                        pai=atual;
                        atual=atual->direita;
                        ligacao_do_atual_com_o_pai=&atual->esquerda;
                        while (atual->esquerda!=NULL)
                        {
                            pai=atual;
                            atual=atual->esquerda;
                            ligacao_do_atual_com_o_pai=&atual->esquerda;
                        }
                        a_excluir->informacao=atual->informacao;
                        *ligacao_do_atual_com_o_pai=atual->direita;
                        free(atual);
                    }
                }
            }
            return true;
        }

        pai=atual;
        if (comp<0)
        {
            atual=atual->esquerda;
            ligacao_do_atual_com_o_pai=&pai->esquerda;
        }
        else // comp>0
        {
            atual=atual->direita;
            ligacao_do_atual_com_o_pai=&pai->direita;
        }      
    }
} 

#define abs(x) ((x)<0?-(x):(x))

void balanceie_a_arvore_binaria_de_busca (Arvore_binaria_de_busca* a)
{
    Ptr_de_no_de_arvore_binaria_de_busca bkp_raiz=a->raiz;

    a->raiz=bkp_raiz->esquerda;
    unsigned int qtd_esquerda=quantas_info_tem_na_arvore_binaria_de_busca(a);

    a->raiz=bkp_raiz->direita;
    unsigned int qtd_direita=quantas_info_tem_na_arvore_binaria_de_busca(a);

    unsigned int diferenca  = abs(qtd_esquerda-qtd_direita);
    unsigned int transferir = diferenca/2;

    if (qtd_esquerda>qtd_direita)
        for (int i=0; i<transferir; i++)
        {
            pai=bkp_raiz;
            atual=bkp_raiz->esquerda;
            ligacao_do_atual_com_o_pai=&atual->esquerda;
            while (atual->direita!=NULL)
            {
                pai=atual;
                atual=atual->direita;
                ligacao_do_atual_com_o_pai=&atual->direita;
            }
            elemento substituto=atual->informacao;
            *ligacao_do_atual_com_o_pai=atual->esquerda;
            free(atual);
            a->raiz=bkp_raiz;
            elemento raiz_original=a->raiz->informacao;
            a->raiz->informacao=substituto;
            guarde_na_arvore_binaria_de_busca(a,raiz_original->informacao);
       }
    else // qtd_direita>qtd_esquerda
        for (int i=0; i<transferir; i++)
            // faça

    bkp_raiz=a->raiz;

    a->raiz=bkp_raiz->esquerda;
    balanceie_a_arvore_binaria_de_busca(a);
    bkp_raiz->esquerda=a->raiz;

    a->raiz=bkp_raiz->direita;
    balanceie_a_arvore_binaria_de_busca(a);
    bkp_raiz->direita=a->raiz;

    a->raiz=bkp_raiz;    
}

void free_arvore_binaria_de_busca (Arvore_binaria_de_busca* a)
{
    if (a->raiz==NULL) return;
    
    Ptr_de_no_de_arvore_binaria_de_busca esq=a->raiz->esquerda;
    Ptr_de_no_de_arvore_binaria_de_busca dir=a->raiz->direita;
    
    free(a->raiz->informacao);
    free(a->raiz);
    
    a->raiz=esq;
    free_arvore_binaria_de_busca(a);
    
    a->raiz=dir;
    free_arvore_binaria_de_busca(a);
}
