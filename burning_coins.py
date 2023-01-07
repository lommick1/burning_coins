import numpy as np
import sys
import time

# Solution recursive, cout exponentiel
# Paramètres :
#   i : borne inférieure
#   j : borne supérieure
#   v : tableau contenant les valeurs des pièces
# Appel initial : coins_rec(0, n-1, v)
def coins_rec(i, j, v):
    if (i > j):
        return 0

    # cas où il ne reste que la ième pièce => le choix est vite fait
    # arrive quand il y a un nombre pair de pièce
    if (i == j):
        return v[i]

    # cas où il ne reste que 2 pièces => on prend la plus elevée
    # arrive quand il y a un nombre impair de pièce
    if (i == j-1):
        return max(v[i], v[j])
    
    # cas général, 4 possibilités :
    #   - si on prend la pièce à l'extremité gauche (ième):
    #       - l'adversaire prend ensuite celle à gauche ((i+1)ème)
    #       - l'adversaire prend ensuite celle à droite (jème)
    #   - si on prend la pièce à l'extremité droite (jème):
    #       - l'adversaire prend ensuite celle à gauche (ième)
    #       - l'adversaire prend ensuite celle à droite ((j-1)ème)
    # 
    return max(
        min(
            v[i] + coins_rec(i+2, j, v),
            v[i] + coins_rec(i+1, j-1, v)
        ),
        min(
            v[j] + coins_rec(i, j-2, v),
            v[j] + coins_rec(i+1, j-1, v)
        )
    )


def coins(v):
    # Déclaration d'un tableau 2 dimensions n*n pour stocker les résultats des sous problèmes
    n = len(v)
    T = np.zeros((n, n), dtype=np.int32)

    # Initialisation du tableau avec les cas d'arrets (s'il reste 1 ou 2 pièces)
    # j'ai décidé d'initialiser les diagonales pour les deux cas (nombre de pièce pair et impair)
    # meme si une d'elle ne sera pas utilisée
    # car n*n cases mémoire ont déjà été allouées et cela ne change pas l'algorithme
    for i in range(n-1):
        T[i, i] = v[i]
        T[i+1, i] = max(v[i], v[i+1])
    T[n-1,n-1] = v[n-1]

    # indice de début de boucle (cas nb pièce pair ou impair)
    p = 2 if (n % 2 == 1) else 3

    for i in range(p, n, 2):
        for j in range(0, n-i):
            k = i+j # indice qui permet de se déplacer sur la "ième diagonale"

            # calcul et affectation de la valeur optimale dans la case [k, j]
            T[k, j] = max(
                min(
                    v[k] + T[k-1, j+1],
                    v[k] + T[k-2, j]
                ),
                min(
                    v[j] + T[k-1, j+1],
                    v[j] + T[k, j+2]
                )
            )

    # print(T)
    return T[n-1, 0]


def burning_coins_1d(v):
    n = len(v)
    T = np.zeros(n, dtype=np.int32)

    # Initialisation d'un tableau de taille n selon la parité du nombre de pièces
    if (n % 2 == 1):
        for i in range(n):
            T[i] = v[i]
    else:
        for i in range(n-1):
            T[i] = max(v[i], v[i+1])

    # p : indice de début de boucle (cas nb pièce pair ou impair)
    p = 2 if (n % 2 == 1) else 3

    for i in range(p, n, 2):
        for j in range(n-i):
            # k : indice permettant de bien se déplacer dans le tableau v (contenant les valeurs des pièces)
            k = i+j

            T[j] = max(
                min(
                    v[j] + T[j+1],
                    v[j] + T[j+2]
                ),
                min(
                    v[k] + T[j],
                    v[k] + T[j+1]
                )
            )

    # print(T)
    return T[0]

def run_test(filename, output_path):
    output_file = open(output_path, "w")

    with open(filename) as f:
        inputs = f.read().splitlines()[1:][1::2]

    inputs = [list(map(int, v.split(' '))) for v in inputs]

    start = time.time()
    for v in inputs:
        my_res = burning_coins_1d(v)
        output_file.write(str(my_res) + "\n")

    end = time.time()
    temps = round(end-start, 4)
    print(f"Le calcul a pris {temps} secondes")


if __name__ == '__main__':
    run_test(sys.argv[1], sys.argv[2])
