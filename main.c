#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
 
int main() {
    char texto[200];
    int frequencia[256] = {0};
    NoHuffman* nos[256];
    int total_nos = 0;
    Fila fil;
 
    nova_fila(&fil, 256);
 
    strcpy(texto, "Eerie eyes seen over lake.");
    printf("Texto: %s\n\n", texto);
 
    // 1. Contar frequencia
    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char c = (unsigned char)texto[i];
        frequencia[c]++;
    }
 
    // 2. Criar nos folha
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0 && i > 31) {
            nos[total_nos] = cria_no_folha((char)i, frequencia[i]);
            total_nos++;
        }
    }
 
    // 3. Ordenar por frequencia (menor primeiro)
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
 
    // 4. Enfileirar
    for (int i = 0; i < total_nos; i++) {
        guarde_na_fila(&fil, (ElementoDeFila)nos[i]);
    }
 
    // 5. Montar a arvore
    NoHuffman* raiz = monta_arvore(&fil);
 
    // 6. Imprimir a arvore
    printf("Arvore de Huffman:\n\n");
    imprime_arvore(raiz, 0);
 
    // 7. Gerar codigos
    Codigo tabela[256];
    for (int i = 0; i < 256; i++) {
        tabela[i].byte = NULL;
        tabela[i].tamanho = 0;
        tabela[i].capacidade = 0;
    }
 
    Codigo codigo_atual;
    novo_codigo(&codigo_atual);
    gera_codigos(raiz, codigo_atual, tabela);
    free_codigo(&codigo_atual);
 
    // 8. Imprimir tabela de codigos
    printf("\nTabela de codigos:\n");
    for (int i = 0; i < 256; i++) {
        if (tabela[i].byte != NULL) {
            char* str = toString(tabela[i]);
            printf("'%c': %s\n", (char)i, str);
            free(str);
        }
    }
 
    // 9. Compactar o texto
    Codigo compactado;
    compacta_texto(texto, tabela, &compactado);
 
    char* bits = toString(compactado);
    printf("\nTexto compactado:\n%s\n", bits);
 
 
  // 10. Descompactar
    char* descompactado = descompacta_texto(compactado, raiz);
    printf("\nTexto descompactado: %s\n", descompactado);

    if (strcmp(texto, descompactado) == 0)
        printf("Descompactacao OK!\n");
    else
        printf("ERRO: texto descompactado diferente do original!\n");

    // Limpar
    free(descompactado);
    free_codigo(&compactado);
    for (int i = 0; i < 256; i++) {
        if (tabela[i].byte != NULL)
            free_codigo(&tabela[i]);
    }
    free_arvore(raiz);
    free_fila(&fil);

    return 0;
}
