#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"

/* Remove o \n do final se houver (deixado pelo fgets) */
static void chomp(char* s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
    }
}

/* Tenta obter o tamanho de um arquivo. Retorna -1 em caso de erro. */
static long tamanho_arquivo(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (f == NULL) return -1;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fclose(f);
    return n;
}

int main(void) {
    char opcao[16];
    char nome_entrada[512];
    char nome_saida[512];

    printf("=== Compactador Huffman ===\n");
    printf("Escolha uma opcao:\n");
    printf("  1 - Compactar arquivo\n");
    printf("  2 - Descompactar .huff\n");
    printf("  0 - Sair\n");
    printf("Opcao: ");
    if (fgets(opcao, sizeof(opcao), stdin) == NULL) return 0;
    chomp(opcao);

    if (strcmp(opcao, "1") == 0) {
        printf("Arquivo de entrada (caminho do arquivo a compactar): ");
        if (fgets(nome_entrada, sizeof(nome_entrada), stdin) == NULL) return 1;
        chomp(nome_entrada);

        printf("Arquivo de saida: ");
        if (fgets(nome_saida, sizeof(nome_saida), stdin) == NULL) return 1;
        chomp(nome_saida);

        if (compacta_arquivo(nome_entrada, nome_saida)) {
            long orig = tamanho_arquivo(nome_entrada);
            long comp = tamanho_arquivo(nome_saida);
            printf("\nOK! Arquivo compactado com sucesso.\n");
            printf("  Original:    %ld bytes\n", orig);
            printf("  Compactado:  %ld bytes\n", comp);
            if (orig > 0)
                printf("  Razao:       %.2f%% do original\n",
                       100.0 * (double)comp / (double)orig);
        } else {
            printf("\nFalha na compactacao.\n");
            return 1;
        }

    } else if (strcmp(opcao, "2") == 0) {
        printf("Arquivo .huff de entrada: ");
        if (fgets(nome_entrada, sizeof(nome_entrada), stdin) == NULL) return 1;
        chomp(nome_entrada);

        printf("Arquivo de saida (onde gravar o conteudo original): ");
        if (fgets(nome_saida, sizeof(nome_saida), stdin) == NULL) return 1;
        chomp(nome_saida);

        if (descompacta_arquivo(nome_entrada, nome_saida)) {
            printf("\nOK! Arquivo descompactado em '%s'.\n", nome_saida);
        } else {
            printf("\nFalha na descompactacao.\n");
            return 1;
        }

    } else if (strcmp(opcao, "0") == 0) {
        return 0;
    } else {
        printf("Opcao invalida.\n");
        return 1;
    }

    return 0;
}
