//
// Created by perra on 24/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
#include <stdio.h>
#include <stdlib.h>

// Structure pour le tableau dynamique
typedef struct {
    int *tableau;
    int taille;
} struct_Tableau;

typedef struct operation {
    int numero;
    float temps;
    int degre;
    int couleur; //pour exclusion
    int station; //pour precedence
    double debut; //pour temps
    struct_Tableau *tableau_ptr; // pour mettre toutes les opé à effectuer avant
} t_operation;

typedef struct {
    t_operation origine;
    t_operation destination;
} t_relation;

int lire_ordre();

int lire_cycle();

int lire_nb_arrete();

void creer_operation(t_operation* tabOpe, int ordre);

void afficher_stations(t_operation *tabOpe, int ordre);

void contrainte_exclusion(t_operation* tabOpe, int ordre);

void contrainte_temps(t_operation *tabOpe, int ordre);

void contrainte_precedence_temps(t_operation* tabOpe, int ordre);

void cree_les_sommets(int nb_sommet, t_operation *tableau_operations);

void cree_les_liens(int nb_arrete, t_relation *relations);

void contrainte_de_precedence(int nombre_relations, int nb_sommet, int start, t_relation *relations, t_operation *operations);

void bfs(int start, int *visited, t_relation *relations, int nb_sommet);
#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
