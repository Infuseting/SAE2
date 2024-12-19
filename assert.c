#include <stdio.h>
#include <assert.h>
#include "board.h"

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
    int x = 1+ ( i % 6);
    int y = 1+ ( i / 6);
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
  if (portee) {
    game = new_special_game(false, true);
    assert(is_hex(game) == false);
    assert(uses_range(game) == true);
    assert(current_player(game) == NORTH);
    
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
  if (hex) {
    game = new_special_game(true, false);
    assert(is_hex(game) == true);
    assert(uses_range(game) == false);
    assert(current_player(game) == NORTH);

    for (int i = 0; i < MAX_DIMENSION; i++) {
      for (int j = 0; j < MAX_DIMENSION; j++) {
        if (j == HEX_SIDE / 2  && i == 0) {
          assert(get_content(game, i, j) == NORTH_KING);
        }
        else if (j == HEX_SIDE + HEX_SIDE / 2 - 1 && i == MAX_DIMENSION - 1) {
          assert(get_content(game, i, j) == SOUTH_KING);
        }
        else if ((i < HEX_SIDE && j >= MAX_DIMENSION - (HEX_SIDE - i - 1)) || (i > HEX_SIDE - 1 && j <= i - HEX_SIDE) ) {
          assert(get_content(game, i, j) == KILLED);
        }
        else {
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
    assert(get_content(game, 0, HEX_SIDE / 2- 1) == NORTH_KING);
    
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
    for (int i = 0; i < 25; i++) {
      if (y == 4 + (i >= 8 ? 1 : 0) + (i >= 14 ? 1 : 0) + (i >= 21 ? 1 : 0) - (i >= 27 ? 1 : 0) - (i >= 32 ? 1 : 0) - (i >= 36 ? 1 : 0)) {
        y = 1 + (i >= 21 ? 1 : 0) + (i >= 27 ? 1 : 0) + (i >= 32 ? 1 : 0) + (i >= 36 ? 1 : 0);
        x++;
      } else {
        y++;
      }
      
      if (i < 4) {
        if (user == 0) {
          assert(move_toward(game, NW) == OUT);
          assert(move_toward(game, NE) == OUT);
          assert(move_toward(game, W) == OK);
          user = 1;
        }
        else {
          assert(move_toward(game, SE) == OUT);
          assert(move_toward(game, SW) == OUT);
          assert(move_toward(game, E) == OK);
          user = 0;
        }
      }
      else if (i < 12) {
        if (user == 0) {
          assert(move_toward(game, NW) == OUT);
          assert(move_toward(game, W) == OUT);
          assert(move_toward(game, SW) == OK);
          user = 1;
        }
        else {
          assert(move_toward(game, E) == OUT);
          assert(move_toward(game, SE) == OUT);
          assert(move_toward(game, NE) == OK);
          user = 0;
        }
      }
      else if (i < 20) {
        if (user == 0) {
          assert(move_toward(game, W) == OUT);
          assert(move_toward(game, SW) == OUT);
          assert(move_toward(game, SE) == OK);
          user = 1;
        }
        else {
          assert(move_toward(game, NE) == OUT);
          assert(move_toward(game, E) == OUT);
          assert(move_toward(game, NW) == OK);
          user = 0;
        }
      
      }
      else if (i < 25) {
        if (user == 0) {
          assert(move_toward(game, SW) == OUT);
          assert(move_toward(game, SE) == OUT);
          assert(move_toward(game, E) == OK);
          user = 1;
        }
        else {
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
  if (portee && hex) {
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
  
  /*

       ASSERTION NORMAL GAME + DISTANCE

  */
  if (portee) {

  }

  /*

      ASSERTION HEX GAME

  */
  if (hex){

  }
  /*

       ASSERTION HEX GAME + DISTANCE

  */
  if (hex && portee) {
    
  }
}



int main()
{
  bool hex = true;
  bool portee = true;
  initial(hex, portee);
  approfondis(hex, portee);
  return 0;
}