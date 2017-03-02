#ifndef FONCTIONPUZZLE_H
#define FONCTIONPUZZLE_H
#include <stdio.h>
#include <time.h>
#include <SDL.h>

typedef struct Piece Piece;
struct Piece{
    SDL_Rect imageDecoupe;
    SDL_Rect screenRect;
    SDL_Rect pieceRectMoving;
    int entierAngle; 
    int isSelected;
};


void initPiece(Piece * piece, SDL_Rect * imageDecoupe, SDL_Rect * screenRect, int entierAngle);

void incrementeEntierAngle(Piece * piece);

void renderPieceTexture(Piece * piece, SDL_Texture * texture, SDL_Renderer * renderer);

void renderPieceSelected(Piece * piece, SDL_Texture * texture, SDL_Renderer * renderer);

void shuffleList(int liste[16]);

int getPiece(int x_souris, int y_souris, Piece listePieces[16]);

void createPuzzle(Piece listePieces[16], int liste_permutation[16]);

void renderPuzzle(Piece listePieces[16], SDL_Texture * texture, SDL_Renderer * renderer);

void switchScreenRect(Piece * piece1, Piece * piece2);

int isInRect(int x_souris, int y_souris, SDL_Rect * rect);

void trouverCase(int x_souris, int y_souris, SDL_Rect * new_rect, SDL_Rect * grille);

int isCaseUsed(SDL_Rect * new_rect, Piece listePieces[16]);

int playerWon(Piece listePieces[16]);


#endif