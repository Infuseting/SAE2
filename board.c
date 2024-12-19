#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "board.h"

typedef enum actual_turn_e {
    MOVE,
    KILL
} actual_turn;  


struct board_s {
    cell cells[MAX_DIMENSION][MAX_DIMENSION];
    player current;
    bool is_hex;
    bool use_range;
    actual_turn turn;
};

int* find_king_board(board game, player player) {
    static int coordinates[2];
    for (int i = 0; i < MAX_DIMENSION; i++) {
        for (int j = 0; j < MAX_DIMENSION; j++) {
            if ((player == NORTH && game->cells[i][j] == NORTH_KING) ||
                (player == SOUTH && game->cells[i][j] == SOUTH_KING)) {
                coordinates[0] = i;
                coordinates[1] = j;
                return coordinates;
            }
        }
    }
    return NULL; // King not found
    

}

int distance(board game, int x1, int y1, int x2, int y2)
{
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    int distance = (dx > dy) ? dx : dy;
    return distance;
}
board new_game() {
    board game = malloc(sizeof(struct board_s));
    game->current = NORTH;
    game->is_hex = false;
    game->use_range = false;
    game->turn = MOVE;
    for (int i = 0; i < MAX_DIMENSION; i++) {
        for (int j = 0; j < MAX_DIMENSION; j++) {
            if (i > NB_LINES - 1 || j > NB_COLS - 1) {
                game->cells[i][j] = KILLED;
            }
            else {
                game->cells[i][j] = EMPTY;

            }
        }
    }
    
    game->cells[0][3] = NORTH_KING;
    game->cells[NB_LINES-1][3] = SOUTH_KING;
    return game;
}

board new_special_game(bool is_hex, bool use_range) {
    board game = malloc(sizeof(struct board_s));
    game->current = NORTH;
    game->is_hex = is_hex;
    game->use_range = use_range;
    game->turn = MOVE;
    if (is_hex) {
        for (int i = 0; i < MAX_DIMENSION; i++) {
            for (int j = 0; j < MAX_DIMENSION; j++) {
                game->cells[i][j] = EMPTY;
            }
        }
        for (int i = 0 ; i < HEX_SIDE; i++) {
            for (int j = i; j < HEX_SIDE - 1; j++) {
                game->cells[i][j + HEX_SIDE] = KILLED;
            }
        }
        for (int i = -1; i < HEX_SIDE - 1; i++) {
            
            for (int j = 0; j < i + 1; j++) {
                game->cells[HEX_SIDE + i][j] = KILLED;
            }
            
        }
        game->cells[0][2] = NORTH_KING;
        game->cells[MAX_DIMENSION-1][6] = SOUTH_KING;
    }
    else {
        game = new_game();
        game->use_range = use_range;
    }
    
    return game;
}

board copy_game(board original_game) {
    board new_game_copy = malloc(sizeof(struct board_s));
    new_game_copy->current = original_game->current;
    new_game_copy->is_hex = original_game->is_hex;
    new_game_copy->use_range = original_game->use_range;
    new_game_copy->turn = original_game->turn;
    for (int i = 0; i < MAX_DIMENSION; i++) {
        for (int j = 0; j < MAX_DIMENSION; j++) {
            new_game_copy->cells[i][j] = original_game->cells[i][j];
        }
    }
    return new_game_copy;
}

void destroy_game(board game) {
    free(game);
}

bool is_hex(board game) {
    return game->is_hex;
}

bool uses_range(board game) {
    return game->use_range;
}

player current_player(board game) {
    return game->current;
}

cell get_content(board game, int line, int column) {
    if (line < 0 || line >= MAX_DIMENSION || column < 0 || column >= MAX_DIMENSION) {
        return KILLED;
    }
    return game->cells[line][column];
}

player get_winner(board game) {
    bool continueGame = true;
    if (game->is_hex) {
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                int* tab = find_king_board(game, game->current);
                if (tab[0] + i < 0 || tab[0] + i >= MAX_DIMENSION || tab[1] + j < 0 || tab[1] + j >= MAX_DIMENSION) {
                    printf("Case hors plateau %d %d\n", tab[0] + i, tab[1] + j);
                    continue;
                }
                if (game->cells[tab[0] + i][tab[1] + j] == EMPTY && !(i == 0 && j == 0) && !(i == -1 && j == 1) && !(i == 1 && j == -1)) {
                    printf("Case vide %d %d\n", tab[0] + i, tab[1] + j);
                    continueGame = false;
                }
            }
        }
        /*
        case NW: line = -1; column = -1; break;
            case NE: line = -1; column = 0; break;
            case W: line = 0; column = -1; break;
            case E: line = 0; column = 1; break;
            case SW: line = 1; column = 0; break;
            case SE: line = 1; column = 1; break;
            */
    }
    else {
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                int* tab = find_king_board(game, game->current);
                if (tab[0] + i < 0 || tab[0] + i >= NB_LINES || tab[1] + j < 0 || tab[1] + j >= NB_COLS) {
                    continue;
                }
                if (game->cells[tab[0] + i][tab[i] + j] == EMPTY ) {
                    printf("Case vide %d %d\n", tab[0] + i, tab[1] + j);
                    continueGame = false;
                }
            }
        }
    }
    
    if (continueGame) {
        game->current = (game->current == NORTH) ? SOUTH : NORTH;
        return game->current == NORTH ? SOUTH : NORTH;
    }
    else {
        return NO_PLAYER;
    }
}


enum return_code move_toward(board game, direction d) {
    if (game->turn == KILL) {
        return RULES;
    }
    int line = 0, column = 0;
    
    if (game->is_hex) {
        switch (d) {
            case NW: line = -1; column = -1; break;
            case NE: line = -1; column = 0; break;
            case N: return RULES;
            case W: line = 0; column = -1; break;
            case E: line = 0; column = 1; break;
            case SW: line = 1; column = 0; break;
            case S: return RULES;
            case SE: line = 1; column = 1; break;
            default: break;
        }
    }
    else {
        switch (d) {
            case NW: line = -1; column = -1; break;
            case N: line = -1; column = 0; break;
            case NE: line = -1; column = 1; break;
            case W: line = 0; column = -1; break;
            case E: line = 0; column = 1; break;
            case SW: line = 1; column = -1; break;
            case S: line = 1; column = 0; break;
            case SE: line = 1; column = 1; break;
            default: break;
        }
    }
    int* tab = find_king_board(game, game->current);
    if (game->is_hex) {
        if (tab[0] + line < 0 || tab[0] + line >= MAX_DIMENSION || tab[1] + column < 0 || tab[1] + column >= MAX_DIMENSION) {
            return OUT;
        }
    }
    else {
        if (tab[0] + line < 0 || tab[0] + line >= NB_LINES || tab[1] + column < 0 || tab[1] + column >= NB_COLS) {
            return OUT;
        }
    }
    if (game->cells[tab[0] + line][tab[1] + column] == EMPTY) {
        game->cells[tab[0] + line][tab[1] + column] = game->cells[tab[0]][tab[1]];
        game->cells[tab[0]][tab[1]] = EMPTY;
        game->turn = KILL;
        return OK;
    }
    if (game->cells[tab[0] + line][tab[1] + column] == NORTH_KING || game->cells[tab[0] + line][tab[1] + column] == SOUTH_KING) {
        return BUSY;
    }
    return BUSY;
}

enum return_code kill_cell(board game, int line, int column) {
    if (game->turn == MOVE) {
        return RULES;
    }
    int* tab = find_king_board(game, game->current);
    if (game->is_hex) {
        if (line < 0 || line >= MAX_DIMENSION || column < 0 || column >= MAX_DIMENSION) {
            return OUT;
        }
    }
    else {
        if (line < 0 || line >= NB_LINES || column < 0 || column >= NB_COLS) {
            return OUT;
        }
    }
    if (game->cells[line][column] == KILLED) {
        return OUT;
    }
    if (( game->cells[line][column] == NORTH_KING) ||
        ( game->cells[line][column] == SOUTH_KING)) {
        return BUSY;
    }
    if (game->use_range && distance(game, tab[0], tab[1], line, column) > 3) {
        return RULES;
    }
    game->cells[line][column] = KILLED;
    game->current = (game->current == NORTH) ? SOUTH : NORTH;
    game->turn = MOVE;
    return OK;
}