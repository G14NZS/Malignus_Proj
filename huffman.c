#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define RUFF_MAGIC "RUFF"
#define RUFF_MAGIC_LEN 4

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
    NoHuffman* temp[512];

    while (!fila_vazia(*fil)) {
        NoHuffman* atual;
        recupere_da_fila(*fil, (ElementoDeFila*)&atual);
        remova_elemento_da_fila(fil);
        temp[tam++] = atual;
    }

    int inserido = 0;
    for (int i = 0; i < tam; i++) {
        if (!inserido && novo->frequencia <= temp[i]->frequencia) {
            guarde_na_fila(fil, (ElementoDeFila)novo);
            inserido = 1;
        }
        guarde_na_fila(fil, (ElementoDeFila)temp[i]);
    }
    if (!inserido) {
        guarde_na_fila(fil, (ElementoDeFila)novo);
    }
}

NoHuffman* monta_arvore(Fila* fil) {
    while (1) {
        NoHuffman* esq;
        recupere_da_fila(*fil, (ElementoDeFila*)&esq);
        remova_elemento_da_fila(fil);

        if (fila_vazia(*fil)) {
            return esq;
        }

        NoHuffman* dir;
        recupere_da_fila(*fil, (ElementoDeFila*)&dir);
        remova_elemento_da_fila(fil);

        NoHuffman* pai = cria_no_interno(esq, dir);
        insere_ordenado(fil, pai);
    }
}

void imprime_arvore(NoHuffman* raiz, int nivel) {
    if (raiz == NULL) return;
    imprime_arvore(raiz->direita, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("    ");
    if (raiz->esquerda == NULL && raiz->direita == NULL)
        printf("[%u:%d]\n", (unsigned char)raiz->caractere, raiz->frequencia);
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

void gera_codigos(NoHuffman* raiz, Codigo* atual, Codigo tabela[256]) {
    if (raiz == NULL) return;

    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        unsigned char idx = (unsigned char)raiz->caractere;
        clone(*atual, &tabela[idx]);
        return;
    }

    adiciona_bit(atual, 0);
    gera_codigos(raiz->esquerda, atual, tabela);
    joga_fora_bit(atual);

    adiciona_bit(atual, 1);
    gera_codigos(raiz->direita, atual, tabela);
    joga_fora_bit(atual);
}

boolean compacta_dados(U8* dados, U64 n_bytes,
                       Codigo tabela[256], Codigo* saida) {
    if (!novo_codigo(saida)) return false;
    for (U64 i = 0; i < n_bytes; i++) {
        U8 c = dados[i];
        if (tabela[c].byte == NULL) {
            free_codigo(saida);
            return false;
        }
        char* bits = toString(tabela[c]);
        if (bits == NULL) {
            free_codigo(saida);
            return false;
        }
        for (int j = 0; bits[j] != '\0'; j++) {
            adiciona_bit(saida, bits[j] == '1' ? 1 : 0);
        }
        free(bits);
    }
    return true;
}

U8* descompacta_dados(Codigo compactado, NoHuffman* raiz,
                      U64 n_bytes_original) {
    if (n_bytes_original == 0) {
        /* arquivo vazio — retorna buffer vazio (alocado de 1 byte só pra não retornar NULL) */
        U8* r = (U8*)malloc(1);
        return r;
    }

    U8* resultado = (U8*)malloc(n_bytes_original * sizeof(U8));
    if (resultado == NULL) return NULL;

    /* Caso especial: árvore com UM só nó (arquivo de um byte único repetido).
     * O código de Huffman não gera bits nesse caso, então decodificamos
     * simplesmente replicando o único byte. */
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        for (U64 i = 0; i < n_bytes_original; i++)
            resultado[i] = (U8)raiz->caractere;
        return resultado;
    }

    char* bits = toString(compactado);
    if (bits == NULL) {
        free(resultado);
        return NULL;
    }

    U64 pos = 0;
    NoHuffman* atual = raiz;
    for (U64 i = 0; i < compactado.tamanho && pos < n_bytes_original; i++) {
        if (bits[i] == '0')
            atual = atual->esquerda;
        else
            atual = atual->direita;

        if (atual->esquerda == NULL && atual->direita == NULL) {
            resultado[pos++] = (U8)atual->caractere;
            atual = raiz;
        }
    }

    free(bits);
    return resultado;
}

NoHuffman* monta_arvore_de_frequencias(U32 frequencia[256]) {
    NoHuffman* nos[256];
    int total_nos = 0;

    /* cria folhas em ordem de byte (0..255) — ordem estável e reproduzível */
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0) {
            nos[total_nos] = cria_no_folha((char)i, (int)frequencia[i]);
            if (nos[total_nos] == NULL) {
                for (int k = 0; k < total_nos; k++) free(nos[k]);
                return NULL;
            }
            total_nos++;
        }
    }

    if (total_nos == 0) return NULL;

    /* selection sort por frequência (ascendente). Empates mantêm ordem de byte. */
    for (int i = 0; i < total_nos - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < total_nos; j++) {
            if (nos[j]->frequencia < nos[min_idx]->frequencia)
                min_idx = j;
        }
        if (min_idx != i) {
            NoHuffman* temp = nos[i];
            nos[i] = nos[min_idx];
            nos[min_idx] = temp;
        }
    }

    /* Caso de 1 só byte distinto: a raiz é a própria folha. */
    if (total_nos == 1) {
        return nos[0];
    }

    Fila fil;
    if (!nova_fila(&fil, 256)) {
        for (int i = 0; i < total_nos; i++) free_arvore(nos[i]);
        return NULL;
    }

    for (int i = 0; i < total_nos; i++) {
        guarde_na_fila(&fil, (ElementoDeFila)nos[i]);
    }

    NoHuffman* raiz = monta_arvore(&fil);
    free_fila(&fil);
    return raiz;
}

boolean gera_tabela(NoHuffman* raiz, Codigo tabela[256]) {
    for (int i = 0; i < 256; i++) {
        tabela[i].byte = NULL;
        tabela[i].tamanho = 0;
        tabela[i].capacidade = 0;
    }

    if (raiz == NULL) return true;

    /* Caso especial: 1 folha só — atribui código "0" manualmente. */
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        U8 idx = (U8)raiz->caractere;
        if (!novo_codigo(&tabela[idx])) return false;
        adiciona_bit(&tabela[idx], 0);
        return true;
    }

    Codigo atual;
    if (!novo_codigo(&atual)) return false;
    gera_codigos(raiz, &atual, tabela);
    free_codigo(&atual);
    return true;
}

static void libera_tabela(Codigo tabela[256]) {
    for (int i = 0; i < 256; i++) {
        if (tabela[i].byte != NULL) free_codigo(&tabela[i]);
    }
}

/* --- Compactação em arquivo ------------------------------------------- */
boolean compacta_arquivo(const char* nome_entrada, const char* nome_saida) {
    FILE* fin = fopen(nome_entrada, "rb");
    if (fin == NULL) {
        printf("Erro: nao foi possivel abrir '%s' para leitura.\n", nome_entrada);
        return false;
    }

    fseek(fin, 0, SEEK_END);
    long tam_l = ftell(fin);
    if (tam_l < 0) { fclose(fin); return false; }
    U64 tam_original = (U64)tam_l;
    fseek(fin, 0, SEEK_SET);

    U8* dados = NULL;
    if (tam_original > 0) {
        dados = (U8*)malloc(tam_original);
        if (dados == NULL) {
            fclose(fin);
            printf("Erro: sem memoria para ler '%s'.\n", nome_entrada);
            return false;
        }
        if (fread(dados, 1, tam_original, fin) != tam_original) {
            free(dados); fclose(fin);
            printf("Erro: falha ao ler '%s'.\n", nome_entrada);
            return false;
        }
    }
    fclose(fin);

    /* Contar frequências */
    U32 frequencia[256] = {0};
    for (U64 i = 0; i < tam_original; i++) {
        frequencia[dados[i]]++;
    }

    /* Montar árvore e gerar tabela (só se tiver conteúdo) */
    NoHuffman* raiz = NULL;
    Codigo tabela[256];
    for (int i = 0; i < 256; i++) { tabela[i].byte = NULL; tabela[i].tamanho = 0; tabela[i].capacidade = 0; }
    Codigo compactado; compactado.byte = NULL; compactado.tamanho = 0; compactado.capacidade = 0;

    if (tam_original > 0) {
        raiz = monta_arvore_de_frequencias(frequencia);
        if (raiz == NULL) { free(dados); return false; }

        if (!gera_tabela(raiz, tabela)) {
            free_arvore(raiz); free(dados); return false;
        }

        if (!compacta_dados(dados, tam_original, tabela, &compactado)) {
            printf("Erro: falha ao compactar os dados.\n");
            libera_tabela(tabela);
            free_arvore(raiz); free(dados); return false;
        }
    }

    /* Escrever o .ruff */
    FILE* fout = fopen(nome_saida, "wb");
    if (fout == NULL) {
        printf("Erro: nao foi possivel criar '%s'.\n", nome_saida);
        if (compactado.byte) free_codigo(&compactado);
        libera_tabela(tabela);
        if (raiz) free_arvore(raiz);
        free(dados);
        return false;
    }

    /* Cabeçalho: magic + tam_original + n_bits + frequencias[256] */
    fwrite(RUFF_MAGIC, 1, RUFF_MAGIC_LEN, fout);
    fwrite(&tam_original, sizeof(U64), 1, fout);

    U64 n_bits = compactado.tamanho;
    fwrite(&n_bits, sizeof(U64), 1, fout);

    fwrite(frequencia, sizeof(U32), 256, fout);

    /* Dados compactados */
    if (n_bits > 0) {
        U64 n_bytes = (n_bits + 7) / 8;
        /* Máscara no último byte para zerar bits não utilizados (evita lixo no arquivo) */
        if (n_bits % 8 != 0) {
            U8 mask = (U8)((1u << (n_bits % 8)) - 1);
            compactado.byte[n_bytes - 1] &= mask;
        }
        fwrite(compactado.byte, 1, n_bytes, fout);
    }

    fclose(fout);

    /* Cleanup */
    if (compactado.byte) free_codigo(&compactado);
    libera_tabela(tabela);
    if (raiz) free_arvore(raiz);
    free(dados);

    return true;
}

/* --- Descompactação do arquivo ---------------------------------------- */
boolean descompacta_arquivo(const char* nome_entrada, const char* nome_saida) {
    FILE* fin = fopen(nome_entrada, "rb");
    if (fin == NULL) {
        printf("Erro: nao foi possivel abrir '%s' para leitura.\n", nome_entrada);
        return false;
    }

    /* Valida o magic */
    char magic[RUFF_MAGIC_LEN];
    if (fread(magic, 1, RUFF_MAGIC_LEN, fin) != RUFF_MAGIC_LEN ||
        memcmp(magic, RUFF_MAGIC, RUFF_MAGIC_LEN) != 0) {
        printf("Erro: '%s' nao e um arquivo .ruff valido.\n", nome_entrada);
        fclose(fin);
        return false;
    }

    U64 tam_original = 0, n_bits = 0;
    if (fread(&tam_original, sizeof(U64), 1, fin) != 1 ||
        fread(&n_bits,       sizeof(U64), 1, fin) != 1) {
        printf("Erro: cabecalho corrompido em '%s'.\n", nome_entrada);
        fclose(fin);
        return false;
    }

    U32 frequencia[256];
    if (fread(frequencia, sizeof(U32), 256, fin) != 256) {
        printf("Erro: tabela de frequencias corrompida em '%s'.\n", nome_entrada);
        fclose(fin);
        return false;
    }

    /* Arquivo original vazio */
    if (tam_original == 0) {
        fclose(fin);
        FILE* fout = fopen(nome_saida, "wb");
        if (fout == NULL) return false;
        fclose(fout);
        return true;
    }

    /* Lê os bytes compactados */
    U64 n_bytes_comp = (n_bits + 7) / 8;
    Codigo compactado;
    compactado.byte = NULL; compactado.tamanho = 0; compactado.capacidade = 0;

    if (n_bytes_comp > 0) {
        compactado.byte = (U8*)malloc(n_bytes_comp);
        if (compactado.byte == NULL) {
            fclose(fin); return false;
        }
        if (fread(compactado.byte, 1, n_bytes_comp, fin) != n_bytes_comp) {
            printf("Erro: dados compactados truncados em '%s'.\n", nome_entrada);
            free(compactado.byte); fclose(fin); return false;
        }
        compactado.tamanho = n_bits;
        compactado.capacidade = n_bytes_comp * 8;
    }
    fclose(fin);

    /* Reconstrói a árvore — MESMA ordenação da compactação */
    NoHuffman* raiz = monta_arvore_de_frequencias(frequencia);
    if (raiz == NULL) {
        if (compactado.byte) free(compactado.byte);
        printf("Erro: nao foi possivel reconstruir a arvore.\n");
        return false;
    }

    /* Decodifica */
    U8* dados = descompacta_dados(compactado, raiz, tam_original);
    if (dados == NULL) {
        free_arvore(raiz);
        if (compactado.byte) free(compactado.byte);
        printf("Erro: falha na descompactacao.\n");
        return false;
    }

    /* Escreve saída */
    FILE* fout = fopen(nome_saida, "wb");
    if (fout == NULL) {
        printf("Erro: nao foi possivel criar '%s'.\n", nome_saida);
        free(dados); free_arvore(raiz);
        if (compactado.byte) free(compactado.byte);
        return false;
    }
    fwrite(dados, 1, tam_original, fout);
    fclose(fout);

    free(dados);
    free_arvore(raiz);
    if (compactado.byte) free(compactado.byte);

    return true;
}
