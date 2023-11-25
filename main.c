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


int lire_valeur_du_cycle()
{
    FILE *fichier = fopen("fichier.txt", "r");//modifier le nom du fichier cycle si besoin

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int cycle;
    if (fscanf(fichier, "%d", &cycle) != 1) {
        fprintf(stderr, "Erreur lors de la lecture de l'entier depuis le fichier\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
    printf("la valeur du cycle est de %d", cycle);
    return cycle;
}
