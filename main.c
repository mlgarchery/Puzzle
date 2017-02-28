// #include <stdio.h>
// #include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>
    
typedef struct Piece Piece;
struct Piece{
    SDL_Rect imageDecoupe;
    SDL_Rect screenRect;
    int entierAngle; 
};

void initPiece(Piece * piece, SDL_Rect * imageDecoupe, SDL_Rect * screenRect, int entierAngle){
    /* Découpe un rectangle (imageDecoupe) de l'image et l'associe a 
    un rectangle de l'écran (screenRect) avec une rotation de
    entierAngle*90 degrés */

    piece->imageDecoupe = *imageDecoupe; // === (*piece).imageDecoupe.x
    piece->screenRect = *screenRect;
    piece->entierAngle = entierAngle%4; // entierAngle appartient à {0,1,2,3}
}

void renderPieceTexture(Piece * piece, SDL_Texture * texture, SDL_Renderer * renderer){
    /* Copy sur le renderer la texture associée à la pièce */

    SDL_RenderCopyEx(renderer, texture,  &piece->imageDecoupe, &piece->screenRect, piece->entierAngle*90, NULL, SDL_FLIP_NONE);
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
    for (int i = 0; i < 16; i++){
        renderPieceTexture(&listePieces[i], texture, renderer);
    }    

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
        int pos_image = -1;
        int pos_screen = -1;

        pos_image = listePieces[i].imageDecoupe.x/100 + (listePieces[i].imageDecoupe.y/100)*4;
        pos_screen = (listePieces[i].screenRect.x-460)/100 + ((listePieces[i].screenRect.y-25)/100)*4;

        if(pos_image != pos_screen){
            return 0;
        }
        if(listePieces[i].entierAngle !=0){
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
    SDL_Surface * image = SDL_LoadBMP("images/0/slardar.bmp");
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

    // Création du puzzle (grille gauche)
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
    int numPiece = -1;
    
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

                    if(numPiece != -1){
                        SDL_Rect rectMouvement;
                        rectMouvement.x = x_souris - listePieces[numPiece].screenRect.w/2;
                        rectMouvement.y = y_souris - listePieces[numPiece].screenRect.h/2;
                        rectMouvement.w = listePieces[numPiece].screenRect.w;
                        rectMouvement.h = listePieces[numPiece].screenRect.h;

                        printf("%d,\n", rectMouvement.x );

                        initPiece(&listePieces[numPiece], 
                                    &listePieces[numPiece].imageDecoupe, 
                                    &rectMouvement, 
                                    listePieces[numPiece].entierAngle);
                    }

                case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
                    if(event.button.button == SDL_BUTTON_LEFT)  // check if it is in the desired area
                    {
                        if(numPiece == -1){
                            if(event.button.clicks !=0){
                                numPiece = getPiece(x_souris, y_souris, listePieces);
                            }
                        }else{
                            if(event.button.clicks !=0){

                                SDL_Rect new_rect;
                                int modif = 0;
                                if(isInRect(x_souris, y_souris, &grilleGauche)){
                                    trouverCase(x_souris, y_souris, &new_rect, &grilleGauche);
                                    modif = 1;
                                }else if(isInRect(x_souris, y_souris, &grilleDroite)){
                                    trouverCase(x_souris, y_souris, &new_rect, &grilleDroite);
                                    modif = 1;
                                }
                                if(modif && isCaseUsed(&new_rect, listePieces) == -1){

                                    initPiece(&listePieces[numPiece], 
                                    &listePieces[numPiece].imageDecoupe, 
                                    &new_rect, 
                                    listePieces[numPiece].entierAngle);
                                    numPiece = -1;      

                                }else if(modif && isCaseUsed(&new_rect, listePieces) != -1){
                                    printf("%d \n", isCaseUsed(&new_rect, listePieces) );
                                    Piece temp;
                                    temp = listePieces[isCaseUsed(&new_rect, listePieces)];
                                    listePieces[isCaseUsed(&new_rect, listePieces)] = listePieces[numPiece];
                                    listePieces[numPiece] = temp;
                                    numPiece = -1;
                                }
                                numPiece = -1;

                            }
                        }
                    }else if(event.button.button == SDL_BUTTON_RIGHT){
                        numPiece = getPiece(x_souris, y_souris, listePieces);
                        if(event.button.clicks !=0){
                            if(numPiece != -1){
                                initPiece(&listePieces[numPiece], 
                                    &listePieces[numPiece].imageDecoupe, 
                                    &listePieces[numPiece].screenRect, 
                                    listePieces[numPiece].entierAngle + 1);
                                numPiece = -1;
                            }
                        }else{
                            numPiece = -1;
                        }
                    }
            }
        }

        // Tout clear en noir
        SDL_SetRenderDrawColor( renderer, 0, 
        0, 0, 255);
        SDL_RenderClear(renderer);
        // Afficher les grille gauche et droite
        SDL_SetRenderDrawColor( renderer, 242, 
        227, 198, 255); 
        SDL_RenderFillRect(renderer, &grilleGauche);
        SDL_RenderFillRect(renderer, &grilleDroite);
        // Afficher le puzzle
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
        SDL_Delay(8000);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit();
    return 0;
}
