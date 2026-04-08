/*#include <stdio.h>
#include <stdlib.h>
#include "basics.h"

int main(){
    char text[100];
    printf("Digite um texto: ");
    fgets(text, sizeof(text), stdin);

    printf("%s", text);

    for(int i=0; text[i] != '\0' ; i++){
        
        printf("%c", text[i]);
    }
    return 0;
}*/
#include <stdio.h>
#include <string.h>
#include "basics.h"


int main() {
    char texto[200];
    int frequencia[256] = {0}; 

    printf("Digite uma frase: ");
    fgets(texto, sizeof(texto), stdin);

    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char caractere = (unsigned char)texto[i];
        frequencia[caractere]++;
    }

    printf("\nFrequencia dos caracteres:\n");
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0 && i > 31) {
            printf("'%c': %d vezes\n", i, frequencia[i]);
        }
    }

    return 0;
}