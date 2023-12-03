#include "ligne_assemblage.h"

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
        tabOpe[i].station = 0;
    }

    fclose(file);
}


void contrainte_temps(t_operation *tabOpe, int ordre) {
    FILE *file1 = fopen("../temps_cycle.txt", "r");
    if (!file1) {
        printf("probleme lecture du fichier temps_cycle.txt");
        exit(-1);
    }

    creer_operation(tabOpe, ordre);

    //récupération du temps de cycle dans le fichier
    float temps_cycle;
    fscanf(file1, "%f", &temps_cycle);

    int station_actuelle = 1;
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


    printf("\ncontrainte temps\n");
    afficher_stations(tabOpe, ordre);
    fclose(file1);

}

void afficher_stations(t_operation *tabOpe, int ordre) {
    // Afficher les opérations assignées à chaque station
    int max_station = 0;
    for (int i = 0; i < ordre; i++) {
        if (tabOpe[i].station > max_station) {
            max_station = tabOpe[i].station;
        }
    }

    for (int station = 1; station <= max_station; station++) {
        printf("Station %d : ", station);
        for (int i = 0; i < ordre; i++) {
            if (tabOpe[i].station == station) {
                //printf("debut a %0.2f pour %d \n", tabOpe[i].debut, tabOpe[i].numero);
                printf("%d ", tabOpe[i].numero);
            }
        }
        printf("\n");
    }

}  
