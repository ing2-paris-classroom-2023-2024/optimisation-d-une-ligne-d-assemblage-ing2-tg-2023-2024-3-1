#include <stdio.h>
#include <stdlib.h>
#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre();
    printf("%d", ordre);
    return 0;
}

int lire_ordre(){
    FILE *file = fopen("../operations.txt", "r");
    if (!file){
        printf("probleme lecture du fichier operations.txt");
        exit(-1);
    }
    int ordre = 0;
    int c;
    while ((c=fgetc(file))!= EOF){
        if (c == '\n'){
            ordre++;
        }
    }
    return ordre;
}


