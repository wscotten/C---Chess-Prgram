#ifndef AI_H_INCLUDED_
#define AI_H_INCLUDED_

#include "Move.h"

void AITurn(Piece *board, char color);

double Calculate(Piece *board, int src, int dest);
void Opener(Piece* board, char color);
int Checkmate(Piece *board, char color);
double Capture(Piece *board, int src, int dest);
double Threat(Piece *board, int src, int dest);
double Protect(Piece *board, int src, int dest);
double Protected(Piece *board, int src);
double Check(Piece *board, int src, int dest);
double Retaliate(Piece *board, int src, int dest);
int Stalemate(Piece *board, int src, int dest);

#endif 