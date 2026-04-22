#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
 
int main() {
    int resposta;
    char texto[200];
    int frequencia[256] = {0};
    NoHuffman* nos[256];
    int total_nos = 0;
    Fila fil;
 
    nova_fila(&fil, 256);
    
    printf("Insira o que você deseja fazer: \n 1- Compactar \n 2- Descompactar\n");
    scanf("%d%*c", &resposta); 
    
    if (resposta==1){
        printf("Insira o que você deseja compactar: ");
        fgets(texto, sizeof(texto), stdin);
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
    }
    if(resposta == 2){
        // No Huffman, a descompactação precisa da árvore montada.
        // Se você acabou de compactar, a árvore já está na variável 'raiz'.
        
        // Se você quer apenas descompactar um código que já existe:
        while (quantas_info_tem_na_fila(&fil) > 1) { // Ajuste para o nome real da sua função
            NoHuffman* esq = (NoHuffman*)desenfileire(&fil);
            NoHuffman* dir = (NoHuffman*)desenfileire(&fil);
            
            NoHuffman* pai = cria_no_interno(esq, dir);
            guarde_na_fila(&fil, (ElementoDeFila)pai);
        }
        
        NoHuffman* raiz_huffman = (NoHuffman*)desenfileire(&fil);

        Codigo compactado;
        char* descompactado;
        Codigo tabela[256] = {0}; // Inicializa a tabela de códigos
        
        // 2. Gera a tabela de tradução (A vira 01, B vira 10...)
        Codigo atual;
        novo_codigo(&atual);
        gera_codigos(raiz_huffman, atual, tabela);
        
        // 3. COMPACTA (Transforma o texto em bits)
        compacta_texto(texto, tabela, &compactado);
        printf("Texto Compactado (bits): %s\n", toString(compactado));
        
        // 4. DESCOMPACTA (O que você pediu!)
        descompactado = descompacta_texto(compactado, raiz);
        
        if (descompactado != NULL) {
            printf("\n--- RESULTADO DA DESCOMPACTACAO ---\n");
            printf("%s\n", descompactado);
            printf("------------------------------------\n");
            
            // Limpeza de memória
            free(descompactado);
        }
    }
    return 0;
}
