#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ScreenDisplay.h"
#include "AI.h"


/*#define debug*/

Piece *MakeBoard(Piece *pieces) {
    int i;
    for (i = 0; i < 64; i++) {
        pieces[i].letPos = i % 8 + 'a'; 
        pieces[i].numPos = i/ 8 + 1;
        pieces[i].movevalue = 0;
        pieces[i].moveindex= 0;
        pieces[i].hasmoved = 0;
        pieces[i].enpassant = ' ';
       if( i <16)
           pieces[i].color = 'w';
       if ( i >47)
           pieces[i].color = 'b';
       if( i < 16 || i > 47) {
         if((i > 7 && i < 16) || (i > 47 && i < 56)) {
           pieces[i].name = 'p';
           pieces[i].value = 1;
         }
        else if(i == 0 || i == 7 || i ==56 || i == 63) {
          pieces[i].name = 'r';
          pieces[i].value = 5;
        }
        else if(i == 1 || i == 6 || i == 57 || i == 62) {
          pieces[i].name = 'n';
          pieces[i].value = 3;   
        }
        else if(i == 2 || i == 5 || i == 58 || i == 61) {
          pieces[i].name = 'b';
          pieces[i].value = 3;
        }
        else if(i == 3 || i == 59) {
          pieces[i].name = 'q';
          pieces[i].value = 9;
        }
        else {
          pieces[i].name = 'k';
          pieces[i].value = 1;
        }
      }
     }
    return pieces;
}

void Play(Piece *board, char w, char b) {
    srand(time(NULL));
    int checkmate;
    Display(board);
    /* human = 0, ai = 1 */
    while(1) {            
      if(w == 1){
      	sleep(1);
        AITurn(board, 'w');
        }
      else
        HumanTurn(board, 'w');
      Display(board);
      if(IsInCheck(board, 'b')){
        checkmate = IsInCheckmate(board, 'b');
        if(checkmate == 2){
          printf("Black wins!\n");
          break;
        }
        if(checkmate == 1){
          printf("White wins!\n");
          break;
        }
        if(checkmate == 0){
          printf("Black is in check!");
        }
      }
      if(b == 1){
      	sleep(1);
        AITurn(board, 'b');
        }
      else
        HumanTurn(board, 'b');
      Display(board);
      if(IsInCheck(board, 'w')){
        checkmate = IsInCheckmate(board, 'w');
        if(checkmate == 2){
          printf("White wins!\n");
          break;
        }
        if(checkmate == 1){
          printf("Black wins!\n");
          break;
        }
        if(checkmate == 0){
          printf("White is in check!");
        }
      }  
    }
    printf("Thank you for playing!\n");
}




/* Everything below this point shouldn't need to be coded */

void Display(Piece *board) {
    system("clear");
    int x, y, i;
    char str[] = "abcdefgh";
    for (x = 7; x >= 0; x--) {
        printf(" +----+----+----+----+----+----+----+----+\n");
        printf("%d", x+1);
        for (y = 0; y < 8 ; y++) {
            printf("| ");
            printf("%c%c",board[y + x * 8].color, board[y + x * 8].name);
            printf(" ");
        }
        printf("| \n");
    }
    printf(" +----+----+----+----+----+----+----+----+\n ");
    for (i = 0; i < 8; i++) {
        printf("  %c  ", str[i]);
    }
    printf("\n");
}

void PrintMenu() {
printf("--------------------------------------------"
        "\nWelcome to Chessbot 2k17!"
        "\nType the number for the game mode to play:"
        "\n1. Human versus Human"
        "\n2. Human(White) versus Computer(Black)"
        "\n3. Human(Black) versus Computer(White)"
        "\n4. Computer versus Computer"
        "\n5. How to play"
        "\n6. Credits"
        "\n7. Exit"
        "\n--------------------------------------------\n");
}

void Help(){
    printf(" \nHow to Play\n" );
    printf("\nThe King (K)-The king is the most important piece, but is one of the weakest. The king can only move one square in any direction - up, down, left, right, and diagonally.The king may never move himself into check (where he could be captured). The king must also always immediately move if he is in check.\n");
    printf("\nThe Queen (Q)-The queen is the most powerful piece. She can move in any one straight direction - forward, backward, sideways, or diagonally - as far as possible as long as she does not move through any of her own pieces.\n"); 

    printf("\nThe Rook (R)-The rook may move as far as it wants, but only forward, backward, and to the sides. The rooks are particularly powerful pieces when they are protecting each other and working together!\n");
    printf("\nThe Bishop (B)-The bishop may move as far as it wants, but only diagonally. Each bishop starts on one color (light or dark) and must always stay on that color. Bishops work well together because they cover up each other’s weaknesses.\n");
printf("\nThe Knight (N)-Knights move in a very different way from the other pieces – going two squares in one direction, and then left or right of that angle, just like the shape of an “L”. Knights are also the only pieces that can move over other pieces.\n");

printf("\nThe Pawn (P)-Pawns are unusual because they move and capture in different ways: they move forward, but capture diagonally. Pawns can only move forward one square at a time, except for their very first move where they may move forward two squares. Pawns can only capture one square diagonally in front of them. They can never move or capture backwards. If there is another piece directly in front of a pawn he cannot move past or capture that piece. When reaching the end of the other board, a pawn can be promoted to any other piece except a king.\n"); 

printf("\nCheck: The word Check is declared when a player threatens to take the opponent’s King. The opposing player must immediately move their King out of check in the next game or lose the game.\n"); 

printf("\nCheckmate: “Checkmate” is declared when a player Checks the opposing player’s King and the opposing player has no legal move to move the King out of check. The checked player loses the game.\n"); 

printf("\nCastling: A special move that can only occur if the King and one of the Rooks have not moved its original position. The King moves two spaces towards the specified Rook and the Rook moves to the other side of the King. This can only occur if all of the spaces between the King and the Rook are unoccupied, the King is not in check, and the King does not become in check while making the Castling movement.\n"); 

printf("\nEn Passant: If a pawn moves out two squares on its first move, and by doing so lands to the side of an opponent’s pawn (effectively jumping past the other pawn’s ability to capture it), that other pawn has the option of capturing the first pawn as it passes by. This special move must be done immediately after the first pawn has moved past, otherwise the option to capture it is no longer available.\n"); 

printf("\nPromotion: Pawns have another special ability and that is that if a pawn reaches the other side of the board it can become any other chess piece (called promotion). A pawn may be promoted to any piece except a king.\n");
}

void Credits(){
    printf("Credits\n\n");
    printf("Sexiest Man Alive\n");
    printf("Brian Truong\n\n");
    printf("Executive Deluxe Programmer\n");
    printf("Desmond Chan\n\n");
    printf("Programmer\n");
    printf("William Scotten\n\n");
    printf("Brogrammer\n");
    printf("Hikaru Kasai\n\n");
}

void Intro(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("   \t\t\t _____ _                   _           _     _____  _     __   ______\n");
    printf("   \t\t\t/  __ \\ |                 | |         | |   / __  \\| |   /  | |___  /\n");
    printf("   \t\t\t| /  \\/ |__   ___  ___ ___| |__   ___ | |_  `' / /'| | __`| |    / /\n"); 
    printf("   \t\t\t| |   | '_ \\ / _ \\/ __/ __| '_ \\ / _ \\| __|   / /  | |/ / | |   / /\n");  
    printf("   \t\t\t| \\__/\\ | | |  __/\\__ \\__ \\ |_) | (_) | |_  ./ /___|   < _| |_./ /\n");   
    printf("   \t\t\t \\____/_| |_|\\___||___/___/_.__/ \\___/ \\__| \\_____/|_|\\_\\\\___/\\_/\n");    
   
    printf("   \t\t\t\t\t           ___       ___ \n");
    printf("   \t\t\t\t\t          [___] /~\\ [___] \n");
    printf("   \t\t\t\t\t          |ooo|.\\_/.|ooo| \n");
    printf("   \t\t\t\t\t          |888||   ||888| \n");
    printf("   \t\t\t\t\t         /|888||   ||888|\\ \n");
    printf("   \t\t\t\t\t       /_,|###||___||###|._\\ \n");
    printf("   \t\t\t\t\t      /~\\  ~~~ /[_]\\ ~~~  /~\\ \n");
    printf("   \t\t\t\t\t     (O_O) /~~[_____]~~\\ (O_O) \n");
    printf("   \t\t\t\t\t          (  |       |  ) \n");
    printf("   \t\t\t\t\t         [~` ]       [ '~] \n");
    printf("   \t\t\t\t\t         |~~|         |~~| \n");
    printf("   \t\t\t\t\t        _<\\/>_       _<\\/>_ \n");
    printf("   \t\t\t\t\t       /_====_\\     /_====_\\ \n"); 

    printf("     _________  ___________    _____      _________       ____  __.     .____       _____ __________  _________\n");
    printf("    /   _____/  \\__    ___/   /  _  \\     \\_   ___ \\     |    |/ _|     |    |     /  _  \\\\______   \\/   _____/\n");
    printf("    \\_____  \\     |    |     /  /_\\  \\    /    \\  \\/     |      <       |    |    /  /_\\  \\|    |  _/\\_____  \\ \n");
    printf("    /        \\    |    |    /    |    \\   \\     \\____    |    |  \\      |    |___/    |    \\    |   \\/        \\\n");
    printf("   /_______  / /\\ |____| /\\ \\____|__  / /\\ \\______  / /\\ |____|__ \\ /\\  |_______ \\____|__  /______  /_______  /\n");
    printf("           \\/  \\/        \\/         \\/  \\/        \\/  \\/         \\/ \\/          \\/       \\/       \\/        \\/\n");
    printf("\n    \t\t\t\t\t      Press Enter to Continue\n\n");
    getchar();
}

