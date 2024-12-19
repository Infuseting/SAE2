#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "board.h"


struct board_s {
    cell cells[MAX_DIMENSION][MAX_DIMENSION];
    player current;
    bool is_hex;
    bool use_range;
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
    if (game->is_hex) {
        return (abs(x1 - x2) + abs(y1 - y2) + abs(x1 + y1 - x2 - y2)) / 2;
    }
    return abs(x1 - x2) + abs(y1 - y2);
}
board new_game() {
    board game = malloc(sizeof(struct board_s));
    game->current = NORTH;
    game->is_hex = false;
    game->use_range = false;
    for (int i = 0; i < MAX_DIMENSION; i++) {
        for (int j = 0; j < MAX_DIMENSION; j++) {
            game->cells[i][j] = EMPTY;
        }
    }
    for (int i = NB_LINES; i < MAX_DIMENSION; i++) {
        for (int j = NB_COLS; j < MAX_DIMENSION; j++) {
            game->cells[i][j] = KILLED;
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
    return game->cells[line][column];
}

player get_winner(board game) {
    bool win = false;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int* tab = find_king_board(game, game->current);
            if (tab[0] + i < 0 || tab[0] + i >= MAX_DIMENSION || tab[1] + j < 0 || tab[1] + j >= MAX_DIMENSION) {
                continue;
            }
            if (game->cells[tab[0] + i][tab[i] + j] == EMPTY ) {
                win = true;
            }
        }
    }
    if (!win) {
        return game->current;
    }
    else {
        return NO_PLAYER;
    }
}


enum return_code move_toward(board game, direction d) {
    int line = 0, column = 0;
    if (game->is_hex) {
        switch (d) {
            case NW: line = -1; column = -1; break;
            case NE: line = -1; column = 0; break;
            case W: line = 0; column = -1; break;
            case E: line = 0; column = 1; break;
            case SW: line = 1; column = 0; break;
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
        return OK;
    }
    if (line == 0 && column == 0) {
        return RULES;
    }
    return BUSY;
}

enum return_code kill_cell(board game, int line, int column) {
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
        return BUSY;
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
    return OK;
}