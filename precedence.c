#include "ligne_assemblage.h"

Graphe* creer_graphe(int ordre) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    graphe->ordre = ordre;
    graphe->pSommet = (pSommet*)malloc(ordre * sizeof(pSommet));
    graphe->sommetsExistants = (int*)malloc(ordre * sizeof(int));

    for (int i = 0; i < ordre; i++) {
        graphe->pSommet[i] = (pSommet)malloc(sizeof(struct Sommet));
        graphe->pSommet[i]->arc = NULL;
        graphe->pSommet[i]->nbPred = 0;
        graphe->sommetsExistants[i] = 0;
    }
    return graphe;
}

void creer_arete(Graphe* graphe, int s1, int s2) {

    // On marque les sommets existants à 1 (0 si non marqués)
    graphe->sommetsExistants[s1] = 1;
    graphe->sommetsExistants[s2] = 1;

    pArc nouvel_arc = (pArc)malloc(sizeof(struct Arc));
    nouvel_arc->sommet = s2;
    nouvel_arc->arc_suivant = graphe->pSommet[s1]->arc;
    graphe->pSommet[s1]->arc = nouvel_arc;
    graphe->pSommet[s2]->nbPred++;
}

void sommets_source(Graphe* graphe, int* sources, int* nbSources) {
    *nbSources = 0;

    //On déclare qu'un sommet est source s'il n'a pas de prédécesseurs et que ce sommet se trouve dans le fichier (on exclut le 0/14/19 etc...)
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->nbPred == 0 && graphe->sommetsExistants[i]) {
            sources[*nbSources] = i;
            (*nbSources)++;
        }
    }
}

void BFS(Graphe* graphe) {
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
    while (debut < fin) {

        int sommetCourant = fin - debut;
        printf("Station %d : ", compteurStation);

        for (int i = 0; i < sommetCourant; i++) {
            int sommetActuel = file[debut];
            debut++;
            printf("%d ", sommetActuel);

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
        compteurStation++;
        printf("\n");
    }

    free(visite);
    free(file);
}

void contrainte_de_precedence(t_operation* tabOpe, int ordre){
    FILE* file = fopen("../precedences.txt", "r");

    if (!file) {
        printf("probleme lecture du file precedences.txt");
        exit(-1);
    }

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

    printf("\ncontrainte precedence\n");

    BFS(graphe);

    fclose(file);
}


void contrainte_precedence_temps(t_operation *tabOpe, int ordre) {
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
