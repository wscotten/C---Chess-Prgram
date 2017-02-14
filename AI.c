#include <stdio.h>
#include <stdlib.h>

#include "AI.h"


void AITurn(Piece *board, char color) {
  int src, dest, i;
  double maxsum = -10;
  int SI = 0;
  int src2, dest2, x = 0;
  
  for(src2 = 0; src2 < 64; src2++){
    if(x == 1)
      break;
    if(board[src2].color != color)
      continue;
    for(dest2 = 0; dest2< 64; dest2++){
      if(IsLegalMove(board, src2, dest2, 'a') && WillBeInCheck(board, color, src2, dest2) == 0){
        x = 1;
        break;
      }
    }
  }
  if(x == 0){
    board[0].name = 'z';
    return;
  }
  
  if(Checkmate(board, color)){
    return;
  }
  for(i = 0; i < 64; i++){
    board[i].moveindex = 0;
    board[i].movevalue = 0;
  }
  int count = 0;
  for(i = 0; i < 64; i++){
    if(board[i].color != color && board[i].color != ' '){
      count++;
    }
  }
  if(count < 6) {
    for(i = 0; i < 64; i++){
      if(board[i].color == color && board[i].name == 'p'){
        board[i].movevalue += 3;
      }
    }
  }
  for(src = 0; src < 64; src++){
    if(board[src].color != color)
      continue;
    for(dest = 0; dest < 64; dest++){
      if(IsLegalMove(board, src, dest, 'a') && WillBeInCheck(board, color, src, dest) == 0 && Stalemate(board, src, dest) == 0 && board[src].lastspot != dest){
        if(((dest <= 7 && board[src].color == 'b') || (dest >= 56 && board[src].color == 'w')) && board[src].name == 'p'){
          board[src].movevalue += 9;
        }
        board[src].movevalue += Calculate(board, src, dest);
        if(board[src].movevalue > maxsum){
          board[src].moveindex = dest;
          SI = src;
          maxsum = board[src].movevalue;
        }
        board[src].movevalue = 0;  
      }
    }
  }
  #ifdef debug
  printf("   %.2f    ", maxsum);
  printf("\n%d %d", SI, board[SI].moveindex);
  printf("\ncap %.1f prot %.1f threat %.1f check %.1f ret %.1f\n", Capture(board, SI, board[SI].moveindex), Protect(board, SI, board[SI].moveindex), Threat(board, SI, board[SI].moveindex), Check(board, SI, board[SI].moveindex), 
  Retaliate(board, SI, board[SI].moveindex));
  #endif
  
  if(maxsum < 1 && IsInCheck(board, color) == 0){
    Opener(board, color);
  }
  else{
   if(board[SI].moveindex >= 56 && board[SI].color == 'w' && board[SI].name == 'p'){
     board[SI].name = 'q';
     board[SI].value = 9;
   }   
     /*check blacsk pawn promotion*/
   if(board[SI].moveindex <= 7 && (board[SI].color == 'b') && (board[SI].name == 'p')){
     board[SI].name = 'q';
     board[SI].value = 9;
   }    
   MovePiece(board, SI, board[SI].moveindex);
   return;
 }
}

double Calculate(Piece *board, int src, int dest){
  return Capture(board, src, dest)+ Protect(board, src, dest) + Threat(board, src, dest) + Check(board, src, dest) + Retaliate(board, src, dest);
}

/* Checks to see if they can checkmate */
int Checkmate(Piece *board, char color){
  char opp;
  if(color == 'w')
    opp = 'b';
  else
    opp = 'w';
  int src, dest;
  for(src = 0; src < 64; src++){
    if(board[src].color != color)
      continue;
    for(dest = 0; dest < 64; dest++){
      if(IsLegalMove(board, src, dest, 'a') && WillBeInCheck(board, color, src, dest) == 0){
        Piece temp_src = board[src];
        Piece temp_dest = board[dest];
        MoveFake(board, src, dest);
        if (IsInCheckmate(board, opp)){
          board[src] = temp_src;
          board[dest] = temp_dest;
          MovePiece(board, src, dest);
          return 1;
        }
        board[src] = temp_src;
        board[dest] = temp_dest;
      }
    }
  }
  return 0;
}
/* Checks to see if they can put the king in check */
double Check(Piece *board, int src, int dest){
  double x = 0;
  char opp;
  if(board[src].color == 'w')
    opp = 'b';
  else
    opp = 'w';
  Piece temp_src = board[src];
  Piece temp_dest = board[dest];
  MoveFake(board, src, dest);
  if (IsInCheck(board, opp)){
    x = .5;
  }
  board[src] = temp_src;
  board[dest] = temp_dest;
  return x;
}
/* Checks to see if they can capture a piece */
double Capture(Piece *board, int src, int dest){
  char opp;
  if(board[src].color == 'w')
    opp = 'b';
  else
    opp = 'w';
  if(IsLegalMove(board, src, dest, 'a') && WillBeInCheck(board, board[src].color, src, dest) == 0 && board[dest].color == opp){
    return(board[dest].value);
  }
  return 0;
}
/* checks if it can capture any pieces after the move */
double Threat(Piece *board, int src, int dest){
  int i;
  double sum = 0;
  Piece temp_src = board[src];
  Piece temp_dest = board[dest];
  MoveFake(board, src, dest);
  for(i = 0; i < 64; i++){
    if(Capture(board, dest, i) > 0 && Protected(board, i) == 0){
      sum += board[i].value/3;
    }
  }
  board[src] = temp_src;
  board[dest] = temp_dest;
  return sum;
}
/* Checks to see if they can protect a piece that is threatened */
double Protect(Piece *board, int src, int dest){
  double sum = 0;
  double temp = 0;
  int i, j;
  if(Protected(board, src) == 0){
    for(i = 0; i < 64; i++){
      if(Capture(board, i, src) > 0){
        Piece temp_src = board[src];
        Piece temp_dest = board[dest];
        MoveFake(board, src, dest);
        for(j = 0; j < 64; j++){
          if(Capture(board, j, dest) > 0){
            break;
          }
        }
        board[src] = temp_src;
        board[dest] = temp_dest;
        if(j == 64)
          return board[src].value;
      }
    }
  }
  for(i = 0; i < 64; i++){
    if(board[i].color != board[src].color){
      continue;
    }
    else {
      if(Protected(board, i) == 0){
        for(j = 0; j < 64; j++) {
          if(Capture(board, j, i) > 0){
            Piece temp_src = board[src];
            Piece temp_dest = board[dest];
            MoveFake(board, src, dest);
            temp = Protected(board, i);
            if(temp > 0 && board[j].value > sum){
              sum = board[i].value/3;
            }
            board[src] = temp_src;
            board[dest] = temp_dest;
          }
        }
      }
    }
  }
  return sum;
}  
/* checks to see if a piece is protected */
double Protected(Piece *board, int src){
  double sum = 0;
  int i;
  for(i = 0; i < 64; i++){
    if(board[i].color != board[src].color){
      continue;
    }
    if(IsLegalMove(board, i, src, 'p') && WillBeInCheck(board, board[src].color, i, src) == 0){
      return board[src].value;
    }
  }
  return sum;
}  

double Retaliate(Piece *board, int src, int dest)
{
 int k;
 Piece temp_src = board[src];
 Piece temp_dest = board[dest];
 MoveFake(board, src, dest);
 int retaliatePoints = 0;

 for(k = 0; k < 64; k++){
	if(board[k].color == board[dest].color)
     continue;
   if(IsLegalMove(board, k, dest, 'a') == 1){
      if(Protected(board, dest) == 0){
        if(retaliatePoints > board[dest].value*-1){
          retaliatePoints = board[dest].value*-1;
        }
      }
      else{
        if((board[dest].value - board[k].value) > 0){
          if(retaliatePoints > board[k].value - board[dest].value){
            retaliatePoints = board[k].value - board[dest].value;
          }
        }
      }
	  }
  }
 board[src] = temp_src;
 board[dest] = temp_dest;
 return retaliatePoints;
}

void Opener(Piece* board, char color){
  int whitesrc[] = {11, 12, 5, 6, 4};
  int whitedest[] = {27, 20, 19, 21, 6};
  int blacksrc[] = {51, 52, 61, 62, 60};
  int blackdest[] = {35, 44, 43, 45, 62};
  int i, j, x = 0;
  for(i = 0; i < 5; i++){
    if(color == 'w'){
      if(IsLegalMove(board, whitesrc[i], whitedest[i], 'a') && WillBeInCheck(board, color, whitesrc[i], whitedest[i]) == 0 && Calculate(board, whitesrc[i], whitedest[i]) >= 0){
        MovePiece(board, whitesrc[i], whitedest[i]);
        return;
      }
    }
    else {
      if(IsLegalMove(board, blacksrc[i], blackdest[i], 'a') && WillBeInCheck(board, color, blacksrc[i], blackdest[i]) == 0 && Calculate(board, blacksrc[i], blackdest[i]) >= 0){
        MovePiece(board, blacksrc[i], blackdest[i]);
        return;
      }
    }
  } 
  for(i = 0; i < 64; i++){ 
    if(board[i].color != color)
      continue;
    if( x == 1)
      break;
    for(j = 0; j < 64; j++){
      if(IsLegalMove(board, i, j, 'a') && WillBeInCheck(board, color, i, j) == 0 && Stalemate(board, i, j) == 0 && Calculate(board, i, j) >= 0){
        x = 1;
        break;
      }
    }
  }
  if( x == 0){
    for( i = 0; i < 64; i++){
      for(j = 0; j < 64; j++){
        if(board[i].color == color && IsLegalMove(board, i, j, 'a') && WillBeInCheck(board, color, i, j) == 0 && Stalemate(board, i, j) == 0){
          MovePiece(board, i, j);
          return;
        }
      }
    }
  }
  while(1){
    int src = rand() % 64; 
    if(board[src].color != color)
      continue;
    if(board[src].name == 'p'){
      for(i = 0; i < 64; i++){
        if(IsLegalMove(board, src, i, 'a') && WillBeInCheck(board, color, src, i) == 0 && Stalemate(board, src, i) == 0 && Calculate(board, src, i) >= 0){
          MovePiece(board, src, i);
          return;
        }
      }
    }
    int dest = rand() % 64;
    if(IsLegalMove(board, src, dest, 'a') && WillBeInCheck(board, color, src, dest) == 0 && Stalemate(board, src, dest) == 0 && Calculate(board, src, dest) >= 0){
        MovePiece(board, src, dest);
        return;
    }
  }
}

int Stalemate(Piece *board, int src, int dest){
  int x = 1;
  char oppcolor;
  if(board[src].color == 'w')
    oppcolor = 'b';
  else
    oppcolor = 'w';
  Piece temp_src = board[src];
  Piece temp_dest = board[dest];
  MoveFake(board, src, dest);  
  if(IsInCheck(board, oppcolor) == 1)
    x = 0;
  int i, j;
  for(i = 0; i < 64; i++){
    if(x == 0)
      break;
    if(board[i].color != oppcolor)
      continue;
    for(j = 0; j < 64; j++){
      if(IsLegalMove(board, i, j, 'a') && WillBeInCheck(board, oppcolor, i, j) == 0){
        x = 0;
      } 
    }
  }
  board[src] = temp_src;
  board[dest] = temp_dest;
  return x;
}



























