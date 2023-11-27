#include <stdio.h>
#include <stdlib.h>
#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre();
    t_operation *tabOpe = (t_operation *) malloc(sizeof(t_operation) * ordre);
    creer_operation(tabOpe, ordre);
    for (int i = 0; i < ordre; ++i) {
        printf("%d %0.2f\n", tabOpe[i].numero, tabOpe[i].temps);
    }
    contrainte_precedence_temps(tabOpe, ordre);
    free(tabOpe);
    return 0;
}

int lire_ordre() {
    FILE *file = fopen("../operations.txt", "r");
    if (!file) {
        printf("probleme lecture du fichier operations.txt");
        exit(-1);
    }

    //récupération du nombre de lignes du fichier
    int ordre = 0;
    int c;
    while ((c = fgetc(file)) != EOF) { //EOF = End Of File
        if (c == '\n') {
            ordre++;
        }
    }
    fclose(file);
    return ordre;
}

void creer_operation(t_operation *tabOpe, int ordre) {
    FILE *file = fopen("../operations.txt", "r");
    if (!file) {
        printf("probleme lecture du fichier operations.txt");
        exit(-1);
    }

    //Initialisation du tableau comportant toutes les données des opérations
    for (int i = 0; i < ordre; i++) {
        fscanf(file, "%d %f", &tabOpe[i].numero, &tabOpe[i].temps);
        tabOpe[i].degre = 0;
        tabOpe[i].couleur = 0;
        tabOpe[i].station = 0;
    }

    fclose(file);
}

void contrainte_temps(t_operation *tabOpe, int ordre, float temps_cycle) {
    int station_actuelle = 0;
    float temps_actuel = 0;

    for (int i = 0; i < ordre; i++) {
        if (temps_actuel + tabOpe[i].temps <= temps_cycle) {
            tabOpe[i].station = station_actuelle;
            tabOpe[i].debut = temps_actuel;
            temps_actuel += tabOpe[i].temps;
        } else {
            // Passer à la station suivante et réinitialiser le temps actuel
            station_actuelle++;
            temps_actuel = 0;
            i--; // Reconsidérer la même opération pour la nouvelle station
        }
    }

}

void afficher_stations(t_operation *tabOpe, int ordre) {
    // Afficher les opérations assignées à chaque station
    int max_station = 0;
    for (int i = 0; i < ordre; i++) {
        if (tabOpe[i].station > max_station) {
            max_station = tabOpe[i].station;
        }
    }

    for (int couleur = 0; couleur <= max_station; couleur++) {
        printf("Station %d : ", couleur);
        for (int i = 0; i < ordre; i++) {
            if (tabOpe[i].station == couleur) {
                printf("debut a %0.2f pour %d \n", tabOpe[i].debut, tabOpe[i].numero);
            }
        }
        printf("\n");
    }

}


void contrainte_precedence_temps(t_operation *tabOpe, int ordre) {
    FILE *file = fopen("../precedences.txt", "r");
    if (!file) {
        printf("probleme lecture du fichier precedences.txt");
        exit(-1);
    }

    FILE *file1 = fopen("../temps_cycle.txt", "r");
    if (!file1) {
        printf("probleme lecture du fichier temps_cycle.txt");
        exit(-1);
    }

    //récupération du temps de cycle dans le fichier
    float temps_cycle;
    fscanf(file1, "%f", &temps_cycle);

    //Récupération de la valeur du sommet la plus élevée pour créer la matrice adéquate
    int ordrePrecedence = 0, op1, op2;
    while (fscanf(file, "%d %d", &op1, &op2) == 2) {
        if (op1 > ordrePrecedence) {
            ordrePrecedence = op1;
        }
        if (op2 > ordrePrecedence) {
            ordrePrecedence = op2;
        }
    }
    rewind(file); //on revient au début du fichier

    // Initialisation de la matrice dynamique qui permet de stocker les informations du fichier
    int **matricePrecedence = (int **) malloc(ordrePrecedence * sizeof(int *));
    for (int i = 0; i < ordrePrecedence; i++) {
        matricePrecedence[i] = (int *) malloc(ordrePrecedence * sizeof(int));
        for (int j = 0; j < ordrePrecedence; j++) {
            matricePrecedence[i][j] = 0;  // Initialisation de la matrice à 0
        }
    }

    //Matrice non-symétrique, car il faut respecter l'ordre de précédence
    int num1, num2;
    while (fscanf(file, "%d %d", &num1, &num2) == 2) {
        matricePrecedence[num1 - 1][num2 - 1] = 1;
    }

    contrainte_temps(tabOpe, ordre, temps_cycle);

    afficher_stations(tabOpe, ordre);

    // On libère la mémoire
    for (int i = 0; i < ordre; i++) {
        free(matricePrecedence[i]);
    }
    free(matricePrecedence);
    fclose(file);
    fclose(file1);
}