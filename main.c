#include "ligne_assemblage.h"

int main() {
    int ordre = lire_ordre(); // Vous devez définir lire_ordre() en fonction de votre logique
    int nombre_arrete = lire_nb_arrete(); // Vous devez définir lire_nb_arrete() en fonction de votre logique
    printf("ordre: %d, nombre d'aretes: %d\n", ordre, nombre_arrete);
    ordre++;
    nombre_arrete++;


    // creation arrete pour precedence
    t_relation *tableau_relations;
    tableau_relations = (t_relation *)malloc(sizeof(t_relation) * nombre_arrete);
    cree_les_liens(nombre_arrete, tableau_relations);


    // creation tableau de sommet pour la precedence
    t_operation *tableau_operations;
    tableau_operations = (t_operation *)malloc(sizeof(t_operation) * ordre);
    cree_les_sommets(ordre, tableau_operations);


    //appele pour tout les sommet la fonction contrainte de precedence
    for (int i = 1; i < 36; i++) {
        contrainte_de_precedence(nombre_arrete, ordre, i, tableau_relations, tableau_operations);
        printf("\n");
    }

    //liberation des tableau precedences
    free(tableau_relations);
    free(tableau_operations);

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



//fonction qui lit et qui initialise les structurses des sommets qui pointe vers dans un tableau
void cree_les_sommets(int nb_sommet, t_operation *tableau_operations) {
    FILE *fichier = fopen("../operations.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int i = 0;
    while (i < nb_sommet && fscanf(fichier, "%d %f", &tableau_operations[i].numero, &tableau_operations[i].temps) == 2) {
        i++;
    }

    fclose(fichier);
}

//fonction qui lit et qui initialise les structures des arretes qui pointe vers dans un tableau
void cree_les_liens(int nb_arrete, t_relation *relations) {
    FILE *fichier = fopen("../precedences.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int i = 0;
    while (i < nb_arrete && fscanf(fichier, "%d %d", &relations[i].origine.numero, &relations[i].destination.numero) == 2) {
        i++;
    }

    fclose(fichier);
}


void contrainte_de_precedence(int nombre_relations, int nb_sommet, int start, t_relation *relations, t_operation *operations) {
    int *visite = (int *)malloc(sizeof(int) * nb_sommet);

    for (int i = 0; i < nb_sommet; i++) {
        visite[i] = 0;
    }

    // Skip the starting vertex when printing the result
    printf("l'ope %d doit avoir ete precede par : ", start);

    //appele du bfs qui est inverser
    bfs(start, visite, relations, nb_sommet);

    printf("\n");

    free(visite);
}


void bfs(int sommet_depart, int *visite, t_relation *relations, int nb_sommets) {
    int *file = (int *)malloc(sizeof(int) * nb_sommets);
    int avant = 0, arriere = 0;


    file[arriere++] = sommet_depart;
    visite[sommet_depart - 1] = 1;

    while (avant < arriere) {
        int actuel = file[avant++];
        // permet de ne pas mettre le sommet du quel on part
        if (actuel != sommet_depart) {
            printf("%d ", actuel);
        }
        for (int i = 0; i < nb_sommets; i++) {
            if (relations[i].destination.numero == actuel && visite[relations[i].origine.numero - 1] == 0) {
                //ajoute les sommet adjasant dans le tableau
                file[arriere++] = relations[i].origine.numero;
                visite[relations[i].origine.numero - 1] = 1;
            }
        }
    }

    free(file);
}
