#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre(); // Vous devez définir lire_ordre() en fonction de votre logique
    int nombre_arrete = lire_nb_arrete(); // Vous devez définir lire_nb_arrete() en fonction de votre logique
    printf("ordre: %d, nombre d'aretes: %d\n", ordre, nombre_arrete);
    ordre++;
    nombre_arrete++;




    t_operation *tabOpe = (t_operation *) malloc(sizeof(t_operation) * ordre);

    contrainte_exclusion(tabOpe, ordre);

    contrainte_precedence_temps(tabOpe, ordre);

    //contrainte_exclusion_temps(tabOpe, ordre);

    //free(tabOpe);


    // creation arrete pour precedence
    t_relation *tableau_relations = (t_relation *)malloc(sizeof(t_relation) * nombre_arrete);

    cree_les_liens(nombre_arrete, tableau_relations);


    // creation tableau de sommet pour la precedence
    t_operation *tableau_operations = (t_operation *)malloc(sizeof(t_operation) * ordre);

    cree_les_sommets(ordre, tableau_operations);


    //appele pour tout les sommet la fonction contrainte de precedence
    for (int i = 0; i < ordre; i++) {
        int z = tabOpe[i].numero;
        contrainte_de_precedence(nombre_arrete, ordre, z, tableau_relations, tabOpe);
        printf("\n");
    }


    //liberation des tableau precedences
    //free(tableau_relations);
    //free(tableau_operations);

    return 0;
}
