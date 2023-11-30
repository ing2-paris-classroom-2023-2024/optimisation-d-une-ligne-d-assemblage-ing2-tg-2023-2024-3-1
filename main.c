#include "ligne_assemblage.h"


int main() {
    int ordre = lire_ordre();
    int nb_arrete = lire_nb_arrete();
    printf("ordre: %d, nombre d'aretes: %d\n", ordre, nb_arrete);
    printf("ok\n");

    t_operation *tabOpe = NULL;
    cree_struct_sommet(ordre, &tabOpe);
    printf("ok1\n");

    t_relation *relations = NULL;
    cree_les_liens(nb_arrete, &relations);
    printf("ok2\n");

    mettre_indice_origine(nb_arrete, &relations);

    //afficher la contrainte de precedence
    for (int i = 1; i <= ordre; i++) {
        contrainte_de_precedence(nb_arrete, i, &relations);
        printf("\n");
    }

    // Additional code for other constraints
    // contrainte_exclusion(tabOpe, ordre);
    // contrainte_precedence_temps(tabOpe, ordre);
    // contrainte_exclusion_temps(tabOpe, ordre);

    // Free allocated memory
    free(tabOpe);
    free(relations);

    return 0;
}


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

int lire_nb_arrete() {
    FILE *file = fopen("../precedences.txt", "r");
    if (!file) {
        printf("probleme lecture du fichier precedences.txt");
        exit(-1);
    }

    //récupération du nombre de lignes du fichier
    int nombre_arrete = 0;
    int c;
    while ((c = fgetc(file)) != EOF) { //EOF = End Of File
        if (c == '\n') {
            nombre_arrete++;
        }
    }
    fclose(file);
    return nombre_arrete;
}

int lire_cycle(){
    // ouvrir le fichier cycle changer le nom ici si la valeur du temps_cycle n'est le fichier nomme cycle
    FILE *file = fopen("../temps_cycle.txt", "r");//modifier le nom du fichier cycle si besoin
    if (!file) {
        printf("probleme ouverture du fichier");
        exit(-1);
    }
    // lecture de la valeur dans le fichier
    int cycle;
    if (fscanf(file, "%d", &cycle) != 1) {
        fprintf(stderr, "Erreur lors de la lecture de l'entier depuis le fichier\n");
        exit(-1);
    }

    fclose(file);
    return cycle;
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
        tabOpe[i].couleur = 0;
        tabOpe[i].station = 0;
    }

    fclose(file);
}

void initialiser_tableau_a_0(struct Tableau *tableau, int taille) {
    tableau->tableau = (int *)malloc(taille * sizeof(int));
    tableau->taille = taille;

    if (tableau->tableau == NULL) {
        perror("Erreur allocation mémoire dans initialiser_tableau");
        exit(-1);
    }

    for (int i = 0; i < taille; i++) {
        tableau->tableau[i] = 0;
    }
}



void contrainte_exclusion(t_operation* tabOpe, int ordre){
    FILE *file = fopen("../exclusions.txt", "r");
    if (!file){
        printf("probleme lecture du fichier exclusions.txt");
        exit(-1);
    }

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

    // Affichage des opérations par station/couleur
    printf("\ncontrainte exclusion\n");
    afficher_stations(tabOpe, ordre);

    //libération de la mémoire
    for (int i = 0; i < ordreExclusion; ++i) {
        free(matriceExclusion[i]);
    }
    free(matriceExclusion);

    fclose(file);
}


void contrainte_temps(t_operation *tabOpe, int ordre) {
    FILE *file1 = fopen("../temps_cycle.txt", "r");
    if (!file1) {
        printf("probleme lecture du fichier temps_cycle.txt");
        exit(-1);
    }
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





void initialiser_tableau(t_operation *donnees) {
    donnees->degre = 0;
    donnees->couleur = 0;
    donnees->station = 0;
}
void mettre_indice_origine(int nombre_relations, t_relation **relations) {
    for (int i = 0; i < nombre_relations; i++) {
        int indice_origine = (*relations)[i].origine.numero;
        int indice_destination = (*relations)[i].destination.numero;
        (*relations)[indice_destination - 1].destination.station = (*relations)[indice_origine - 1].origine.station;
    }
}

void cree_struct_sommet(int ordre, t_operation **donnees) {
    FILE *fichier = fopen("../operations.txt", "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    *donnees = (t_operation *)malloc(sizeof(t_operation) * ordre);

    int i = 0;
    while (fscanf(fichier, "%d %f", &(*donnees)[i].numero, &(*donnees)[i].temps) == 2) {
        // initialiser le tableau à 0 pour tout les structure t_operation
        initialiser_tableau(&(*donnees)[i]);
        i++;
    }

    fclose(fichier);
}

void cree_les_liens(int nb_arrete, t_relation **relations) {
    FILE *fichier = fopen("../precedences.txt", "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    *relations = (t_relation *)malloc(sizeof(t_relation) * nb_arrete);

    int i = 0;
    while (i < nb_arrete && fscanf(fichier, "%d %d", &(*relations)[i].origine.numero, &(*relations)[i].destination.numero) == 2) {
        i++;
    }

    fclose(fichier);
}

void contrainte_de_precedence(int nombre_relations, int start, t_relation **relations) {
    // Adaptation nécessaire ici selon la nouvelle structure
    int *queue = (int *)malloc(sizeof(int) * nombre_relations);
    int *visited = (int *)malloc(sizeof(int) * nombre_relations);

    int front = 0, rear = 0;
    queue[rear++] = start;
    visited[start] = 1;

    printf("l'ope %d, doit avoir ete precede par : ", start);

    while (front < rear) {
        int current = queue[front++];

        printf("%d ", current);

        for (int i = 0; i < nombre_relations; i++) {
            if ((*relations)[i].destination.numero == current && visited[(*relations)[i].origine.numero] == 0) {
                queue[rear++] = (*relations)[i].origine.numero;
                visited[(*relations)[i].origine.numero] = 1;
            }
        }
    }

    free(queue);
    free(visited);
}
