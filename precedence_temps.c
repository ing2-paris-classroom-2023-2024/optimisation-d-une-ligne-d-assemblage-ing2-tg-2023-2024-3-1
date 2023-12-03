
#include "ligne_assemblage.h"


void BFS_avec_temps(Graphe* graphe, float tempsCycle, t_operation* tabOpe) {
    int* visite = (int*)calloc(graphe->ordre , sizeof(int)); //initialisation à 0;

    //initialisation des variables et de la file pour le BFS
    int* file = (int*)malloc(graphe->ordre * sizeof(int));
    int debut = 0;
    int fin = 0;

    //On détermine quels sont les sommets sources, c'est-à-dire qui n'ont pas de précédences
    int sources[graphe->ordre];
    int nbSources;

    sommets_source(graphe, sources, &nbSources);

    for (int i = 0; i < nbSources; i++) {
        int source = sources[i];
        visite[source] = 1;
        file[fin] = source;
        fin++;
    }


    //Algorithme BFS
    int compteurStation = 1;
    float tempsStation = 0;

    while (debut < fin) {

        int sommetCourant = fin - debut;
        printf("Station %d : ", compteurStation);

        for (int i = 0; i < sommetCourant; i++) {
            int sommetActuel = file[debut];
            debut++;
            if (tempsStation + tabOpe[sommetActuel].temps <= tempsCycle) {
                // Ajoutez le sommet à la station actuelle
                printf("%d ", sommetActuel);
                tempsStation += tabOpe[sommetActuel].temps;
            } else {
                // Temps dépassé, passer à la station suivante
                compteurStation++;
                printf("\nStation %d : %d ", compteurStation, sommetActuel);
                tempsStation = tabOpe[sommetActuel].temps; // Réinitialiser le temps de la station
            }

            pArc arc = graphe->pSommet[sommetActuel]->arc;
            while (arc != NULL) {
                int sommet_adjacent = arc->sommet;

                if (!visite[sommet_adjacent] && --graphe->pSommet[sommet_adjacent]->nbPred == 0) {
                    visite[sommet_adjacent] = 1;
                    file[fin] = sommet_adjacent;
                    fin++;
                }

                arc = arc->arc_suivant;
            }
        }
        tempsStation = 0;
        compteurStation++;
        printf("\n");
    }

    free(visite);
    free(file);
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

    creer_operation(tabOpe, ordre);

    //récupération du temps de cycle dans le fichier
    float temps_cycle;
    fscanf(file1, "%f", &temps_cycle);


    //Récupération de la valeur du sommet la plus élevée pour récupérer l'ordre du graphe
    int ordrePrecedence = 0, op1, op2;

    while (fscanf(file, "%d %d", &op1, &op2) == 2) {
        if (op1 > ordrePrecedence) ordrePrecedence = op1;
        if (op2 > ordrePrecedence) ordrePrecedence = op2;
    }
    ordrePrecedence++; //on incrémente de 1 pour bien prendre en compte le sommet 35
    rewind(file); //on revient au début du fichier


    Graphe* graphe = creer_graphe(ordrePrecedence);

    int num1, num2;
    while (fscanf(file, "%d %d", &num1, &num2) == 2) {
        creer_arete(graphe, num1, num2);
    }

    printf("\ncontrainte precedence x temps\n");

    BFS_avec_temps(graphe, temps_cycle, tabOpe);

    fclose(file1);
    fclose(file);
}