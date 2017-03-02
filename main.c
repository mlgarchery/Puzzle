// #include <stdio.h>
// #include <math.h>
#include <SDL.h>
#include <time.h>
#include "fonctionPuzzle.h"


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
            	case SDL_QUIT:
            		quit = 1;
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
