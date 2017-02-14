#ifndef SCREENDISPLAY_H_INCLUDED_
#define SCREENDISPLAY_H_INCLUDED_

#include <unistd.h>

typedef struct Pieces {
    char letPos;
    int numPos;
    char color;
    char name;
    int value;
    double movevalue;
    int moveindex;
    int hasmoved;
    int lastspot;
    char enpassant;
} Piece;

Piece *MakeBoard(Piece *pieces);
void Play(Piece *board, char w, char b);
void Display(Piece *board);
void PrintMenu();
void Help();
void Credits();
void Intro();

#endif 
