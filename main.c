#include <stdio.h>
#include <stdlib.h>
#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre();
    t_operation* tabOpe = (t_operation*) malloc(sizeof (t_operation) * ordre);
    creer_operation(tabOpe, ordre);
    for (int i = 0; i < ordre; ++i) {
        printf("%d %0.2f\n", tabOpe[i].numero, tabOpe[i].temps);
    }
    contrainte_exclusion(tabOpe, ordre);

    free(tabOpe);
    return 0;
}

int lire_ordre(){
    FILE *file = fopen("../operations.txt", "r");
    if (!file){
        printf("probleme lecture du fichier operations.txt");
        exit(-1);
    }

    //récupération du nombre de lignes du fichier
    int ordre = 0;
    int c;
    while ((c=fgetc(file))!= EOF){ //EOF = End Of File
        if (c == '\n'){
            ordre++;
        }
    }
    fclose(file);
    return ordre;
}

void creer_operation(t_operation* tabOpe, int ordre){
    FILE *file = fopen("../operations.txt", "r");
    if (!file){
        printf("probleme lecture du fichier operations.txt");
        exit(-1);
    }

    //Initialisation du tableau comportant toutes les données des opérations
    for (int i = 0; i < ordre; i++) {
        fscanf(file,"%d %f",&tabOpe[i].numero, &tabOpe[i].temps);
        tabOpe[i].degre = 0;
        tabOpe[i].couleur = 0;
    }

    fclose(file);
}


