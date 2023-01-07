#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/// @brief Explique l'utilisation du programme en cas d'erreur
/// @param argv La liste des arguments
void usage(argv)
{
    fprintf(stderr, "./exe chemin_input_file output_file\n");
    exit(1);
}

/// @brief Affiche le contenu d'un tableau 1 dimension
/// @param arr Le tableau à afficher
/// @param n La taille du tableau
void print_array(int *arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

/// @brief Affiche le contenu d'un tableau 2 dimensions (n*n)
/// @param arr Le tableau à afficher
/// @param n La taille du tableau
void print_array_2d(int **arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        print_array(arr[i], n);
    }
}

/// @brief Calcul le minimum entre deux entiers
/// @param a 
/// @param b 
/// @return Le minimum entre a et b
int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

/// @brief Calcul le maximum entre deux entiers
/// @param a
/// @param b
/// @return Le maximum entre a et b
int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

/// @brief Libère l'espace alloué par un tableau 2 dimensions
/// @param t Le tableau à libérer
/// @param n La taille du tableau
void free_arr_2d(int **t, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(t[i]);
    }
    free(t);
}

/// @brief Calcul du gain garantie dans la configuration où il ne reste que les pièces de i à j
///         Fonction récursive, appel initial : burning_coins_rec(0, n-1, v)
/// @param i L'indice de la pièce à l'extrémité gauche
/// @param j L'indice de la pièce à l'extrémité droite
/// @param v Le tableau contenant les valeurs des pièces
/// @return Le gain garantie indépendamment de la stratégie de l'adversaire
int burning_coins_rec(int i, int j, int *v)
{
    if (i > j)
        return 0;

    /* Cas où il ne reste que la ième pièce = > le choix est vite fait
    arrive quand il y a un nombre pair de pièce */
    if (i == j)
        return v[i];

    /* Cas où il ne reste que 2 pièces => on prend la plus elevée
    arrive quand il y a un nombre impair de pièce */
    if (i == j-1)
        return max(v[i], v[j]);

    /* Cas général, 4 possibilités:
        - si on prend la pièce à l'extremité gauche (ième):
            - l'adversaire prend ensuite celle à gauche ((i+1)ème)
            - l'adversaire prend ensuite celle à droite (jème)
        - si on prend la pièce à l'extremité droite (jème):
            - l'adversaire prend ensuite celle à gauche (ième)
            - l'adversaire prend ensuite celle à droite ((j-1)ème)
        Plus de détails dans le rapport
    */
    return max(
        min(
            v[i] + burning_coins_rec(i+2, j, v),
            v[i] + burning_coins_rec(i+1, j-1, v)
        ),
        min(
            v[j] + burning_coins_rec(i, j-2, v),
            v[j] + burning_coins_rec(i+1, j-1, v)
        )
    );
}

/// @brief Calcul du gain garantie en utilisant la programmation dynamique (sans optimisation mémoire)
/// @param v Le tableau contenant les valeurs des pièces
/// @param n La taille du tableau
/// @return Le gain garantie indépendamment de la stratégie de l'adversaire
int burning_coins_2d(int *v, int n)
{
    // Déclaration d'un tableau 2 dimensions n*n pour stocker les résultats des sous problèmes
    int **T = malloc(n * sizeof(int *));

    /*
    Initialisation du tableau avec les cas d'arrets (s'il reste 1 ou 2 pièces)
    j'ai décidé d'initialiser les diagonales pour les deux cas (nombre de pièce pair et impair)
    meme si une d'elle ne sera pas utilisée
    car n*n cases mémoire ont déjà été allouées et cela ne change pas l'algorithme
    */
    for (int i = 0; i < n; i++)
    {
        T[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                T[i][j] = v[i];
            else if (i == j+1)
                T[i][j] = max(v[i], v[j]);
            else
                T[i][j] = 0;
        }
    }

    // p : indice de début de boucle (cas nb pièce pair ou impair)
    int p = (n%2 == 1) ? 2 : 3;

    int k;
    for (int i = p; i < n; i += 2)
    {
        for (int j = 0; j < n-i; j++)
        {
            // k : indice qui permet de se déplacer sur la "ième diagonale"
            k = i+j;

            // Calcul et affectation de la valeur optimale dans la case [k, j]
            T[k][j] = max(
                min(
                    v[k] + T[k-1][j+1],
                    v[k] + T[k-2][j]
                ),
                min(
                    v[j] + T[k-1][j+1],
                    v[j] + T[k][j+2]
                )
            );
        }
    }

    // print_array_2d(T, n);
    free_arr_2d(T, n);
    return T[n-1][0];
}

/// @brief Calcul du gain garantie en utilisant la programmation dynamique avec optimisation mémoire
/// @param v Le tableau contenant les valeurs des pièces
/// @param n La taille du tableau
/// @return Le gain garantie indépendamment de la stratégie de l'adversaire
int burning_coins_1d(int *v, int n)
{
    // Déclaration d'un tableau de taille n
    int *T = malloc(n * sizeof(int));
    int p;

    // Initialisation du tableau selon la parité du nombre de pièces
    // On initialise aussi p : l'indice de début de boucle (cas nb pièce pair ou impair)
    if (n % 2 == 1) {
        p = 2;
        for (int i = 0; i < n; i++)
            T[i] = v[i];
    }
    else {
        p = 3;
        for (int i = 0; i < n-1; i++)
            T[i] = max(v[i], v[i+1]);
    }

    int k;
    for (int i = p; i < n; i += 2)
    {
        for (int j = 0; j < n-i; j++)
        {
            // k : indice permettant de bien se déplacer dans le tableau v (contenant les valeurs des pièces)
            k = i+j;

            // Calcul et affectation de la valeur optimale dans la case [j]
            T[j] = max(
                min(
                    v[j] + T[j+1],
                    v[j] + T[j+2]
                ),
                min(
                    v[k] + T[j],
                    v[k] + T[j+1]
                )
            );
        }   
    }

    // print_array(T, n);
    return T[0];
}

/// @brief Lis un fichier de tests, calcul les résultats des différents tests et les écrits dans le fichier spécifié
///         Mesure et affiche le temps d'éxécution sur le terminal
/// @param filepath Le chemin relatif vers le fichier d'entrée
/// @param output_path Le chemin relatif vers le fichier de sortie
void read_input_file(char *filepath, char *output_path)
{
    FILE *input_file = fopen(filepath, "r");
    FILE *output_file = fopen(output_path, "w");

    // Récupération du nombre de tests
    int nb_test;
    fscanf(input_file, "%d", &nb_test);

    // Mesure de l'heure de commencement des calculs
    clock_t start, end;
    start = clock();

    // Itération des 
    for (int i = 0; i < nb_test; i++)
    {
        // On alloue la mémoire pour le tableau contenant les valeurs des pièces du ième test
        int tab_size;
        fscanf(input_file, "%d", &tab_size);
        int *my_tab = malloc(tab_size * sizeof(int));

        // Initialisation des éléments du tableau de valeurs
        for (int j = 0; j < tab_size; j++)
        {
            fscanf(input_file, "%d", &my_tab[j]);
        }

        // Calcul du gain garantie pour le ième test
        int tmp = burning_coins_1d(my_tab, tab_size);

        // Ecriture du résultat dans le fichier de sortie
        fprintf(output_file, "%d\n", tmp);

        // On libère l'espace allouer pour le tableau de valeurs
        free(my_tab);
    }

    // Mesure de l'heure à la fin des calculs
    end = clock();
    // Calcul du temps d'execution
    double temps = ((double) (end-start)) / CLOCKS_PER_SEC;
    printf("Le calcul a pris %lf secondes\n", temps);

    // On ferme les fichiers utilisés
    fclose(input_file);
    fclose(output_file);
}

 
int main(int argc, char **argv)
{
    read_input_file(argv[1], argv[2]);

    return 0;
}