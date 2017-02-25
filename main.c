// #include <stdio.h>
// #include <math.h>
#include <SDL.h>
#include <stdbool.h>
    
typedef struct Piece Piece;
struct Piece{
    SDL_Rect imageDecoupe;
    SDL_Rect screenRect;
    int angle; 
};

void initPiece(Piece * piece, SDL_Rect * imageDecoupe, SDL_Rect * screenRect, int entierAngle){
    /* Découpe un rectangle (imageDecoupe) de l'image et l'associe a 
    un rectangle de l'écran (screenRect) avec une rotation de
    entierAngle*90 degrés */

    piece->imageDecoupe = *imageDecoupe; // === (*piece).imageDecoupe.x
    piece->screenRect = *screenRect;
    piece->angle = entierAngle*90; // entierAngle appartient à {0,1,2,3}
}

void renderPieceTexture(Piece * piece, SDL_Texture * texture, SDL_Renderer * renderer){
    /* Copy sur le renderer la texture associée à la pièce */

    SDL_RenderCopyEx(renderer, texture,  &piece->imageDecoupe, &piece->screenRect, piece->angle, NULL, SDL_FLIP_NONE);
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

void createPuzzle(int liste[16], SDL_Texture * texture, SDL_Renderer * renderer){
    /*  */

    for(int i=0; i<16; i++){
        Piece piece;
        SDL_Rect imageDecoupe = { i%4*100, i/4*100, 100, 100 };
        SDL_Rect screenRect = { 40+ liste[i]%4*100, 25 + liste[i]/4*100, 100, 100 };
        initPiece(&piece, &imageDecoupe, &screenRect, rand() % 4);
        renderPieceTexture(&piece, texture, renderer);
    }
}

int main()
{
    bool quit = false;
    SDL_Event event;
 
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

    while (!quit)
    {
        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }

        // Création du puzzle (grille gauche)
        int liste[16];
        for(int i=0; i<16; i=i+1){
            liste[i] = i;
        }
        shuffleList(liste);
        createPuzzle(liste, texture, renderer);

        // grille droite, de résolution
        SDL_Rect rect;
        rect.x = 460;
        rect.y = 25;
        rect.w = 400;
        rect.h = 400; 
        SDL_RenderCopy(renderer, texture,  NULL, &rect);


        SDL_RenderPresent(renderer);
        SDL_Delay(4000);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit();
 
    return 0;
}


// SDL_Surface* pieceSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, largeurDecoupe, hauteurDecoupe, 32, 0, 0, 0, 0 );

// SDL_Rect rectDecoupe = { 0, 0, largeurDecoupe, hauteurDecoupe };

// int decoupeRetour = SDL_BlitSurface(image, &rectDecoupe , pieceSurface, NULL);
// if (decoupeRetour < 0){
//     printf( " SDL_BlitSurface Error: %s\n", SDL_GetError() );
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     return 1;
// }



// Modifier les pixels d'un carré donné de l'image
// SDL_Rect piece = { 0, 0, 100, 100 };
// int decoupe = SDL_UpdateTexture(texture, &piece, const void* pixels , int pitch);
// if (decoupe < 0){
//     printf( " SDL_UpdateTexture Error: %s\n", SDL_GetError() );
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     return 1;
// }