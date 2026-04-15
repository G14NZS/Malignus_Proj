#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basics.h"
#include "fila.h"

typedef struct {
  char caractere;
  int frequencia;
} no;

int main() {
    char texto[200];
    int frequencia[256] = {0}; 
    no n[256]; // Um array de 256 structs do tipo 'no'
    int total_nos = 0;
    Fila fil;
    nova_fila(&fil,256);


    printf("Digite uma frase: ");
    fgets(texto, sizeof(texto), stdin);

    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char caractere = (unsigned char)texto[i];
        frequencia[caractere]++;
    }

    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0 && i > 31) {
            printf("'%c': %d vezes\n", i, frequencia[i]);
        
        // Agora acessamos o índice 'total_nos' do array de structs
            n[total_nos].caractere = (char)i;
            n[total_nos].frequencia = frequencia[i];
            total_nos++;
        }
    }
    for (int i = 0; i<256; i++){
        int verificação = n[i].frequencia
        for (int j = i; j<256; j++){
            if (verificação<n[j].frequencia)
                verificação = n[j].frequencia
        
        
        guarde_na_fila(&fil, (ElementoDeFila)&n[total_nos]);
    
        
    }
    
    return 0;
}
