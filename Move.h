#ifndef MOVE_H_INCLUDED_
#define MOVE_H_INCLUDED_

#include "ScreenDisplay.h"
#include <assert.h>

void HumanTurn(Piece *board, char color);
int IsLegalMove(Piece *board, int src, int dest, char attack);
int IsInCheck(Piece *board, char color);
int WillBeInCheck(Piece *board, char color, int src, int dest);
int IsInCheckmate(Piece *board, char color);
void MovePiece(Piece *board, int src, int dest);
void MovePieceThink(Piece *board, int src, int dest);
void MoveFake(Piece *board, int src, int dest);
void WipePiece(Piece *board, int src);
int Coord2Index(char c, char num);
void PrintOut(Piece *board, int src, int dest);
void AskPromotion(Piece *board, int src, int dest);

#endif 

