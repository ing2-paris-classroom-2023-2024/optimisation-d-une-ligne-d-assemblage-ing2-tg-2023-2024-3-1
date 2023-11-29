#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre();
    t_operation *tabOpe = (t_operation *) malloc(sizeof(t_operation) * ordre);
    creer_operation(tabOpe, ordre);
    for (int i = 0; i < ordre; ++i) {
        printf("%d %0.2f\n", tabOpe[i].numero, tabOpe[i].temps);
    }
    contrainte_exclusion(tabOpe, ordre);
    contrainte_precedence_temps(tabOpe, ordre);


    free(tabOpe);
    return 0;
}
