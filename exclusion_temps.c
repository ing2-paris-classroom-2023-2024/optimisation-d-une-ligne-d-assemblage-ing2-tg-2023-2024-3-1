#include "ligne_assemblage.h"


void contrainte_exclusion_temps(t_operation* tabOpe, int ordre) {
    FILE *file = fopen("../exclusions.txt", "r");
    if (!file){
        printf("probleme lecture du fichier exclusions.txt");
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

    //On remet le tableau d'opérations en ordre
    creer_operation(tabOpe, ordre);


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
        if (tabOpe[i].station == 0) {  // Si aucune station n'est encore assignée
            int station = 1;
            int station_valide;

            do {
                station_valide = 1;
                for (int j = 0; j < ordre; j++) {
                    if (matriceExclusion[tabOpe[i].numero - 1][tabOpe[j].numero - 1] == 1 && station == tabOpe[j].station) {
                        station_valide = 0;
                        break;
                    }
                }
                if (!station_valide) {
                    station++;
                }
            } while (!station_valide);

            tabOpe[i].station = station;
        }
    }

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

    // Affichage des opérations par station/couleur

    printf("\ncontrainte exclusion x temps\n");
    afficher_stations(tabOpe ,ordre);

    //libération de la mémoire
    for (int i = 0; i < ordreExclusion; ++i) {
        free(matriceExclusion[i]);
    }
    free(matriceExclusion);

    fclose(file);
    fclose(file1);

}