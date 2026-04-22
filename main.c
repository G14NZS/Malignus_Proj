#include <stdio.h>
#include <string.h>
#include "huffman.h"

int main() {
    char texto[200];
    int freq[256] = {0};
    No* lista[256];
    int n = 0;

    printf("Digite o texto: ");
    fgets(texto, 200, stdin);
    texto[strcspn(texto, "\n")] = 0;

    for (int i = 0; texto[i]; i++)
        freq[(unsigned char)texto[i]]++;

    for (int i = 0; i < 256; i++)
        if (freq[i])
            lista[n++] = cria_no(i, freq[i]);

    No* raiz = monta_arvore(lista, n);

    Codigo tabela[256];
    for (int i = 0; i < 256; i++)
        novo_codigo(&tabela[i]);

    Codigo atual;
    novo_codigo(&atual);

    gera_codigos(raiz, atual, tabela);

    printf("\nCodigos:\n");
    for (int i = 0; i < 256; i++) {
        if (tabela[i].tamanho > 0) {
            char* s = toString(tabela[i]);
            printf("%c: %s\n", i, s);
            free(s);
        }
    }

    Codigo comp;
    compacta_texto(texto, tabela, &comp);

    char* bits = toString(comp);
    printf("\nCompactado:\n%s\n", bits);
    free(bits);

    char* decomp = descompacta(comp, raiz);
    printf("\nDescompactado:\n%s\n", decomp);

    free(decomp);
    free_codigo(&comp);
    free_arvore(raiz);

    return 0;
}