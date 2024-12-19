#include <stdio.h>
#include <stdbool.h>
#include "board.h"

typedef struct {
    /*Definition of a structure containing the names of the 2 players*/
    char j1[30];
    char j2[30];
} names;

bool ask_value(int *value){
    /*Define of a function to avoid players putting characters instead of numbers*/
    char c;
    if (scanf("%d", value) == 0) {
        while ((c = getchar()) != '\n' && c != EOF);
        return false;
    }
    return true;
}

// Affichage
char verify_cell_content(board game, int line, int column, names players){
    /*Returns the character contained in the cell :
        - Returns the first letter of player 1's name if present in the cell
        - Returns the first letter of player 2's name if present in the cell
        - Returns the first letter 'X' if the cell is dead
        - A space is returned in all other cases
    */
    cell content = get_content(game, line, column);
    if(content == NORTH_KING){
        return players.j1[0];
    } 
    else if(content == SOUTH_KING){
        return players.j2[0];
    } 
    else if(content == KILLED){
        return 'X';
    }
    else {
        return ' ';
    }
}

void grid_line(int nb_line, int current_line, int nb_column){
    /*Displays a horizontal dividing line*/
    if (current_line == 0){
        printf("  ╔");
        for (int i = 0; i < nb_column; i++){
            if (i != nb_column - 1) printf("═─═╦");
            else printf("═─═╗");
        }
        printf("\n");
    }
    else if  (current_line == nb_line){
        printf("  ╚");
        for (int i = 0; i < nb_column; i++){
            if (i != nb_column - 1) printf("═─═╩");
            else printf("═─═╝");
        }
        printf("\n");
    }
    else {
        printf("  ╠");
        for (int i = 0; i < nb_column; i++){
            if (i != nb_column - 1) printf("───╬");
            else printf("───╣");
        }
        printf("\n");
    }
}

void line_content(board game, int current_line, int nb_column, names players){
    /*Displays vertical dividing lines and cell contents*/
    printf("%d │", current_line + 1);
    for (int c = 0; c < nb_column; c++){
        printf(" ");
        printf("\033[1m%c\033[0m │", verify_cell_content(game, current_line, c, players));
    }
    printf("\n");
}

void grid(board game, int nb_line, int nb_column, names players){
    /*Displays the entire board with cell contents*/
    printf("\033[2J");
    if (current_player(game) == NORTH) printf("         \033[4mTour de %s\033[0m\n", players.j1);
    else if (current_player(game) == SOUTH) printf("         \033[4mTour de %s\033[0m\n", players.j2);
    printf("   ");
    for (int i = 1; i <= nb_column; i++){
        printf(" %d  ", i);
    }
    printf("\n");
    grid_line(nb_line, 0, nb_column);
    for (int i = 1; i <= nb_line; i++){
        line_content(game, i - 1, nb_column, players);
        grid_line(nb_line, i, nb_column);
    }
}


// Deplacement
void can_move(board game){
    /*Checks available boxes and displays only those that are available to the user*/
    int current_line;
    int current_column;
    for (int i = 0; i < NB_LINES; i++){
        for (int j = 0; j < NB_COLS; j++){
            if ((get_content(game, i, j) == NORTH_KING && current_player(game) == NORTH) || (get_content(game, i, j) == SOUTH_KING && current_player(game) == SOUTH)){
                current_line = i;
                current_column = j;
            } 
        }
    }

    if (get_content(game, current_line-1, current_column-1) == EMPTY && current_line-1 >= 0 && current_column-1 >= 0) printf("            \033[1;93m6\033[0m");
    else printf("            \033[1;33m×\033[0m");

    if (get_content(game, current_line-1, current_column) == EMPTY && current_line-1 >= 0) printf(" \033[1;93m7\033[0m ");
    else printf(" \033[1;33m×\033[0m ");

    if (get_content(game, current_line-1, current_column+1) == EMPTY && current_line-1 >= 0 && current_column+1 < NB_COLS) printf("\033[1;93m8\033[0m\n");
    else printf("\033[1;33m×\033[0m\n");

    if (get_content(game, current_line, current_column-1) == EMPTY && current_column-1 >= 0) printf("            \033[1;93m4\033[0m   ");
    else printf("            \033[1;33m×\033[0m   ");

    if (get_content(game, current_line, current_column+1) == EMPTY && current_column+1 < NB_COLS) printf("\033[1;93m5\033[0m\n");
    else printf("\033[1;33m×\033[0m\n");

    if (get_content(game, current_line+1, current_column-1) == EMPTY && current_line+1 < NB_LINES && current_column-1 >= 0) printf("            \033[1;93m1\033[0m");
    else printf("            \033[1;33m×\033[0m");

    if (get_content(game, current_line+1, current_column) == EMPTY && current_line+1 < NB_LINES) printf(" \033[1;93m2\033[0m ");
    else printf(" \033[1;33m×\033[0m ");

    if (get_content(game, current_line+1, current_column+1) == EMPTY && current_line+1 < NB_LINES && current_column+1 < NB_COLS) printf("\033[1;93m3\033[0m\n");
    else printf("\033[1;33m×\033[0m\n");

}

void choice_move(board game, names players){
    /*Ask the player in which direction he wants to moves*/
    int choice;
    enum return_code state = 1;
    grid(game, NB_LINES, NB_COLS, players);
    printf("\n");
    //printf("C'est au tour de %s de jouer.", player_name);
    while (state == 1){
        printf("\033[1;3;4;36m ✲ Veuillez choisir une direction dans laquelle vous déplacer :\033[0m\n\n");
        can_move(game);
        ask_value(&choice);
        direction dir = choice-1;
        if (move_toward(game, dir) == OK){
            state = 0;
        }
    }    
}

// Kill cell
void choice_kill(board game, names players){
    /*Ask the player which cell he wants to kill*/
    int column;
    int line;
    enum return_code state = 1;
    grid(game, NB_LINES, NB_COLS, players);
    //printf("C'est au tour de %s de jouer.", player_name);
    while (state == 1){
        printf("\033[1;3;4;36m ✲ Veuillez choisir la cellule a éliminer\033[0m\n");
        printf("    \033[1;3;31m ✲ Ligne : \033[0m");
        ask_value(&line);
        printf("    \033[1;3;31m ✲ Colonne : \033[0m");
        ask_value(&column);
        if (kill_cell(game, line - 1, column - 1) == OK){
            state = 0;
        }
    }    
}


// End game
player winner(board game, names players){
    /*Displays and returns the name of the player who won when the second player can't move.*/
    player win = get_winner(game);
    if(win == NORTH){
        printf("Le joueur %s a gagné la partie !\n", players.j1);
        return NORTH;
    }
    else if(win == SOUTH){
        printf("Le joueur %s a gagné la partie !\n", players.j2);
        return SOUTH;
    }
    else {
        return NO_PLAYER;
    }
}


player tour(board game, names players){
    /*Allows you to cycle the rounds in the game*/
    choice_move(game, players);
    choice_kill(game, players);
    return winner(game, players);
}

void Jeu(){
    /*Request player names 
    creates the board
    Takes turns until there's a winner*/
    names players;
    printf("Entrez le nom du joueur 1 : ");
    scanf("%s", players.j1);
    printf("Entrez le nom du joueur 2 : ");
    scanf("%s", players.j2);
    board game = new_game();
    player is_winner = NO_PLAYER;
    while (is_winner == NO_PLAYER){
        is_winner = tour(game, players);
    }
}



int main(){
    Jeu();
    return 0;
}