/*Groupe SAE n°4 - Clément Esnault & Marin Lerondel--Touzé*/


#include <stdio.h>
#include "board.h"


// Définir une constante pour la longueur maximale du pseudo
#define MAX_PSEUDO_LENGTH 50

// Stockage des pseudos des joueurs
char player_names[2][MAX_PSEUDO_LENGTH];

// Fonction pour demander les pseudos des joueurs
void demander_pseudos() {
    printf("Entrez le pseudo du joueur 1 (N): ");
    scanf("%s", player_names[0]);
    printf("Entrez le pseudo du joueur 2 (S): ");
    scanf("%s", player_names[1]);
}

// Fonction pour obtenir l'initiale d'un pseudo
char obtenir_initiale(char *pseudo) {
    return pseudo[0]; // L'initiale est simplement la première lettre du pseudo
}


void affichage(board game, int lig, int col){
    cell affiche = get_content(game,lig,col);
        /*ON REMPLACE LES 0, 1, 2, 3 QUI S'AFFICHENT DANS LES CASES PAR LES LETTRES CORRESPONDANTES*/
        if (affiche==3){
            char caractere = '#';
            printf("\033[37;47m%c\033[0m", caractere);
        }
        else if (affiche==1){
            char caractere = obtenir_initiale(player_names[0]); 
            printf("\033[1;32m%c\033[0m", caractere);
        }
        else if (affiche==2){
            char caractere = obtenir_initiale(player_names[1]);
            printf("\033[1;36m%c\033[0m", caractere);
        }
        else{
            printf(" ");
        }
}


void visualisation_standart(board game){
    /*AFFICHAGE DE LA GRILLE*/
    printf(" 0 1 2 3 4 5 6 \n");
    int s;
    printf("\u2554\u2550\u2566\u2550\u2566\u2550\u2566\u2550\u2566\u2550\u2566\u2550\u2566\u2550\u2557 \n");
    for (int i =0; i<NB_LINES;i++){
        for(int j=0; j<NB_COLS;j++){
            printf("\u2551");
            affichage(game,i,j);
            }
            s=i;
            printf("\u2551 %d",s);
            printf("\n");
            printf("\u2560\u2550\u256C\u2550\u256C\u2550\u256C\u2550\u256C\u2550\u256C\u2550\u256C\u2550\u2563 \n");
        }

}



void visualisation_hexa(board game) {

    /*AFFICHAGE HEXAGONE HAUT*/
    int stock=1;
    for (int i = 0; i < HEX_SIDE * 2; i++) {
        stock=0;
        for (int j = HEX_SIDE * 2 - 1; j > i; j--) {
            printf(" ");
        }
        if (i % 2 == 0) {
            for (int j = 0; j < HEX_SIDE + i/2; j++) {
                printf("/ \\ ");
            }
            printf("\n");
        }
        else {
            for (int j = 0; j < i / 2 + HEX_SIDE + 1; j++) {
                if(j != i/2 + HEX_SIDE){
                    printf("| ");
                    affichage(game, (i-1)/2,j);
                    printf(" ");
                }
                else{
                    printf("|\n");;
                }
        }
        
    }
}

    /*AFFICHAGE HEXAGONE BAS*/
    stock=0;
    //stocka=0;
    for (int i = HEX_SIDE * 2 - 1; i > 0 ; i--) {
        stock++;
        for (int j = HEX_SIDE * 2; j > i; j--) {
            printf(" ");
        }
        if (i % 2 == 1) {
            for (int j = 0; j < HEX_SIDE + i/2; j++) {
                printf("\\ / ");
            }
            printf("\n");
        }
        else {
            for (int j = 0; j < i / 2 + HEX_SIDE; j++) {
                if(j != i/2 + HEX_SIDE){
                    printf("| ");
                    if(j != i / 2 + HEX_SIDE - 1){
                        affichage(game, HEX_SIDE*2 - i/2 - 1, j + stock/2);
                    }
                    printf(" ");
                }
                else{
                    printf("|   ");
                }

            }
            printf("\n");
        }
    }
}


int conversion(board game, int choix_direc){
    /*ON CONVERTI LES CHOIX DE DIRECTION DE L'USER EN FONCTION DES DIRECTIONS DE MOVE_TOWARD*/
    switch(choix_direc){
        case 1:
            return SW;
        break;
        case 2:
            return S;
            break;
        case 3:
            return SE;
            break;
        case 4:
            return W;
            break;
        case 6:
            return E;
            break;
        case 7:
            return NW;
            break;
        case 8:
            return N;
            break;
        case 9:
            return NE;
            break;
        default:
            return 1000;
            break;
    }
    return 11000;
}


int move_player_stard(board game){
    /*ON FAIT LE DÉPLACEMENT DU JOUEUR POUR UNE PARTIE STANDARD*/
    int nb_convertit;
    int choix_direc;
    enum return_code choix = -1;
    while(choix != OK){
        /*ON AFFICHE LES POSSIBILITÉS DE MOUVEMENT*/
        printf("choisissez la direction : \n");
        printf("7 8 9\n");
        printf("4   6\n");
        printf("1 2 3\n");
        /*oN REGARDE SI LA VALEUR ENTREE EST BIEN UN CHIFFRE*/
        while(scanf("%d",&choix_direc) == 0){
            getchar();
        }
        
        nb_convertit=conversion(game,choix_direc);
        choix = move_toward(game,nb_convertit);
        
        if(choix == OK){
            printf("déplacement réussi.\n");
        }
        else{
            /*SI LE DEPLACEMENT ECHOUE, ON RETOURNE AU DEBUT DE LA FONCTION*/
            printf("Erreur, vous ne pouvez pas vous déplacer en direction %d\n", choix);
            return move_player_stard(game);
        }
    }
    return 1;
}



int move_player_hexa(board game){
    /*ON FAIT LE DÉPLACEMENT DU JOUEUR*/
    int nb_convertit;
    int choix_direc;
    enum return_code choix = -1;
    while(choix != OK){
        /*ON AFFICHE LES POSSIBILITÉS DE MOUVEMENT*/
        printf("choisissez la direction : \n");
        printf("     / \\ / \\\n");
        printf("    | 7 | 9 |\n");
        printf("   / \\ / \\ / \\\n");
        printf("  | 4 | * | 6 |\n");
        printf("   \\ / \\ / \\ /\n");
        printf("    | 1 | 3 |\n");
        printf("     \\ / \\ /\n");
        /*ON REGARDE SI LA VALEUR ENTREE EST BIEN UN CHIFFRE*/
        while(scanf("%d",&choix_direc) == 0){
            getchar();
        }
        nb_convertit=conversion(game,choix_direc);
        choix = move_toward(game,nb_convertit);
        
        if(choix == OK){
            printf("déplacement réussi.\n");
        }
        else{
            /*SI LE DEPLACEMENT ECHOUE, ON RETOURNE AU DEBUT DE LA FONCTION*/
            printf("Erreur, vous ne pouvez pas vous déplacer en direction %d\n", choix);
            return move_player_hexa(game);
        }
    }
    /*ON BOUGE LE JOUEUR*/
    return 1;
}



int del_player(board game){
    /*ON DEMANDE AU JOUEUR QUELLE CELLULE IL VEUT DETRUIRE*/
    int choix_ligne;
    int choix_col;
    enum return_code result2 = -1;
    while(result2 != OK){
        printf("choisissez la ligne \n");
        scanf("%d",&choix_ligne);
        printf("choisissez la colonne \n");
        scanf("%d",&choix_col);
        result2 = kill_cell(game, choix_ligne, choix_col); 

        if(result2 == RULES){
            printf("Il aurait fallu réussir à bouger avant.\n");
        }
        else if(result2 == OK){
            printf("ça marche!\n");
        }
        else{
            printf("Bizarre.\n");
            return del_player(game);
        }
    }
    kill_cell(game,choix_ligne,choix_col);
    return 1;
}


void jeu(){
    demander_pseudos();

    int choix_hexa;
    printf("voulez-vous jouer sur un tableau hexagonal? Oui : 1 // Non : 0 // ");
    scanf("%d", &choix_hexa);

    int choix_range;
    printf("Voulez-vous jouer avec la portée ? Oui : 1 // Non : 0 // ");
    scanf("%d", &choix_range);

    /*ON CREE LA GRILLE*/
    board game = new_special_game(choix_hexa, choix_range);

    if(choix_hexa==1){
        visualisation_hexa(game);
        while(get_winner(game)==0){
            printf("joueur: %s\n",player_names[current_player(game) - 1]);
            move_player_hexa(game);
            del_player(game);
            visualisation_hexa(game);
        }
    }
    else{
        visualisation_standart(game);
        while(get_winner(game)==0){
            printf("joueur: %s \n",player_names[current_player(game) - 1]);
            move_player_stard(game);
            del_player(game);
            visualisation_standart(game);
        }
    }
    printf("Le gagnant est :  %d\n", get_winner(game));
printf(
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣏⢳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣫⠬⠿⡦⡙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡾⣽⠃⡴⠚⠻⣞⣆⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣁⡇⢸⠁⡦⡄⢹⠼⡆⢰⣯⣲⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣾⣿⣿⠀⡇⢾⠀⣗⡇⢸⠂⡇⠘⢿⣽⠞⠋⠀⠀⠀⠀⠀⢀⡴⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠛⣟⠦⣄⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⠀⢿⡸⣄⠙⢁⣼⣰⠃⢠⡞⠁⠀⠀⠀⢀⣄⣀⣠⠏⣠⡄⢻⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡄⢹⣧⣙⣶⣾⡿⢛⡓⢦⡀⠀⠀⠈⢧⠈⢷⣍⣳⣾⡵⠃⢀⡟⠀⠀⠀⠀⣼⠟⠛⢿⣿⣶⣿⡇⢠⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⢀⣤⠶⢦⣤⣀⣠⡶⠦⣄⠀⠀⣠⣤⣷⣌⣿⠿⠻⠋⢸⠋⠉⠻⡷⠀⠀⠀⠈⢧⠀⣸⠋⠙⡇⠀⢸⠀⠀⠀⠀⢸⣧⠤⠶⣄⠀⠈⠉⠁⢼⡿⡿⣦⡀⠀⠀⠀⠀⠀⣀⡤⠴⣄⠀\n"
        "⡞⠁⣠⠶⢤⡟⢳⡦⣤⠘⣧⡾⢿⣿⣿⡛⠁⢰⣄⠀⢸⣤⣀⠀⠀⠀⠀⠀⠀⢸⢠⡿⠀⠀⢻⡀⠈⢧⡀⠀⠀⠈⠁⠀⢀⣾⠀⣠⣆⠀⢠⡾⠶⣌⣷⠀⢠⠖⣺⠟⠁⠀⠀⠈⢳\n"
        "⣇⠀⣧⣤⡀⢳⠼⣇⣼⠀⡿⣧⣾⠀⠀⠹⣄⢸⣈⣷⠞⠃⣨⣟⠀⠀⠀⠀⣠⣾⣿⠃⢠⣆⠀⠳⡄⠀⠛⢦⣀⣀⣀⣠⣿⡉⠛⠯⢿⣦⣿⣄⠀⢸⡿⢀⡏⡼⠁⢠⠞⠉⣹⣦⢀\n"
        "⠉⠛⠁⠀⣹⣾⣦⣌⢻⡄⣇⠈⠉⠀⠀⠀⣿⣉⣡⣤⣤⡞⠁⠈⠛⠒⠲⢛⣿⡿⠁⠀⢸⠉⢧⡀⠙⣦⣄⠀⠀⠉⣩⣽⡿⠛⠓⠲⣶⢤⣼⠏⠀⠀⠁⢸⢠⠁⣠⠟⠲⣎⠉⠘⠿\n"
        "⠀⠀⠀⠀⣿⠀⠀⠙⣧⢿⡻⣄⠀⠀⠀⣀⡼⢃⡿⠁⢱⣍⡓⠲⠶⠶⠞⠛⠉⠀⢀⡴⠃⠀⠀⠳⣄⠀⠙⠛⠻⠟⠛⠋⠀⣀⡀⠀⢹⡀⢳⡀⠀⠀⢀⡟⣸⢰⠃⠀⠀⢸⠀⠀⠀\n"
        "⠀⠀⠀⠀⠘⢇⣀⣠⡿⠈⠳⣌⣉⠛⠋⢉⣠⠞⠁⢠⠏⠀⠉⠓⠲⠤⠤⣤⠤⠖⠋⠀⠀⠀⠀⠀⠈⠳⣄⣀⠀⠀⢀⣠⠼⠋⠳⣄⠀⢳⡀⠙⠒⠒⠋⡰⠃⣼⡀⠀⣠⡟⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠙⣯⠙⠢⣄⡀⠉⠉⠉⠉⢁⣀⣴⣋⣀⣀⣠⣤⣤⣤⣤⣤⡤⠤⠤⢤⣤⣤⣤⣤⣤⣀⣀⣈⣉⣉⠉⠀⠀⠀⠀⠈⠳⣄⡉⠒⠒⠒⢊⣡⠞⠉⡽⠛⠉⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠘⣄⣀⣠⠽⠿⠒⠒⠛⠉⠉⠉⣁⣀⣀⣤⣤⣤⠤⠤⠤⠤⠤⠤⠴⠴⠤⠤⠤⠤⠤⠤⠤⠬⠭⠭⣭⣽⣿⣶⣶⣶⣿⣶⣶⣾⣭⣄⣀⣀⡚⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⣼⣋⣡⣤⣴⡶⠒⢒⣛⡛⠏⠉⠉⠀⠀⠀⠀⢠⣴⠶⣶⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡴⢶⣤⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⢻⡖⠿⢭⣩⣽⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠉⢯⠀⢰⠃⢀⣾⠋⠉⣿⠀⠀⠀⠀⠀⠀⠀⠸⣦⣄⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣄⣀⣽⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⠀⠀⡏⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠘⡆⠸⡄⠀⠛⠷⡾⠋⠀⢀⣀⣀⣀⣀⣀⣀⣈⣉⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣉⣉⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⣸⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⣳⡤⠴⠒⠚⠋⢉⣉⣉⣁⣀⣠⣤⠤⠤⠤⠤⠤⠤⠬⠥⠭⠭⠭⠭⠭⠭⠭⠭⠭⠭⣭⣍⣉⣉⣉⡉⠙⠓⠒⠒⠶⣯⣀⠘⡆⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠋⣠⠤⠖⠒⠛⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠙⠓⠒⠲⠤⢤⣙⣷⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠋⣠⠤⠖⠒⠛⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠙⠓⠒⠲⠤⢤⣙⣷⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠳⢾⣁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⡏⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠓⠒⠒⠦⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣤⠤⠴⢖⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    destroy_game(game);
}


int main(int args, char **argv){

    jeu();
    
    return 0;
}