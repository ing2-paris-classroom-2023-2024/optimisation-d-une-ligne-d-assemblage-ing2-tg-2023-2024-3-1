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
    // ouvrir le fichier cycle changer le nom ici si la valeur du temps_cycle n'est le fichier nomme cycle
    FILE *fichier = fopen("../temps_cycle.txt", "r");//modifier le nom du fichier cycle si besoin
// message erreur si le fichier n'est pas trouver
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    // lecture de la valeur dans le fichier
    int cycle;
    if (fscanf(fichier, "%d", &cycle) != 1) {
        fprintf(stderr, "Erreur lors de la lecture de l'entier depuis le fichier\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
    return cycle;
}
