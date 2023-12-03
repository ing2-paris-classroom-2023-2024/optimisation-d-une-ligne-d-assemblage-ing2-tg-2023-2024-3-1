#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre();

    t_operation *tabOpe = (t_operation *) malloc(sizeof(t_operation) * ordre);

    contrainte_exclusion(tabOpe, ordre);

    contrainte_temps(tabOpe, ordre);

    contrainte_exclusion_temps(tabOpe, ordre);

    free(tabOpe);

    return 0;
}
