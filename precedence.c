#include "ligne_assemblage.h"

void contrainte_precedence(t_operation *tabOpe, int ordre) {
    FILE *file = fopen("../precedences.txt", "r");
    if (!file) {
        printf("probleme lecture du fichier precedences.txt");
        exit(-1);
    }
    creer_operation(tabOpe, ordre);

    contrainte_temps(tabOpe, ordre);

    printf("\ncontrainte temps x precedence\n");
    afficher_stations(tabOpe, ordre);

    fclose(file);
}