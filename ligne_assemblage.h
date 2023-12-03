//
// Created by perra on 24/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct operation {
    int numero;
    float temps;
    int degre;
    int station;
    double debut;
} t_operation;


//structures pour la précédence
struct Arc {
    int sommet;
    struct Arc* arc_suivant;
};

typedef struct Arc* pArc; //Pointeur sur un arc

struct Sommet {
    pArc arc;
    int nbPred;
};

typedef struct Sommet* pSommet; //Pointeur sur un sommet

typedef struct Graphe {
    int ordre;
    int* sommetsExistants;
    pSommet* pSommet;
} Graphe;

int lire_ordre();

void creer_operation(t_operation* tabOpe, int ordre);

void afficher_stations(t_operation *tabOpe, int ordre);

Graphe* creer_graphe(int ordre);

void creer_arete(Graphe* graphe, int s1, int s2);

void sommets_source(Graphe* graphe, int* sources, int* nbSources);

void contrainte_exclusion(t_operation* tabOpe, int ordre);

void contrainte_temps(t_operation *tabOpe, int ordre);

void contrainte_de_precedence(t_operation* tabOpe, int ordre);

void contrainte_precedence_temps(t_operation* tabOpe, int ordre);

void contrainte_exclusion_temps(t_operation* tabOpe, int ordre);

void contrainte_exclusion_precedence(t_operation* tabOpe, int ordre);

void contrainte_exclusion_precedenc_temps(t_operation* tabOpe, int ordre);

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
