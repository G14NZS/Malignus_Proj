#ifndef HUFFMAN
#define HUFFMAN
#include "meustiposonlinegdb.h"
#include "fila.h"
#include "codigo.h"

typedef struct no_huffman {
    char caractere;
    int frequencia;
    struct no_huffman *esquerda;
    struct no_huffman *direita;
} NoHuffman;

/* --- Construção e manipulação da árvore --- */
NoHuffman* cria_no_folha(char c, int freq);
NoHuffman* cria_no_interno(NoHuffman* esq, NoHuffman* dir);
NoHuffman* monta_arvore(Fila* fil);
void imprime_arvore(NoHuffman* raiz, int nivel); /* util para debug */
void free_arvore(NoHuffman* raiz);
void gera_codigos(NoHuffman* raiz, Codigo* atual, Codigo tabela[256]);

/* --- Compactação / descompactação de bytes arbitrários --- */

/* Compacta um buffer de n_bytes (pode conter bytes 0x00).
 * saida é preenchido com os bits do resultado.
 * Retorna false se algum byte não tiver código na tabela. */
boolean compacta_dados(U8* dados, U64 n_bytes,
                       Codigo tabela[256], Codigo* saida);

/* Descompacta usando a árvore. Retorna um buffer alocado com n_bytes_original
 * bytes (NÃO é null-terminated — use o tamanho). Retorna NULL em erro. */
U8* descompacta_dados(Codigo compactado, NoHuffman* raiz,
                      U64 n_bytes_original);

/* Constrói a árvore a partir da tabela de frequências.
 * Retorna a raiz ou NULL se não houver nenhum byte (arquivo vazio).
 * Ordenação determinística (mesma na compactação e descompactação). */
NoHuffman* monta_arvore_de_frequencias(U32 frequencia[256]);

/* Gera a tabela de códigos a partir da raiz, tratando o caso de árvore
 * com um único nó (código "0" para o único byte). */
boolean gera_tabela(NoHuffman* raiz, Codigo tabela[256]);

/* Compacta o arquivo em nome_entrada e grava um .ruff em nome_saida.
 * Retorna true em sucesso. */
boolean compacta_arquivo(const char* nome_entrada, const char* nome_saida);

/* Lê o .ruff de nome_entrada e grava o arquivo original em nome_saida. */
boolean descompacta_arquivo(const char* nome_entrada, const char* nome_saida);

#endif
