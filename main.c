// #include <stdio.h>
// #include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>
    
typedef struct Piece Piece;
struct Piece{
    SDL_Rect imageDecoupe;
    SDL_Rect screenRect;
    SDL_Rect pieceRectMoving;
    int entierAngle; 
    int isSelected;
};

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


int main()
{
    int quit = 0 ;
    srand(time(NULL)); // on donne une nouvelle graine au générateur aléatoire
 
    SDL_Init(SDL_INIT_VIDEO);
    int size_x= 900;
    int size_y= 450;
    SDL_Window * window = SDL_CreateWindow("Puzzle", 100, 100, size_x, size_y, SDL_WINDOW_SHOWN);
    if (window == NULL){
        printf( "SDL_CreateWindow Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return 1;
    }
 
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        printf( "SDL_CreateRenderer Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // On charge l'image
    SDL_Surface * image = SDL_LoadBMP("images/slardar.bmp");
    if (image == NULL){
        printf( "SDL_LoadBMP Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }
    // On crée une texture avec cette image
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Création d'une permutation de pièce
    int liste_permutation[16];
    for(int i=0; i<16; i=i+1){
        liste_permutation[i] = i;
    }
    shuffleList(liste_permutation);

    Piece listePieces[16];
    createPuzzle(listePieces, liste_permutation);

    // grille gauche
    SDL_Rect grilleGauche;
    grilleGauche.x = 40;
    grilleGauche.y = 25;
    grilleGauche.w = 400;
    grilleGauche.h = 400; 

    // grille droite, de résolution
    SDL_Rect grilleDroite;
    grilleDroite.x = 460;
    grilleDroite.y = 25;
    grilleDroite.w = 400;
    grilleDroite.h = 400; 

    int x_souris = -1;
    int y_souris = -1;
    int pieceSelected = -1;
    
    SDL_Event event;

    while (!quit)
    {

        while(SDL_PollEvent(&event)) // check to see if an event has happened
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        quit = 1 ;
                    }
                case SDL_MOUSEMOTION:
                    x_souris = event.button.x;
                    y_souris = event.button.y;

                    if(pieceSelected != -1){
                        SDL_Rect rectMouvement;
                        rectMouvement.x = x_souris - listePieces[pieceSelected].screenRect.w/2;
                        rectMouvement.y = y_souris - listePieces[pieceSelected].screenRect.h/2;
                        rectMouvement.w = listePieces[pieceSelected].screenRect.w;
                        rectMouvement.h = listePieces[pieceSelected].screenRect.h;

                        listePieces[pieceSelected].pieceRectMoving = rectMouvement;
                        listePieces[pieceSelected].isSelected = 1;
                    }

                case SDL_MOUSEBUTTONDOWN: // on détecte un enfoncement de la souris

                    if(event.button.clicks !=0){ //si c'est un click ..
                        
                        if(event.button.button == SDL_BUTTON_RIGHT){ // .. droit
                            int numPieceRightClicked;
                            if(pieceSelected == -1){
                                numPieceRightClicked = getPiece(x_souris, y_souris, listePieces);
                            }else{
                                numPieceRightClicked = pieceSelected;
                            }
                            if(numPieceRightClicked != -1){
                                incrementeEntierAngle(&listePieces[numPieceRightClicked]);
                            }
                        }else if(event.button.button == SDL_BUTTON_LEFT){ // .. gauche
                            SDL_Rect new_rect;
                            int isInGrille = 0;
                            if(isInRect(x_souris, y_souris, &grilleGauche)){
                                trouverCase(x_souris, y_souris, &new_rect, &grilleGauche);
                                isInGrille = 1;
                            }else if(isInRect(x_souris, y_souris, &grilleDroite)){
                                trouverCase(x_souris, y_souris, &new_rect, &grilleDroite);
                                isInGrille = 1;
                            }
                            if(isInGrille){
                                if(pieceSelected == -1){
                                    pieceSelected = getPiece(x_souris, y_souris, listePieces);
                                }else{
                                    if(isInGrille && isCaseUsed(&new_rect, listePieces) == -1){
                                        // On place la piece sur la case vide
                                        listePieces[pieceSelected].screenRect = new_rect; 
                                        listePieces[pieceSelected].isSelected = 0;
                                        pieceSelected = -1;      

                                    }else if(isInGrille && isCaseUsed(&new_rect, listePieces) != -1){
                                        // On échange la position des deux pièces
                                        switchScreenRect(&listePieces[isCaseUsed(&new_rect, listePieces)], 
                                                         &listePieces[pieceSelected]);

                                        listePieces[pieceSelected].isSelected = 0;
                                        pieceSelected = -1;
                                    }
                                    pieceSelected = -1;
                                }
                            }
                        }
                    }
            }
        }

        // Tout clear en noir
        SDL_SetRenderDrawColor( renderer, 0, 
        0, 0, 255);
        SDL_RenderClear(renderer);
        // Afficher les grilles gauche et droite
        SDL_SetRenderDrawColor( renderer, 242, 
        227, 198, 255); 
        SDL_RenderFillRect(renderer, &grilleGauche);
        SDL_RenderFillRect(renderer, &grilleDroite);
        SDL_SetRenderDrawColor( renderer, 0, 
        0, 0, 255);
        SDL_Rect  colonne;
        SDL_Rect ligne;
        for(int i=0;i<6;i++){
            int decalage_x = 40;
            int decalage_y = 25;
            if(i/3>0){
                decalage_x = 460;
            }
            colonne.x = decalage_x +100*(i%3+1);
            colonne.y = decalage_y;
            colonne.w = 2;
            colonne.h = 400;

            ligne.x = decalage_x;
            ligne.y = decalage_y +100*(i%3+1);
            ligne.w = 400;
            ligne.h = 2;
            SDL_RenderFillRect(renderer, &colonne);    
            SDL_RenderFillRect(renderer, &ligne);    
        }
        // Afficher la liste des pièces
        renderPuzzle(listePieces, texture, renderer);

        if(playerWon(listePieces)){
            quit = 1;
            SDL_Surface * imageYouWon = SDL_LoadBMP("images/you_won.bmp");
            SDL_Texture * textureWon = SDL_CreateTextureFromSurface(renderer, imageYouWon);
            SDL_RenderCopy(renderer, textureWon, NULL, &grilleGauche);
            SDL_RenderPresent(renderer); 
        }
        SDL_RenderPresent(renderer);    
    }

    if(playerWon(listePieces)){
        // laisser au joueur le temps d'aprécier sa victoire
        SDL_Delay(8000);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit();
    return 0;
}
