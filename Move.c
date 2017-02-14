#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

#include "Move.h"

/*#define debug*/

void HumanTurn(Piece *board, char color){
    char piecestr[3];
    char deststr[3];
    int src, dest;
    
  if(color == 'w')
    printf("\n** WHITE'S TURN **\n");
  else
    printf("\n** BLACK'S TURN **\n");
  while(1) {
    printf("Enter piece to move (ff to forfeit):");
    scanf("%2s", piecestr);
    src = Coord2Index(piecestr[0], piecestr[1]);
    if (src == 429) {
        board[0].name = 'z';
        break;
    }
    else if (piecestr[0] < 'a' || piecestr[0] > 'h' || piecestr[1] < '1' || piecestr[1] > '8') {
        printf("Invalid input, please try again!\n");
        continue;
    }
    else if(board[src].color != color) {
          printf("Please select a piece of your color!\n");
          continue;
    }
    else {
        printf("Enter square to move to:");
        scanf("%2s", deststr);
        dest = Coord2Index(deststr[0], deststr[1]);
        if (deststr[0] < 'a' || deststr[0] > 'h' || deststr[1] < '1' || deststr[1] > '8') {
            printf("Invalid input, please try again!\n");
            continue;
        }
        else if(IsLegalMove(board, src, dest, 'a') && WillBeInCheck(board, color, src, dest) == 0) {
        	/*check white pawn promotion*/
	      if((dest >= 56) && (dest <= 63) && (board[src].color == 'w') && (board[src].name == 'p'))
	      {
		      AskPromotion(board, src, dest);
	      }   
	      /*check black pawn promotion*/
	      if((dest >= 0) && (dest <= 7) && (board[src].color == 'b') && (board[src].name == 'p'))
	      {
		      AskPromotion(board, src, dest);
	      }   
            	MovePiece(board, src, dest);
            	break;
        }
	      else {
	      printf("Invalid move, please try again!\n");
	      }
    }
  }
}


int IsLegalMove(Piece *board, int src, int dest, char attack)
{ 
  if (src == dest)
    return 0;
  if(attack == 'a' && board[src].color == board[dest].color){
    return 0;
  } 
  if(attack == 'p' && board[src].color != board[dest].color) {
    return 0;
  }
 int i;
    if(board[src].name == 'r') {
      if(board[src].letPos == board[dest].letPos) {
        if(src < dest){
          while((src += 8) < dest) {
           if(board[src].name != ' ') {
             return 0;
           }
          }
        }
        else {   
          while((src -= 8) > dest) {
           if(board[src].name != ' ') {
             return 0;
           }
          }
        }
        return 1;
      }
      else if(board[src].numPos == board[dest].numPos){
        if(src < dest){
        while((src += 1) < dest) {
           if(board[src].name != ' ') {
             return 0;
           }
         }
        }
        else {   
          while((src -= 1) > dest) {
           if(board[src].name != ' ') {
             return 0;
           }
         }
        }
        return 1;
      }
      else {
        return 0;
      }
   }
   else if(board[src].name == 'n') {
     if((abs((int)(board[src].letPos - board[dest].letPos)) == 2 && abs(board[src].numPos - board[dest].numPos) == 1) || (abs((int)(board[src].letPos - board[dest].letPos)) == 1 && abs(board[src].numPos - board[dest].numPos) == 2)){
         return 1;
     }
     else
       return 0;
   }
   else if(board[src].name == 'b') {
     if((int)abs(board[src].letPos-board[dest].letPos) != abs(board[src].numPos-board[dest].numPos)) {
       return 0;
       }
     if(board[src].numPos < board[dest].numPos && board[src].letPos < board[dest].letPos) {
       while((src += 9) < dest) {
         if(board[src].name != ' ') {
           return 0;
         }
       }
     }
     if(board[src].numPos < board[dest].numPos && board[src].letPos > board[dest].letPos) {
       while((src += 7) < dest) {
         if(board[src].name != ' ') {
           return 0;
         }
       }
     }
     if(board[src].numPos > board[dest].numPos && board[src].letPos > board[dest].letPos) {
       while((src -= 9) > dest) {
         if(board[src].name != ' ') {
           return 0;
         }
       }
     }
     if(board[src].numPos > board[dest].numPos && board[src].letPos < board[dest].letPos) {
       while((src -= 7) > dest) {
         if(board[src].name != ' ') {
           return 0;
         }
       }
     }
     return 1;
   }
   else if(board[src].name == 'p') {
     if(board[src].color == 'w') {
       if(src + 8 == dest && board[src+8].name == ' '){
         return 1;
       }
       /*pawn can jump 2 squares for first move*/
       if (src + 16 == dest && board[src].hasmoved == 0 && board[src+8].name == ' ' && board[src+16].name == ' ') {
         return 1;
       }
       if(board[src].numPos + 1 == board[dest].numPos && abs(board[src].letPos - board[dest].letPos) == 1){
         if(board[dest].name != ' '){
           return 1;
         }
         if(board[dest].enpassant != ' ' && board[src].name != board[dest].enpassant){
           board[src].movevalue += 5;
           return 1;
         }
       }
     }
     if(board[src].color == 'b') {
       if(src - 8 == dest && board[src-8].name == ' '){
         return 1;
       }
       if(src - 16 == dest && board[src].hasmoved == 0 && board[src-8].name == ' ' && board[src-16].name == ' ') {
         return 1;
       }
       if(board[src].numPos - 1 == board[dest].numPos && abs(board[src].letPos - board[dest].letPos) == 1){
         if(board[dest].name != ' '){
           return 1;
         }
         if(board[dest].enpassant != ' ' && board[src].name != board[dest].enpassant){
           board[src].movevalue += 5;
           return 1;
         }  
       }
     }
     return 0;
   } 
   else if(board[src].name == 'q') {
     if(board[src].letPos == board[dest].letPos) {
        if(src < dest){
          for(i = src+8; i < dest; i+=8){
            if(board[i].name != ' '){
                  return 0;
                }
            }
        }
        else {   
          for(i = src-8; i > dest; i-=8){
            if(board[i].name != ' '){
                return 0;
            }
          }
        }
        return 1;
      }
     else if(board[src].numPos == board[dest].numPos){
        if(src < dest){
          for(i = src+1; i < dest; i++){
            if(board[i].name != ' '){
              return 0;
            }
          }
        }
        else{   
          for(i = src-1; i > dest; i--){
            if(board[i].name != ' '){
              return 0;
            }
          }
        }
        return 1;
      }
     else if(abs(board[src].letPos-board[dest].letPos) == abs(board[src].numPos-board[dest].numPos)) {
       if(board[src].numPos < board[dest].numPos && board[src].letPos < board[dest].letPos) {
         while((src += 9) < dest) {
           if(board[src].name != ' ') {
             return 0;
           }
         }
       }
       if(board[src].numPos < board[dest].numPos && board[src].letPos > board[dest].letPos) {
         while((src += 7) < dest) {
           if(board[src].name != ' ') {
             return 0;
           }
         }
       }
       if(board[src].numPos > board[dest].numPos && board[src].letPos > board[dest].letPos) {
         while((src -= 9) > dest) {
           if(board[src].name != ' ') {
             return 0;
           }
         }
       }
       if(board[src].numPos > board[dest].numPos && board[src].letPos < board[dest].letPos) {
         while((src -= 7) > dest) {
           if(board[src].name != ' ') {
             return 0;
           }
         }
       }
       return 1;
     }
   }
   
   
   else if(board[src].name == 'k') {
     if(abs(board[src].letPos-board[dest].letPos) < 2 && abs(board[src].numPos-board[dest].numPos) < 2) {
       return 1;
     }
     if(board[src].color == 'w' && board[src].hasmoved == 0) {
       if(dest == 2 && board[0].name == 'r' && board[0].hasmoved == 0 && board[0].color == 'w' && WillBeInCheck(board, 'w', src, 3) == 0 && WillBeInCheck(board, 'w', src, 2) == 0 && board[1].name == ' ' && board[2].name == ' ' && board[3].name == ' ') {
         board[src].movevalue += 2;
         return 1;
       }
       if(dest == 6 && board[7].name == 'r' && board[7].hasmoved == 0 && board[7].color == 'w' && WillBeInCheck(board, 'w', src, 5) == 0 && WillBeInCheck(board, 'w', src, 6) == 0  && board[5].name == ' ' && board[6].name == ' ') {
         board[src].movevalue += 2;
         return 1;
       }
     }
     if(board[src].color == 'b' && board[src].hasmoved == 0) {
       if(dest == 58 && board[56].name == 'r' && board[56].hasmoved == 0 && board[56].color == 'b' && WillBeInCheck(board, 'b', src, 59) == 0 && WillBeInCheck(board, 'b', src, 58) == 0 && board[57].name == ' ' && board[58].name == ' ' && board[59].name == ' ') {
         board[src].movevalue += 2;
         return 1;
       }
       if(dest == 62 && board[63].name == 'r' && board[63].hasmoved == 0 && board[63].color == 'b' && WillBeInCheck(board, 'b', src, 61) == 0 && WillBeInCheck(board, 'b', src, 62) == 0  && board[61].name == ' ' && board[62].name == ' ') {
         board[src].movevalue += 2;
         return 1;
       }
     }
     return 0;
   }
  return 0;
}

int IsInCheck(Piece *board, char color){
  if(board[0].name == 'z')
    return 1;
  int i;
  int kingindex = 0;
  for(i = 0; i < 64; i++) {
    if(board[i].name == 'k' && board[i].color == color) {
      kingindex = i;
      break;
    }
  }
  for(i = 0; i < 64; i++) {
    if(IsLegalMove(board, i, kingindex, 'a')) {
      return 1; 
    }
  }
  return 0;
}

int WillBeInCheck(Piece *board, char color, int src, int dest) {
  Piece temp_src = board[src];
  Piece temp_dest = board[dest];
  MoveFake(board, src, dest);
  int x = IsInCheck(board, color);
  board[src] = temp_src;
  board[dest] = temp_dest;
  return x;
}

int IsInCheckmate(Piece *board, char color){
  if(board[0].name == 'z')
    return 2;
  int src, dest;
  for(src = 0; src < 64; src++) {
    if(board[src].color != color)
      continue;
    for(dest = 0; dest < 64; dest++) {
      if(IsLegalMove(board, src, dest, 'a') && WillBeInCheck(board, color, src, dest) == 0) {
        return 0;
      }
    }
  }
  return 1;
}

void MovePiece(Piece *board, int src, int dest)
{
  PrintOut(board, src, dest);
  int i;
  if(board[src].name == 'p' && board[src].color == 'w' && board[dest].enpassant == 'b'){
      WipePiece(board, dest-8);
  }
  if(board[src].name == 'p' && board[src].color == 'b' && board[dest].enpassant == 'w'){
      WipePiece(board, dest+8);
  }
   for(i = 0; i < 64; i++) {
     board[i].enpassant = ' ';
   }
  if(board[src].name == 'p' && abs(src-dest) == 16) {
    if(board[src].color == 'w') {
      board[src+8].enpassant = 'w';
    }
    if(board[src].color == 'b') {
      board[src-8].enpassant = 'b';
    }
  }
  if(board[src].name == 'k' && abs(src - dest) == 2){
    if(dest == 2){
      MovePiece(board, 0, 3);
      MoveFake(board, 4, 2);
      return;
    }
    if(dest == 6){
      MovePiece(board, 7, 5);
      MoveFake(board, 4, 6);
      return;
    }
    if(dest == 58){
      MovePiece(board, 56, 59);
      MoveFake(board, 60, 58);
      return;
    }
    if(dest == 62){
      MovePiece(board, 63, 61);
      MoveFake(board, 60, 62);
      return;
    }
  }
  /*execute move*/
  board[dest].color = board[src].color;
  board[dest].name = board[src].name;
  board[dest].value = board[src].value;
  board[dest].moveindex = board[src].movevalue;
  board[dest].movevalue = board[src].moveindex;
  board[dest].hasmoved = 1;
  board[dest].lastspot = src;
  WipePiece(board, src);
}
void MoveFake(Piece *board, int src, int dest)
{
  board[dest].color = board[src].color;
  board[dest].name = board[src].name;
  board[dest].value = board[src].value;
  board[dest].moveindex = board[src].movevalue;
  board[dest].movevalue = board[src].moveindex;
  board[dest].hasmoved = 1;
  board[dest].lastspot = src;
  WipePiece(board, src);
}
int Coord2Index(char c, char num)
{
 int index = (c - 'a') + (num - '1')*8;
 return index;  
}

void WipePiece(Piece *board, int index)
{
 board[index].color = ' ';
 board[index].name = ' ';
 board[index].value = 0;
 board[index].movevalue = 0;
 board[index].moveindex = 0; 
 board[index].hasmoved = 0;
 board[index].enpassant = ' ';
}

void PrintOut(Piece *board, int src, int dest) {

	FILE *fp;
	int x;
	char PieceMove[10];
	char PieceCaptured[10];
	char SrcColor[6];
	
	if(board[src].color == 'w')  
	{
		strcpy(SrcColor, "White");
	}
	if(board[src].color == 'b')  
	{
		strcpy(SrcColor, "Black");
	}
	
	if(board[src].name == 'p')  
	{
		strcpy(PieceMove, "pawn");
	}
	else if(board[src].name == 'n')  
	{
		strcpy(PieceMove, "knight");
	}
	else if(board[src].name == 'b')  
	{
		strcpy(PieceMove, "bishop");
	}
	else if(board[src].name == 'r')  
	{
		strcpy(PieceMove, "rook");
	}
	else if(board[src].name == 'q')  
	{
		strcpy(PieceMove, "queen");
	}
	else if(board[src].name == 'k')  
	{
		strcpy(PieceMove, "king");
	}
	if(board[dest].name == 'p')  
	{
		strcpy(PieceCaptured, "pawn");
	}
	else if(board[dest].name == 'n')  
	{
		strcpy(PieceCaptured, "knight");
	}
	else if(board[dest].name == 'b')  
	{
		strcpy(PieceCaptured, "bishop");
	}
	else if(board[dest].name == 'r')  
	{
		strcpy(PieceCaptured, "rook");
	}
	else if(board[dest].name == 'q')  
	{
		strcpy(PieceCaptured, "queen");
	}
	else if(board[dest].name == 'k')  
	{
		strcpy(PieceCaptured, "king");
	}
	
	if (board[dest].color == ' ')
	{
		x = 0;
	}
	else if (board[src].color != board[dest].color){
		x = 1;
	}
	
	fp = fopen("log.txt", "a+");
	if(!fp) 
	{	
		printf("Error! Cannot open log.txt\n");
		exit(1);
	}	
		
		
	if(x == 0)		/*move without capture*/
	{	
		fprintf(fp, " %s: %s moved from %c%d to %c%d.\n", SrcColor, PieceMove, board[src].letPos, ((int)board[src].numPos), board[dest].letPos, ((int)board[dest].numPos)); 	/*write move to file*/
		printf(" %s: %s moved from %c%d to %c%d.\n", SrcColor, PieceMove, board[src].letPos, ((int)board[src].numPos), board[dest].letPos, ((int)board[dest].numPos));	/*print to console*/
	}
	
	if(x == 1)		/*move with capture*/
	{
		fprintf(fp, " %s: %s moved from %c%d to %c%d and captured a %s.\n", SrcColor, PieceMove, board[src].letPos, ((int)board[src].numPos), board[dest].letPos, ((int)board[dest].numPos), PieceCaptured); 	/*write move to file*/
		printf(" %s: %s moved from %c%d to %c%d and captured a %s.\n", SrcColor, PieceMove, board[src].letPos, ((int)board[src].numPos), board[dest].letPos, ((int)board[dest].numPos), PieceCaptured);
	}		
}

void AskPromotion(Piece *board, int src, int dest) {
	int input;
	printf("\nYour pawn can be promoted to any piece.\n");
	printf(" 1: Queen\n"); 
	printf(" 2: Rook\n"); 
	printf(" 3: Bishop\n"); 
	printf(" 4: Knight\n"); 
	printf(" 5: Do not promote\n"); 
	printf(" Enter the number choice: ");
	while(1) {
		scanf("%d", &input);
		if((input > 5) || (input < 1)) {
			printf("Please enter a valid number.\n");
			continue;
		}
		else{
			if(input == 1){
				board[src].name = 'q';
        board[src].value = 9;
			}
			if(input == 2){
				board[src].name = 'r';
        board[src].value = 5;
			}
			if(input == 3){
				board[src].name = 'b';
        board[src].value = 3;
			}
			if(input == 4){
				board[src].name = 'n';
        board[src].value = 3;
			}
			if(input == 5){
				board[src].name = 'p';
        board[src].value = 1;
			}
		return;
		}
	}		
}

