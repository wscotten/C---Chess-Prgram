#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

#include "AI.h"

int main() {
    Intro();
    system("clear");
    int i = 0;
    Piece pieces[64];
    while(1) {
      PrintMenu();
      FILE *fp;
	fp = fopen("log.txt", "w+");
	if(!fp) 
	{	
		printf("Error! Cannot open log.txt\n");
		exit(1);
	}
/*	fclose(fp);*/
	
      int s;
      scanf("%d", &s);
      if(s == 1) {
        Piece *board = MakeBoard(pieces);
        for(i = 16; i < 48; i++) 
          WipePiece(board, i);
        Play(board, 0, 0);
      }
      else if(s == 2) { 
        Piece *board = MakeBoard(pieces);
        for(i = 16; i < 48; i++) 
          WipePiece(board, i);
        Play(board, 0, 1);
      }
      else if(s == 3) {
        Piece *board = MakeBoard(pieces);
        for(i = 16; i < 48; i++) 
          WipePiece(board, i);
        Play(board, 1, 0);
      }        
      else if(s == 4) {
        Piece *board = MakeBoard(pieces);
        for(i = 16; i < 48; i++) 
          WipePiece(board, i);
        Play(board, 1, 1);
      }        
      else if(s == 5)
        Help();
      else if(s == 6)
        Credits();
      else if(s == 7) {
        printf("Quitting\n");
        fclose(fp);
        return 0;
      }
      else
        printf("Invalid input, enter a number 1-6!\n");
    }
    return 0;
}









