#include <stdio.h>
#include <string.h>
#include "board.h"

// Fonction pour récupérer les noms des joueurs
void get_player_names(char *player1_name, char *player2_name)
{
    printf("Entrez le nom du premier joueur (Nord) : ");
    fgets(player1_name, 100, stdin); // Lecture du nom du joueur 1
    player1_name[strcspn(player1_name, "\n")] = '\0'; // Suppression du caractère de nouvelle ligne

    printf("Entrez le nom du second joueur (Sud) : ");
    fgets(player2_name, 100, stdin); // Lecture du nom du joueur 2
    player2_name[strcspn(player2_name, "\n")] = '\0'; // Suppression du caractère de nouvelle ligne
}

// Affiche le joueur dont c'est le tour
void current_turn(board game, const char *player1_name, const char *player2_name)
{
    if (current_player(game) == 1)
    {
        printf("C'est au tour de %s : \n", player1_name);
    } else {
        printf("C'est au tour de %s : \n", player2_name);
    }
}

// Affiche le gagnant à la fin de la partie
void who_won(board game, const char *player1_name, const char *player2_name)
{
    if (current_player(game) == 1)
    {
        printf("%s WINS\n", player2_name); // Si le joueur 1 a perdu, le joueur 2 gagne
    } else
    {
        printf("%s WINS\n", player1_name); // Sinon, le joueur 1 gagne
    }
}

// Affiche le guide pour les mouvements possibles
void print_moves_carre()
{
    printf("                 ---------------\n");
    printf("                 | 5 |  6  | 7 |\n");
    printf("                 ---------------\n");
    printf("                 | 3 |  *  | 4 |\n");
    printf("                 ---------------\n");
    printf("                 | 0 |  1  | 2 |\n");
    printf("                 ---------------\n");
    printf("               Choisissez un mouvement\n");
}

// Affiche l'état actuel du plateau de jeu
void afiche_plateau(board game, int is_hex)
{
        printf("   ");
        for (int a = 0; a < NB_COLS; a++)
        {
            printf(" %4d ", a + 1); // Affiche les indices des colonnes
        }
        printf("\n");

        printf("   +");
        for (int a = 0; a < NB_COLS; a++)
        {
            printf("------"); // Séparateurs entre les cases
        }
        printf("+\n");

        for (int i = 0; i < NB_LINES; i++)
        {
            printf(" %2d| ", i + 1); // Affiche les indices des lignes

            for (int j = 0; j < NB_COLS; j++) 
            {
                char c = ' ';
                // Détermine le contenu de chaque case
                switch (get_content(game, i, j))
                {
                    case EMPTY: c = ' '; break;
                    case NORTH_KING: c = 'N'; break;
                    case SOUTH_KING: c = 'S'; break;
                    case KILLED: c = 'X'; break;
                    default: c = ' '; break;
                }
                printf("  %c  |", c); // Affiche le contenu de la case
            }
            printf("\n");

            printf("   +");
            for (int a = 0; a < NB_COLS; a++)
            {
                printf("------");
            }
            printf("+\n");
        }
}

// Fonction pour obtenir un entier valide en entrée
int get_valid_int_input()
{
    int input;
    while (1) {
        if (scanf("%d", &input) == 1) {
            return input; // Retourne l'entrée valide
        } else {
            while (getchar() != '\n'); // Vide le buffer en cas d'entrée invalide
            printf("Input invalide. Veuillez entrer un nombre entier : ");
        }
    }
}

int main(int args, char **argv) {
    board game; // Plateau de jeu
    char player1_name[100], player2_name[100];
    get_player_names(player1_name, player2_name); // Récupère les noms des joueurs
    
    // Configuration des variations de jeu
    printf("Activer la variation portée? (0 = Non, 1 = Oui)\n");
    if (get_valid_int_input() == 1)
    {
        game = new_special_game(0, 1);
    }
    else{
        game = new_game();
    }

    int next_step = 0; // Indique si on peut passer a l'étape suivante
    while (get_winner(game) == NO_PLAYER) { // Boucle principale tant qu'il n'y a pas de gagnant
        while (next_step != 1) { // Etape de déplacement
            afiche_plateau(game, is_hex(game)); // Affiche le plateau
            print_moves_carre(); // Affiche les mouvements possibles
            current_turn(game, player1_name, player2_name); // Affiche le joueur actif
            int move = get_valid_int_input(); // Récupère le mouvement
            enum return_code result = move_toward(game, move); // Effectue le déplacement
            if (result == OK)
            {
                printf("Déplacement réussi.\n");
                next_step = 1;
            } else if (result == OUT)
            {
                printf("Cette case n'existe plus ou pas\n");
            } else if (result == BUSY)
            {
                printf("Cette case est déjà occupée par l'autre joueur\n");
            } else if (result == RULES)
            {
                printf("Cette case n'existe plus ou pas\n");
            }
        }

        next_step = 0; // Réinitialise pour le tour suivant
        while (next_step != 1)
        { //Etape de supression de cellule
            afiche_plateau(game, is_hex(game)); // Affiche le plateau
            current_turn(game, player1_name, player2_name); // Affiche le joueur actif
            printf("Colonne de la case à tuer : ");
            int col = get_valid_int_input(); // Récupère la colonne
            printf("Ligne de la case à tuer : ");
            int row = get_valid_int_input(); // Récupère la ligne
            enum return_code result2 = kill_cell(game, row - 1, col - 1); // Effectue la destruction
            if (result2 == OK)
            {
                printf("Case tuée\n");
                next_step = 1;
            } else if (result2 == OUT)
            {
                printf("Cette case n'existe pas ou est déjà morte\n");
            } else if (result2 == BUSY)
            {
                printf("Cette case est occupée par quelqu'un\n");
            } else if (result2 == RULES)
            {
                printf("Cette case est à plus de 3 cases de toi\n");
            }
        }
        next_step = 0;
    }

    who_won(game, player1_name, player2_name); // Affiche le gagnant
    destroy_game(game); // Libère la mémoire du jeu
    printf("Suppression du plateau et sortie\n");
    return 0;
}