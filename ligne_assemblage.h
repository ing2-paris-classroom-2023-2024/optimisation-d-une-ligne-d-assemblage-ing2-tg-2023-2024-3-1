//
// Created by perra on 24/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
/* Alias d'un Graphe */
typedef struct operation
{
    int numero;
    float temps;
    int degre;
    int couleur;
} t_operation;

int lire_ordre();

void creer_operation(t_operation* tabOpe, int ordre);

void contrainte_exclusion(t_operation* tabOpe, int ordre);

void contrainte_precedence(t_operation* tabOpe, int ordre);

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_3_1_LIGNE_ASSEMBLAGE_H
