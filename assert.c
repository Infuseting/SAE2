#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include <time.h>

typedef enum actual_turn_e
{
  MOVE,
  KILL
} actual_turn;

struct board_s
{
  cell cells[MAX_DIMENSION][MAX_DIMENSION];
  player current;
  bool is_hex;
  bool use_range;
  actual_turn turn;
};

int *find_king_board_assert(board game, player player)
{
  static int coordinates[2];
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      if ((player == NORTH && game->cells[i][j] == NORTH_KING) ||
          (player == SOUTH && game->cells[i][j] == SOUTH_KING))
      {
        coordinates[0] = i;
        coordinates[1] = j;
        return coordinates;
      }
    }
  }
  return NULL; // King not found
}

int distance_assert(board game, int x1, int y1, int x2, int y2)
{
  int tab[MAX_DIMENSION][MAX_DIMENSION] = {0};

  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      if (get_content(game, i, j) == KILLED)
      {
        tab[i][j] = -1;
      }
      else
      {
        tab[i][j] = 10000;
      }
    }
  }
  tab[x1][y1] = 0;
  int value = 0;
  bool evolution = true;
  while (evolution)
  {
    int coordinates[100][2];
    int count = 0;
    evolution = false;
    for (int i = 0; i < MAX_DIMENSION; i++)
    {
      for (int j = 0; j < MAX_DIMENSION; j++)
      {
        if (tab[i][j] == value)
        {
          for (int p = -1; p < 2; p++)
          {
            for (int q = -1; q < 2; q++)
            {
              if (i + p >= 0 && i + p < MAX_DIMENSION && j + q >= 0 && j + q < MAX_DIMENSION)
              {
                if (!(game->is_hex && ((p == -1 && q == 1) || (p == 1 && q == -1))))
                {
                  if (tab[i + p][j + q] > value)
                  {
                    coordinates[count][0] = i + p;
                    coordinates[count][1] = j + q;
                    count++;
                  }
                }
              }
            }
          }
        }
      }
    }
    for (int i = 0; i < count; i++)
    {
      if (tab[coordinates[i][0]][coordinates[i][1]] > value)
      {
        tab[coordinates[i][0]][coordinates[i][1]] = value + 1;
        evolution = true;
      }
    }

    value++;
  }
  // printf("\n\n\n");
  // affiche_tab(tab);
  return tab[x2][y2];
}
board new_game_assert()
{
  board game = malloc(sizeof(struct board_s));
  game->current = NORTH;
  game->is_hex = false;
  game->use_range = false;
  game->turn = MOVE;
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      if (i > NB_LINES - 1 || j > NB_COLS - 1)
      {
        game->cells[i][j] = KILLED;
      }
      else
      {
        game->cells[i][j] = EMPTY;
      }
    }
  }

  game->cells[0][NB_COLS / 2] = NORTH_KING;
  game->cells[NB_LINES - 1][NB_COLS / 2] = SOUTH_KING;
  return game;
}

board new_special_game_assert(bool is_hex, bool use_range)
{
  board game = malloc(sizeof(struct board_s));
  game->current = NORTH;
  game->is_hex = is_hex;
  game->use_range = use_range;
  game->turn = MOVE;
  if (is_hex)
  {
    for (int i = 0; i < MAX_DIMENSION; i++)
    {
      for (int j = 0; j < MAX_DIMENSION; j++)
      {
        game->cells[i][j] = EMPTY;
      }
    }
    for (int i = 0; i < HEX_SIDE; i++)
    {
      for (int j = i; j < HEX_SIDE - 1; j++)
      {
        game->cells[i][j + HEX_SIDE] = KILLED;
      }
    }
    for (int i = -1; i < HEX_SIDE - 1; i++)
    {

      for (int j = 0; j < i + 1; j++)
      {
        game->cells[HEX_SIDE + i][j] = KILLED;
      }
    }
    game->cells[0][HEX_SIDE / 2] = NORTH_KING;
    game->cells[MAX_DIMENSION - 1][(HEX_SIDE - 1) + (HEX_SIDE / 2)] = SOUTH_KING;
  }
  else
  {
    game = new_game();
    game->use_range = use_range;
  }

  return game;
}

board copy_game_assert(board original_game)
{
  board new_game_copy = malloc(sizeof(struct board_s));
  if (new_game_copy == NULL)
  {
    perror("Failed to allocate memory for game copy");
    exit(EXIT_FAILURE);
  }
  new_game_copy->current = original_game->current;
  new_game_copy->is_hex = original_game->is_hex;
  new_game_copy->use_range = original_game->use_range;
  new_game_copy->turn = original_game->turn;
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      new_game_copy->cells[i][j] = original_game->cells[i][j];
    }
  }
  return new_game_copy;
}

void destroy_game_assert(board game)
{
  free(game);
}

bool is_hex_assert(board game)
{
  return game->is_hex;
}

bool uses_range_assert(board game)
{
  return game->use_range;
}

player current_player_assert(board game)
{
  return game->current;
}

cell get_content_assert(board game, int line, int column)
{
  if (line < 0 || line >= MAX_DIMENSION || column < 0 || column >= MAX_DIMENSION)
  {
    return KILLED;
  }
  return game->cells[line][column];
}

player get_winner_assert(board game)
{
  if (game->is_hex)
  {
    for (int i = -1; i < 2; i++)
    {
      for (int j = -1; j < 2; j++)
      {
        int *tab = find_king_board_assert(game, game->current);
        if (tab[0] + i < 0 || tab[0] + i >= MAX_DIMENSION || tab[1] + j < 0 || tab[1] + j >= MAX_DIMENSION)
        {
          continue;
        }
        if (game->cells[tab[0] + i][tab[1] + j] == EMPTY && !(i == 0 && j == 0) && !(i == -1 && j == 1) && !(i == 1 && j == -1))
        {
          return NO_PLAYER;
        }
      }
    }
  }
  else
  {
    for (int i = -1; i < 2; i++)
    {
      for (int j = -1; j < 2; j++)
      {
        int *tab = find_king_board_assert(game, current_player(game));
        if (tab[0] + i < 0 || tab[0] + i >= NB_LINES || tab[1] + j < 0 || tab[1] + j >= NB_COLS)
        {
          continue;
        }
        if (game->cells[tab[0] + i][tab[1] + j] == EMPTY)
        {
          return NO_PLAYER;
        }
      }
    }
  }
  game->current = (game->current == NORTH) ? SOUTH : NORTH;
  return game->current == NORTH ? NORTH : SOUTH;
}
enum return_code move_toward_assert(board game, direction d)
{
  if (game->turn == KILL)
    return RULES;
  int line = 0, column = 0;
  switch (d)
  {
  case NW:
    line = -1;
    column = -1;
    break;
  case NE:
    line = -1;
    column = game->is_hex ? 0 : 1;
    break;
  case N:
    if (!game->is_hex)
      line = -1;
    else
      return RULES;
    break;
  case W:
    column = -1;
    break;
  case E:
    column = 1;
    break;
  case SW:
    line = 1;
    column = game->is_hex ? 0 : -1;
    break;
  case S:
    if (!game->is_hex)
      line = 1;
    else
      return RULES;
    break;
  case SE:
    line = 1;
    column = 1;
    break;
  default:
    break;
  }
  int *tab = find_king_board_assert(game, game->current);
  int new_line = tab[0] + line, new_col = tab[1] + column;
  if (new_line < 0 || new_line >= (game->is_hex ? MAX_DIMENSION : NB_LINES) || new_col < 0 || new_col >= (game->is_hex ? MAX_DIMENSION : NB_COLS) || game->cells[new_line][new_col] == KILLED)
    return OUT;
  if (game->cells[new_line][new_col] != EMPTY)
    return BUSY;
  game->cells[new_line][new_col] = game->cells[tab[0]][tab[1]];
  game->cells[tab[0]][tab[1]] = EMPTY;
  game->turn = KILL;
  return OK;
}

enum return_code kill_cell_assert(board game, int line, int column)
{
  if (game->turn == MOVE)
  {
    return RULES;
  }
  int *tab = find_king_board_assert(game, game->current);
  if (game->is_hex)
  {
    if (line < 0 || line >= MAX_DIMENSION || column < 0 || column >= MAX_DIMENSION)
    {
      return OUT;
    }
  }
  else
  {
    if (line < 0 || line >= NB_LINES || column < 0 || column >= NB_COLS)
    {
      return OUT;
    }
  }
  if (game->cells[line][column] == KILLED)
  {
    return OUT;
  }
  if ((game->cells[line][column] == NORTH_KING) ||
      (game->cells[line][column] == SOUTH_KING))
  {
    return BUSY;
  }
  if (game->use_range && distance_assert(game, tab[0], tab[1], line, column) > KING_RANGE)
  {
    return RULES;
  }
  game->cells[line][column] = KILLED;
  game->current = (game->current == NORTH) ? SOUTH : NORTH;
  game->turn = MOVE;
  return OK;
}

void bot_move(board game)
{
  int directions[] = {NW, NE, N, W, E, SW, S, SE};
  int num_directions = sizeof(directions) / sizeof(directions[0]);
  enum return_code result_assert, result;

  int random_direction = directions[0];
  do
  {
    random_direction = directions[rand() % num_directions];
    board copy_games = copy_game_assert(game);
    board copy_games_2 = copy_game_assert(game);
    result_assert = move_toward_assert(copy_games, random_direction);
    result = move_toward(copy_games_2, random_direction);
    assert(result == result_assert);
  } while (result != OK);
  move_toward_assert(game, random_direction);
}

void bot_kill(board game)
{
  enum return_code result, result_assert;
  int random_line = 0;
  int random_column = 0;
  do
  {
    random_column = rand() % MAX_DIMENSION;
    random_line = rand() % MAX_DIMENSION;
    board copy_games = copy_game_assert(game);
    board copy_games_2 = copy_game(game);
    result_assert = kill_cell_assert(copy_games, random_line, random_column);
    result = kill_cell(copy_games_2, random_line, random_column);
    assert(result == result_assert);
  } while (result != OK);
  kill_cell_assert(game, random_line, random_column);
}

bool is_memory_usage_above_1GB()
{
  FILE *file = fopen("/proc/self/status", "r");
  if (file == NULL)
  {
    perror("Cet partie n'est executable seulement sur Linux !");
    return false;
  }

  char line[256];
  long memory_kb = 0; // Mémoire en kilooctets
  while (fgets(line, sizeof(line), file))
  {
    if (strncmp(line, "VmRSS:", 6) == 0)
    {
      sscanf(line + 6, "%ld", &memory_kb); // Récupère la valeur de VmRSS en kilooctets
      break;
    }
  }

  fclose(file);

  // Convertir en octets (1 Go = 1 * 1024 * 1024 Ko) et comparer
  return memory_kb > (1L * 1024 * 1024);
}

void draw_line(int i, int j)
{
  if (j == 0)
    printf("  ");
  if (i == 0 && j == 0)
    printf("\e[34m+\e[0m");
  else if (i == NB_LINES * 2 && j == 0)
    printf("\e[34m+\e[0m");
  else if (i == 0 && j == NB_COLS * 2)
    printf("\e[34m+\e[0m");
  else if (i == NB_LINES * 2 && j == NB_COLS * 2)
    printf("\e[34m+\e[0m");
  else if (i == NB_LINES * 2)
    printf("\e[34m+\e[0m");
  else if (j == NB_COLS * 2)
    printf("\e[34m+\e[0m");
  else if (i == 0)
    printf("\e[34m+\e[0m");
  else if (j == 0)
    printf("\e[34m+\e[0m");
  else
    printf("\e[34m+\e[0m");
}

/**
 * @brief Dessine le contenu d'une case du plateau de jeu.
 *
 * Cette fonction dessine le contenu d'une case du plateau de jeu.
 *
 * @param i La ligne de la case.
 * @param j La colonne de la case.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 *
 * @author Arthur
 */
void draw_content(int i, int j, board game, char *pseudoN, char *pseudoS)
{
  switch (get_content(game, i, j))
  {
  case EMPTY:
    printf(" ");
    break;
  case KILLED:
    printf("\e[31m#\e[0m");
    break;
  case NORTH_KING:
    printf("\e[35m%c\e[0m", pseudoN == NULL ? 'N' : pseudoN[0]);
    break;
  case SOUTH_KING:
    printf("\e[33m%c\e[0m", pseudoS == NULL ? 'S' : pseudoS[0]);
    break;
  default:
    printf("\e[31mE\e[0m");
    break;
  }
}
/**
 * @brief Dessine une case du plateau de jeu.
 *
 * Cette fonction dessine une case du plateau de jeu.
 *
 * @param i La ligne de la case.
 * @param j La colonne de la case.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 */
void draw_box(int i, int j, board game, char *pseudoN, char *pseudoS)
{
  if (i % 2 == 0)
  {
    if (j % 2 == 0)
    {
      draw_line(i, j);
    }
    else
      printf("\e[34m-\e[0m");
  }
  else if (i % 2 == 1)
  {
    if (j % 2 == 0)
    {
      if (j == 0)
        printf("%d ", i / 2);
      printf("\e[34m|\e[0m");
    }
    else
    {
      draw_content(i / 2, j / 2, game, pseudoN, pseudoS);
    }
  }
}

/**
 * @brief Affiche la tête du plateau de jeu.
 *
 * Cette fonction affiche la tête du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 *
 * @author Arthur
 */
void head_line(int n, int offset)
{

  for (int i = 0; i < offset + 1; i++)
    printf(" ");
  for (int i = 0; i < n; i++)
    printf("\e[36m/ \\\e[0m ");
  printf("\n");
}
/**
 * @brief Affiche la tête du plateau de jeu.
 *
 * Cette fonction affiche la tête du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 * @param x La colonne du plateau de jeu.
 *
 * @author Arthur
 */
void body_line(int n, int offset, board game, char *pseudoN, char *pseudoS, int y, int x)
{

  for (int i = 0; i < offset; i++)
    printf(" ");
  printf("\e[32m%d\e[0m", y);
  printf("\e[32m|\e[0m");
  for (int i = 0; i < n; i++)
  {
    printf(" ");
    draw_content(y, i + x, game, pseudoN, pseudoS);
    printf(" \e[32m|\e[0m");
  }
  printf("\n");
}
/**
 * @brief Affiche le pied du plateau de jeu.
 *
 * Cette fonction affiche le pied du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 *
 * @author Arthur
 */
void foot_line(int n, int offset)
{

  for (int i = 0; i < offset + 1; i++)
    printf(" ");
  for (int i = 0; i < n; i++)
    printf("\e[36m\\ /\e[0m ");
  printf("\n");
}

/**
 * @brief Affiche le corps du plateau de jeu.
 *
 * Cette fonction affiche le corps du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 *
 * @author Arthur
 */
void body(int n, board game, char *pseudoN, char *pseudoS, int y)
{

  head_line(n, 1);
  body_line(n, 0, game, pseudoN, pseudoS, y, 0);
  foot_line(n, 1);
}

/**
 * @brief Affiche la tête du plateau de jeu.
 *
 * Cette fonction affiche la tête du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param x addition au numero
 * @param space espace en plus
 *
 * @author Arthur
 *
 *  */
void numerote(int n, int x, int space)
{
  printf("   ");
  for (int i = 0; i < n + space; i++)
  {
    printf(" ");
  }
  for (int i = 0; i < n; i++)
  {
    printf("\e[36m%d\e[0m   ", i + x);
  }
  printf("\n");
}

/**
 * @brief Affiche la tête du plateau de jeu.
 *
 * Cette fonction affiche la tête du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 *
 * @author Arthur
 */
void head(int n, int offset, board game, char *pseudoN, char *pseudoS, int y)
{

  head_line(n, offset + 1);
  body_line(n, offset, game, pseudoN, pseudoS, y, 0);
}

/**
 * @brief Affiche le pied du plateau de jeu.
 *
 * Cette fonction affiche le pied du plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 * @param x La colonne du plateau de jeu.
 *
 * @author Arthur
 */
void foot(int n, int offset, board game, char *pseudoN, char *pseudoS, int y, int x)
{

  body_line(n, offset, game, pseudoN, pseudoS, y, x);
  foot_line(n, offset + 1);
}

/**
 * @brief Affiche le plateau de jeu.
 *
 * Cette fonction affiche le plateau de jeu.
 *
 * @param n La taille du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 *
 * @author Arthur
 */
void fig(int n, board game, char *pseudoN, char *pseudoS)
{
  int y = 0;
  int size = n;
  for (int i = n - 1; i > 0; i--)
  {
    head(size, (i * 2), game, pseudoN, pseudoS, y);
    y++;
    size++;
  }

  body(size, game, pseudoN, pseudoS, y);
  y++;
  int x = 1;

  for (int i = 1; i < n; i++)
  {
    size--;
    foot(size, i * 2, game, pseudoN, pseudoS, y, x);
    y++;
    x++;
  }
}

void afficher_plateau(board game)
{
  char *pseudoN = "N";
  char *pseudoS = "S";
  if (is_hex(game))
  {
    printf("   ");
    for (int i = 0; i < MAX_DIMENSION * 2; i++)
    {
      if (i % 2 == 0)
      {
        printf("%d ", i / 2);
      }
    }
    printf("\n");
    for (int i = 0; i < MAX_DIMENSION * 2 + 1; i++)
    {
      for (int j = 0; j < MAX_DIMENSION * 2 + 1; j++)
      {
        draw_box(i, j, game, pseudoN, pseudoS);
      }
      printf("\n");
    }
    numerote(HEX_SIDE, 0, 5);
    fig((MAX_DIMENSION % 2 == 0) ? (MAX_DIMENSION / 2) : (MAX_DIMENSION / 2) + 1, game, pseudoN, pseudoS);
    numerote(HEX_SIDE, 4, 0);
  }
  else
  {
    printf("   ");
    for (int i = 0; i < NB_COLS * 2; i++)
    {
      if (i % 2 == 0)
      {
        printf("%d ", i / 2);
      }
    }
    printf("\n");
    for (int i = 0; i < NB_LINES * 2 + 1; i++)
    {
      for (int j = 0; j < NB_COLS * 2 + 1; j++)
      {
        draw_box(i, j, game, pseudoN, pseudoS);
      }
      printf("\n");
    }
  }
}

void initial(bool hex, bool portee)
{
  // Assertion qui verifie qu'une fonction fait bien ce qu'il est demandé

  /*

      ASSERTION NOMRAL GAME

  */

  board game = new_game();
  assert(is_hex(game) == false);
  assert(uses_range(game) == false);
  assert(current_player(game) == NORTH);

  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int k = 0; k < MAX_DIMENSION; k++)
    {
      if (get_content(game, i, k) == NORTH_KING)
      {
        assert(i == 0 && k == NB_COLS / 2);
      }
      else if (get_content(game, i, k) == SOUTH_KING)
      {
        assert(i == NB_LINES - 1 && k == NB_COLS / 2);
      }
      else if (i > NB_LINES - 1 || k > NB_COLS - 1)
      {

        assert(get_content(game, i, k) == KILLED);
      }
      else
      {

        assert(get_content(game, i, k) == EMPTY);
      }
    }
  }
  assert(get_winner(game) == NO_PLAYER);

  board copy_new_game = copy_game(game);
  assert(is_hex(copy_new_game) == false);
  assert(uses_range(copy_new_game) == false);
  assert(current_player(copy_new_game) == NORTH);

  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int k = 0; k < MAX_DIMENSION; k++)
    {
      if (get_content(copy_new_game, i, k) == NORTH_KING)
      {
        assert(i == 0 && k == NB_COLS / 2);
      }
      else if (get_content(copy_new_game, i, k) == SOUTH_KING)
      {
        assert(i == NB_LINES - 1 && k == NB_COLS / 2);
      }
      else if (i > NB_LINES - 1 || k > NB_COLS - 1)
      {

        assert(get_content(copy_new_game, i, k) == KILLED);
      }
      else
      {

        assert(get_content(copy_new_game, i, k) == EMPTY);
      }
    }
  }

  assert(kill_cell(game, 1, 1) == RULES);
  assert(move_toward(game, NW) == OUT);
  assert(move_toward(game, 17) == RULES);
  assert(move_toward(game, N) == OUT);
  assert(move_toward(game, NE) == OUT);
  assert(move_toward(game, W) == OK);
  assert(move_toward(game, W) == RULES);

  assert(get_content(game, 0, NB_COLS / 2) == EMPTY);
  assert(get_content(game, 0, NB_COLS / 2 - 1) == NORTH_KING);

  assert(kill_cell(game, -10, -10) == OUT);
  assert(kill_cell(game, 0, NB_COLS / 2 - 1) == BUSY);
  assert(kill_cell(game, NB_LINES - 1, NB_COLS / 2) == BUSY);
  assert(kill_cell(game, 1, 0) == OK);

  assert(get_content(game, 1, 0) == KILLED);
  assert(current_player(game) == SOUTH);

  assert(move_toward(game, W) == OK);
  assert(kill_cell(game, 1, 1) == OK);

  assert(current_player(game) == NORTH);

  assert(move_toward(game, W) == OK);
  assert(kill_cell(game, 1, 2) == OK);

  assert(current_player(game) == SOUTH);

  assert(move_toward(game, W) == OK);
  assert(kill_cell(game, 0, 2) == OK);

  assert(current_player(game) == NORTH);

  assert(move_toward(game, W) == OK);
  assert(kill_cell(game, 0, 1) == OK);

  assert(current_player(game) == SOUTH);

  assert(move_toward(game, W) == OK);
  assert(kill_cell(game, NB_LINES - 2, 1) == OK);

  assert(current_player(game) == NORTH);
  assert(get_winner(game) == SOUTH);

  assert(get_content(game, 0, 0) != get_content(copy_new_game, 0, 0));
  destroy_game(game);
  destroy_game(copy_new_game);

  game = new_game();

  int user = 0;
  for (int i = 0; i < 52; i++)
  {
    int x = 1 + (i % 6);
    int y = 1 + (i / 6);
    if (i < 6)
    {
      if (user == 0)
      {
        assert(move_toward(game, NW) == OUT);
        assert(move_toward(game, N) == OUT);
        assert(move_toward(game, NE) == OUT);
        assert(move_toward(game, W) == OK);
        user = 1;
      }
      else
      {
        assert(move_toward(game, SE) == OUT);
        assert(move_toward(game, S) == OUT);
        assert(move_toward(game, SW) == OUT);
        assert(move_toward(game, E) == OK);
        user = 0;
      }
    }
    else if (i >= 6 && i < 20)
    {
      if (user == 0)
      {
        assert(move_toward(game, NW) == OUT);
        assert(move_toward(game, W) == OUT);
        assert(move_toward(game, SW) == OUT);
        assert(move_toward(game, S) == OK);
        user = 1;
      }
      else
      {
        assert(move_toward(game, SE) == OUT);
        assert(move_toward(game, E) == OUT);
        assert(move_toward(game, NE) == OUT);
        assert(move_toward(game, N) == OK);
        user = 0;
      }
    }
    else if (i >= 20 && i < 27)
    {
      if (user == 0)
      {
        assert(move_toward(game, SE) == OUT);
        assert(move_toward(game, S) == OUT);
        assert(move_toward(game, SW) == OUT);
        assert(move_toward(game, E) == OK);
        user = 1;
      }
      else
      {
        assert(move_toward(game, NW) == OUT);
        assert(move_toward(game, N) == OUT);
        assert(move_toward(game, NE) == OUT);
        assert(move_toward(game, W) == OK);
        user = 0;
      }
    }

    kill_cell(game, x, y);
  }
  destroy_game(game);

  /*

       ASSERTION NORMAL GAME + DISTANCE

  */
  if (portee)
  {
    game = new_special_game(false, true);
    assert(is_hex(game) == false);
    assert(uses_range(game) == true);
    assert(current_player(game) == NORTH);
    assert(kill_cell(game, 0, 2) == RULES);
    assert(move_toward(game, S) == OK);
    assert(kill_cell(game, 3, 2) == OK);

    assert(move_toward(game, N) == OK);
    assert(kill_cell(game, 3, 3) == OK);

    assert(move_toward(game, SW) == OK);
    assert(kill_cell(game, 3, 4) == OK);

    assert(move_toward(game, N) == OK);
    assert(kill_cell(game, 4, 0) == OK);

    assert(move_toward(game, E) == OK);
    assert(kill_cell(game, 4, 1) == OK);

    assert(move_toward(game, W) == OK);
    assert(kill_cell(game, 7, 0) == OK);

    assert(move_toward(game, N) == OK);

    assert(kill_cell(game, 5, 1) == RULES);
    assert(kill_cell(game, 4, 0) == OUT);
    assert(kill_cell(game, 4, 3) == RULES);

    assert(kill_cell(game, 4, 4) == OK);

    assert(move_toward(game, N) == OK);
    assert(kill_cell(game, 1, 3) == BUSY);
  }

  /*

      ASSERTION HEX GAME

  */
  if (hex)
  {
    game = new_special_game(true, false);
    assert(is_hex(game) == true);
    assert(uses_range(game) == false);
    assert(current_player(game) == NORTH);

    for (int i = 0; i < MAX_DIMENSION; i++)
    {
      for (int j = 0; j < MAX_DIMENSION; j++)
      {
        if (j == HEX_SIDE / 2 && i == 0)
        {
          assert(get_content(game, i, j) == NORTH_KING);
        }
        else if (j == HEX_SIDE + HEX_SIDE / 2 - 1 && i == MAX_DIMENSION - 1)
        {
          assert(get_content(game, i, j) == SOUTH_KING);
        }
        else if ((i < HEX_SIDE && j >= MAX_DIMENSION - (HEX_SIDE - i - 1)) || (i > HEX_SIDE - 1 && j <= i - HEX_SIDE))
        {
          assert(get_content(game, i, j) == KILLED);
        }
        else
        {
          assert(get_content(game, i, j) == EMPTY);
        }
      }
    }

    assert(move_toward(game, N) == RULES);
    assert(move_toward(game, S) == RULES);

    assert(kill_cell(game, 1, 1) == RULES);
    assert(move_toward(game, NW) == OUT);
    assert(move_toward(game, NE) == OUT);
    assert(move_toward(game, W) == OK);
    assert(move_toward(game, W) == RULES);

    assert(get_content(game, 0, HEX_SIDE / 2) == EMPTY);
    assert(get_content(game, 0, HEX_SIDE / 2 - 1) == NORTH_KING);

    assert(kill_cell(game, -10, -10) == OUT);
    assert(kill_cell(game, 0, HEX_SIDE / 2 - 1) == BUSY);
    assert(kill_cell(game, MAX_DIMENSION - 1, HEX_SIDE + HEX_SIDE / 2 - 1) == BUSY);
    assert(kill_cell(game, 1, 0) == OK);
    assert(get_content(game, 1, 0) == KILLED);
    assert(current_player(game) == SOUTH);
    destroy_game(game);
    game = new_special_game(true, false);
    user = 0;
    int x = 1;
    int y = 0;
    for (int i = 0; i < 25; i++)
    {
      if (y == 4 + (i >= 8 ? 1 : 0) + (i >= 14 ? 1 : 0) + (i >= 21 ? 1 : 0) - (i >= 27 ? 1 : 0) - (i >= 32 ? 1 : 0) - (i >= 36 ? 1 : 0))
      {
        y = 1 + (i >= 21 ? 1 : 0) + (i >= 27 ? 1 : 0) + (i >= 32 ? 1 : 0) + (i >= 36 ? 1 : 0);
        x++;
      }
      else
      {
        y++;
      }
      if (i < 4)
      {
        if (user == 0)
        {
          assert(move_toward(game, NW) == OUT);
          assert(move_toward(game, NE) == OUT);
          assert(move_toward(game, W) == OK);
          user = 1;
        }
        else
        {
          assert(move_toward(game, SE) == OUT);
          assert(move_toward(game, SW) == OUT);
          assert(move_toward(game, E) == OK);
          user = 0;
        }
      }
      else if (i < 12)
      {
        if (user == 0)
        {
          assert(move_toward(game, NW) == OUT);
          assert(move_toward(game, W) == OUT);
          assert(move_toward(game, SW) == OK);
          user = 1;
        }
        else
        {
          assert(move_toward(game, E) == OUT);
          assert(move_toward(game, SE) == OUT);
          assert(move_toward(game, NE) == OK);
          user = 0;
        }
      }
      else if (i < 20)
      {
        if (user == 0)
        {
          assert(move_toward(game, W) == OUT);
          assert(move_toward(game, SW) == OUT);
          assert(move_toward(game, SE) == OK);
          user = 1;
        }
        else
        {
          assert(move_toward(game, NE) == OUT);
          assert(move_toward(game, E) == OUT);
          assert(move_toward(game, NW) == OK);
          user = 0;
        }
      }
      else if (i < 25)
      {
        if (user == 0)
        {
          assert(move_toward(game, SW) == OUT);
          assert(move_toward(game, SE) == OUT);
          assert(move_toward(game, E) == OK);
          user = 1;
        }
        else
        {
          assert(move_toward(game, NE) == OUT);
          assert(move_toward(game, NW) == OUT);
          assert(move_toward(game, W) == OK);
          user = 0;
        }
      }
      assert(kill_cell(game, x, y) == OK);
    }
    destroy_game(game);
  }
  /*

       ASSERTION HEX GAME + DISTANCE

  */
  if (portee && hex)
  {
    game = new_special_game(true, true);
    assert(is_hex(game) == true);
    assert(uses_range(game) == true);
    assert(current_player(game) == NORTH);

    assert(move_toward(game, SW) == OK);
    assert(kill_cell(game, 3, 2) == OK);

    assert(move_toward(game, NW) == OK);

    assert(kill_cell(game, 3, 3) == RULES);
    assert(kill_cell(game, 4, 3) == OK);

    assert(move_toward(game, SE) == OK);
    assert(kill_cell(game, 3, 4) == OK);

    assert(move_toward(game, NW) == OK);
    assert(kill_cell(game, 5, 2) == OK);

    assert(move_toward(game, W) == OK);
    assert(kill_cell(game, 3, 3) == OK);
    assert(move_toward(game, NE) == OK);
    assert(kill_cell(game, 6, 6) == OK);

    assert(move_toward(game, E) == OK);

    assert(kill_cell(game, 4, 4) == RULES);

    assert(kill_cell(game, 4, 2) == RULES);
    assert(kill_cell(game, 4, 1) == RULES);
    assert(kill_cell(game, 4, 3) == OUT);
    assert(kill_cell(game, 3, 1) == OK);

    assert(move_toward(game, NE) == OK);
    assert(kill_cell(game, 1, 3) == RULES);
    assert(kill_cell(game, 4, 7) == OK);
    assert(move_toward(game, E) == OK);
    assert(kill_cell(game, 4, 4) == BUSY);
    destroy_game(game);
  }
}
void approfondis(bool hex, bool portee)
{
  /*

      ASSERTION NOMRAL GAME

  */

  board game = new_game();
  assert(NB_COLS == 7);
  assert(NB_LINES == 8);
  assert(HEX_SIDE == 5);
  assert(MAX_DIMENSION == 9);
  assert(NB_PLAYERS == 2);
  assert(KING_RANGE == 3);
  assert(MAX_DIMENSION >= NB_COLS);
  assert(MAX_DIMENSION >= NB_LINES);
  assert(MAX_DIMENSION >= 2 * HEX_SIDE - 1);
  destroy_game(game);

  game = new_game();
  for (int i = -3; i < MAX_DIMENSION + 3; i++)
  {
    for (int j = -3; j < MAX_DIMENSION + 3; j++)
    {
      if (!(i >= 0 && i < MAX_DIMENSION && j >= 0 && j < MAX_DIMENSION))
      {
        assert(get_content(game, i, j) == KILLED);
      }
    }
  }
  destroy_game(game);

  //int number_game = 1000000;
  //for (int i = 0; i < number_game; i++)
  //{
  //  game = new_game();
  //  while (get_winner(game) == NO_PLAYER)
  //  {
  //    bot_move(game);
  //    bot_kill(game);
  //    board temp_game_1 = copy_game(game);
  //    board temp_game_2 = copy_game_assert(game);
  //    enum return_code result = get_winner(temp_game_1);
  //    enum return_code result_assert = get_winner_assert(temp_game_2);
  //    destroy_game(temp_game_1);
  //    destroy_game(temp_game_2);
  //    assert(result == result_assert);
  //  }
  //  afficher_plateau(game);
  //  printf("Gagnant : %d\n", game->current);
  //  destroy_game(game);
//
  //  game = new_special_game(false, true);
  //  while (get_winner(game) == NO_PLAYER)
  //  {
  //    bot_move(game);
  //    bot_kill(game);
  //    board temp_game_1 = copy_game(game);
  //    board temp_game_2 = copy_game_assert(game);
  //    enum return_code result = get_winner(temp_game_1);
  //    enum return_code result_assert = get_winner_assert(temp_game_2);
  //    destroy_game(temp_game_1);
  //    destroy_game(temp_game_2);
  //    assert(result == result_assert);
  //  }
  //  afficher_plateau(game);
  //  printf("Gagnant : %d\n", game->current);
  //  destroy_game(game);
//
  //  game = new_special_game(true, false);
  //  while (get_winner(game) == NO_PLAYER)
  //  {
  //    bot_move(game);
  //    bot_kill(game);
  //    board temp_game_1 = copy_game(game);
  //    board temp_game_2 = copy_game_assert(game);
  //    enum return_code result = get_winner(temp_game_1);
  //    enum return_code result_assert = get_winner_assert(temp_game_2);
  //    destroy_game(temp_game_1);
  //    destroy_game(temp_game_2);
  //    assert(result == result_assert);
  //  }
  //  afficher_plateau(game);
  //  printf("Gagnant : %d\n", game->current);
  //  destroy_game(game);
//
  //  int number_game = 1000;
  //  for (int i = 0; i < number_game; i++)
  //  {
  //    game = new_special_game(true, true);
  //    while (get_winner(game) == NO_PLAYER)
  //    {
  //      bot_move(game);
  //      bot_kill(game);
  //      board temp_game_1 = copy_game(game);
  //      board temp_game_2 = copy_game_assert(game);
  //      enum return_code result = get_winner(temp_game_1);
  //      enum return_code result_assert = get_winner_assert(temp_game_2);
  //      destroy_game(temp_game_1);
  //      destroy_game(temp_game_2);
  //      assert(result == result_assert);
  //    }
  //    afficher_plateau(game);
  //    printf("Gagnant : %d\n", game->current);
  //    destroy_game(game);
  //  }
  //}
  //// Check if destroy game work well
  //for (int i = 0; i < 5000000; i++)
  //{
  //  board create_game = new_game();
  //  destroy_game(create_game);
  //}
  //assert(is_memory_usage_above_1GB() == false);
}

int main()
{

  srand(time(NULL));
  bool hex = false;
  bool portee = true;
  initial(hex, portee);
  approfondis(hex, portee);
  return 0;
}
