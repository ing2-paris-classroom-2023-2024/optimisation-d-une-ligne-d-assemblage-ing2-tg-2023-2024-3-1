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
    contrainte_exclusion(tabOpe, ordre);
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

    //Implémentation des valeurs du fichier dans la matrice qui est indexée à 0 et symétrique
    int num1, num2;
    while ((fscanf(file, "%d %d", &num1, &num2)) == 2){
        matriceExclusion[num1 - 1][num2 - 1] = 1;
        matriceExclusion[num2 - 1][num1 - 1] = 1;

    }

    // Calcul des degrés pour chaque opération
    for (int i = 0; i < ordre; i++) {
        for (int j = 0; j < ordre; j++) {
            if (matriceExclusion[tabOpe[i].numero - 1][tabOpe[j].numero - 1] == 1) {
                tabOpe[i].degre++;
            }
        }
    }

    // Tri des opérations par degré décroissant
    for (int i = 0; i < ordre - 1; i++) {
        for (int j = 0; j < ordre - i - 1; j++) {
            if (tabOpe[j].degre < tabOpe[j + 1].degre) {
                t_operation temp = tabOpe[j];
                tabOpe[j] = tabOpe[j + 1];
                tabOpe[j + 1] = temp;
            }
        }
    }

    // Algorithme de Welsh Powell
    for (int i = 0; i < ordre; i++) {
        if (tabOpe[i].couleur == 0) {  // Si aucune couleur n'est encore assignée
            int couleur = 1;
            int couleur_valide;

            do {
                couleur_valide = 1;
                for (int j = 0; j < ordre; j++) {
                    if (matriceExclusion[tabOpe[i].numero - 1][tabOpe[j].numero - 1] == 1 && couleur == tabOpe[j].couleur) {
                        couleur_valide = 0;
                        break;
                    }
                }
                if (!couleur_valide) {
                    couleur++;
                }
            } while (!couleur_valide);

            tabOpe[i].couleur = couleur;
        }
    }

    // Affichage des opérations par station/couleur

    //Récupération de la couleur maximale après le passage de Welsh Powell
    int max_couleur = 0;
    for (int i = 0; i < ordre; i++) {
        if (tabOpe[i].couleur > max_couleur) {
            max_couleur = tabOpe[i].couleur;
        }
    }

    // Pour chaque couleur, afficher les opérations correspondantes
    for (int couleur = 1; couleur <= max_couleur; couleur++) {
        printf("Station %d : ", couleur);
        for (int i = 0; i < ordre; i++) {
            if (tabOpe[i].couleur == couleur) {
                printf("%d ", tabOpe[i].numero);
            }
        }
        printf("\n");
    }

    //libération de la mémoire
    for (int i = 0; i < ordreExclusion; ++i) {
        free(matriceExclusion[i]);
    }
    free(matriceExclusion);

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