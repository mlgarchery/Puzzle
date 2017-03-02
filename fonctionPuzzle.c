#include "fonctionPuzzle.h"


void initPiece(Piece * piece, SDL_Rect * imageDecoupe, SDL_Rect * screenRect, int entierAngle){
    /* Découpe un rectangle (imageDecoupe) de l'image et l'associe a 
    un rectangle de l'écran (screenRect) avec une rotation de
    en./puzzletierAngle*90 degrés */

    piece->imageDecoupe = *imageDecoupe; // === (*piece).imageDecoupe.x
    piece->screenRect = *screenRect;
    piece->entierAngle = entierAngle%4; // entierAngle appartient à {0,1,2,3}
    piece->isSelected = 0;
}

void incrementeEntierAngle(Piece * piece){
    piece->entierAngle = (piece->entierAngle+1)%4;
}

void renderPieceTexture(Piece * piece, SDL_Texture * texture, SDL_Renderer * renderer){
    /* Copy sur le renderer la texture associée à la pièce */

    SDL_RenderCopyEx(renderer, texture,  &piece->imageDecoupe, &piece->screenRect, piece->entierAngle*90, NULL, SDL_FLIP_NONE);
}

void renderPieceSelected(Piece * piece, SDL_Texture * texture, SDL_Renderer * renderer){
    /* Copy sur le renderer la texture associée à la pièce selectionnée (potentiellement mouvante) */

    SDL_RenderCopyEx(renderer, texture,  &piece->imageDecoupe, &piece->pieceRectMoving, piece->entierAngle*90, NULL, SDL_FLIP_NONE);
}

void shuffleList(int liste[16]){
    /* Créer une permutation aléatoire de la liste donnée en paramètre */
    
    for(int i=0; i <10; i=i+1){
        // on effectue des swaps
        for(int j=0; j<16; j=j+1){
            int entierAleatoire = rand() % 16;
            int stockvalue = liste[entierAleatoire];
            liste[entierAleatoire] = liste[j];
            liste[j]= stockvalue;
        }
    }
}

int getPiece(int x_souris, int y_souris, Piece listePieces[16]){
    for(int i=0; i<16; i++){
        int x_rect = listePieces[i].screenRect.x;
        int y_rect = listePieces[i].screenRect.y;
        int w_rect = listePieces[i].screenRect.w;
        int h_rect = listePieces[i].screenRect.h;

        if(x_souris >= x_rect && x_souris < x_rect + w_rect &&
            y_souris >= y_rect && y_souris < y_rect + h_rect){
            return i;
        }
    }
    return -1;

}

void createPuzzle(Piece listePieces[16], int liste_permutation[16]){
    /*  */

    for(int i=0; i<16; i++){
        Piece piece;
        SDL_Rect imageDecoupe = { i%4*100, i/4*100, 100, 100 };
        SDL_Rect screenRect = { 40+ liste_permutation[i]%4*100, 25 + liste_permutation[i]/4*100, 100, 100 };
        initPiece(&piece, &imageDecoupe, &screenRect, rand() % 4);
        listePieces[i] = piece;
    }
}

void renderPuzzle(Piece listePieces[16], SDL_Texture * texture, SDL_Renderer * renderer){
    int pieceSelected = -1;
    for (int i = 0; i < 16; i++){
        if(!listePieces[i].isSelected){
            renderPieceTexture(&listePieces[i], texture, renderer);
        }else{
            pieceSelected = i;
        }
    }
    if(pieceSelected != -1){
        renderPieceSelected(&listePieces[pieceSelected], texture, renderer);
    }
}

void switchScreenRect(Piece * piece1, Piece * piece2){
    SDL_Rect tempScreenRect;
    tempScreenRect = piece1->screenRect;
    piece1->screenRect = piece2->screenRect;
    piece2->screenRect = tempScreenRect;
}

int isInRect(int x_souris, int y_souris, SDL_Rect * rect){
    if( x_souris >= rect->x && x_souris < rect->x + rect->w && y_souris >= rect->y && y_souris < rect->y + rect->h){
        return 1;
    }
    return 0;
}

void trouverCase(int x_souris, int y_souris, SDL_Rect * new_rect, SDL_Rect * grille){
    new_rect->x = (x_souris -grille->x)/100 * 100 + grille->x;
    new_rect->y = (y_souris -grille->y)/100 *100 + grille->y;
    new_rect->w =100;
    new_rect->h =100;
}

int isCaseUsed(SDL_Rect * new_rect, Piece listePieces[16]){
    for(int i=0; i<16; i++){
        if(listePieces[i].screenRect.x == new_rect->x && listePieces[i].screenRect.y == new_rect->y){
            return i;
        }
    }
    return -1;
} 

int playerWon(Piece listePieces[16]){
    for (int i=0; i<16; i++){
        if(listePieces[i].imageDecoupe.x != listePieces[i].screenRect.x-460
        || listePieces[i].imageDecoupe.y != listePieces[i].screenRect.y-25
        || listePieces[i].entierAngle !=0){
            return 0;
        }
    }
    return 1;
}
