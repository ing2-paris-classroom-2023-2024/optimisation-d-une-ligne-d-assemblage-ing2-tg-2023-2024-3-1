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


void contrainte_exclusion(t_operation* tabOpe, int ordre){
    FILE *file = fopen("../exclusions.txt", "r");
    if (!file){
        printf("probleme lecture du fichier exclusions.txt");
        exit(-1);
    }

    //Récupération de la valeur du sommet la plus élevée pour créer la matrice adéquate
    int ordreExclusion = 0, op1, op2;

    while (fscanf(file, "%d %d", &op1, &op2) == 2) {
        if (op1 > ordreExclusion) ordreExclusion = op1;
        if (op2 > ordreExclusion) ordreExclusion = op2;
    }
    rewind(file); //on revient au début du fichier

    //Initialisation de la matrice dynamique
    int** matriceExclusion = (int **) malloc(sizeof (int*) * ordreExclusion);
    for (int i = 0; i < ordreExclusion; ++i) {
        matriceExclusion[i] = (int*) malloc(sizeof (int) * ordreExclusion);
    }

    for (int i = 0; i < ordreExclusion; i++) {
        for (int j = 0; j < ordreExclusion; j++) {
            matriceExclusion[i][j] = 0;
        }
    }



