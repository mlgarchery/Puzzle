// #include <stdio.h>
// #include <math.h>
#include <SDL.h>
#include <stdbool.h>
    
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

int main()
{
    bool quit = false;
 
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


    // for(int i=0;i<16; i++){
    //     printf("%d", liste[i] );
    // }
    // printf("\n");

    Piece listePieces[16];
    createPuzzle(listePieces, liste_permutation);

    // grille droite, de résolution
    SDL_Rect rectDroit;
    rectDroit.x = 460;
    rectDroit.y = 25;
    rectDroit.w = 400;
    rectDroit.h = 400; 

    SDL_SetRenderDrawColor( renderer, 180, 
        255, 255, 255); 
    SDL_RenderFillRect(renderer, &rectDroit);


    int x_souris = -1;
    int y_souris = -1;

    int etatPrecedent =0;
    int etatPrecedentRigthButton =0;
    int numPiece = -1;
    
    while (!quit)
    {
        SDL_PumpEvents(); // On demande à./ la SDL de mettre à jour les états sur les périphériques


        // Clavier
        {
            const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
            if ( pKeyStates[SDL_SCANCODE_ESCAPE] )
            {
                quit = 1;
            }
            if (pKeyStates[SDL_SCANCODE_RETURN]) {
                printf("La touche entree est appuyee.");
            }
        }
        printf("\n");
        // Souris
        {
            Uint32 boutons = SDL_GetMouseState(&x_souris,&y_souris);

            fprintf(stdout, "Position de la souris : %d;%d\n",x_souris,y_souris);
            fprintf(stdout, "Bouton de la souris : %d\n",boutons);

            // SDL_GetRelativeMouseState(&x_souris,&y_souris);
            // fprintf(stdout, "Déplacement de la souris : %d;%d\n",x_souris,y_souris);

            // front montant
            if(boutons ==1 && etatPrecedent !=1){
               
                for(int i=0; i<16; i++){
                    int x_rect = listePieces[i].screenRect.x;
                    int y_rect = listePieces[i].screenRect.y;
                    int w_rect = listePieces[i].screenRect.w;
                    int h_rect = listePieces[i].screenRect.h;

                    if(x_souris >= x_rect && x_souris < x_rect + w_rect && y_souris >= y_rect && y_souris < y_rect + h_rect){
                        numPiece = i;
                        etatPrecedent =1;
                        break;
                    }
                }
                if(etatPrecedent == 0) {
                    if(numPiece !=-1){
                        if(x_souris >= 460 && x_souris< 860 &&
                            y_souris >= 25 && y_souris < 425){

                            SDL_Rect new_rect;
                            new_rect.x = (x_souris -460)/100 * 100 + 460;
                            new_rect.y = (y_souris -25)/100 *100 + 25;
                            new_rect.w =100;
                            new_rect.h =100;
                            initPiece(&listePieces[numPiece], 
                            &listePieces[numPiece].imageDecoupe, 
                            &new_rect, 
                            listePieces[numPiece].entierAngle);
                            numPiece = -1;
                        }else if(x_souris >= 40 && x_souris< 440 &&
                                y_souris >= 25 && y_souris < 425){

                            SDL_Rect new_rect;
                            new_rect.x = (x_souris -40)/100 * 100 + 40;
                            new_rect.y = (y_souris -25)/100 *100 + 25;
                            new_rect.w =100;
                            new_rect.h =100;
                            initPiece(&listePieces[numPiece], 
                            &listePieces[numPiece].imageDecoupe, 
                            &new_rect, 
                            listePieces[numPiece].entierAngle);
                            numPiece = -1;

                        }else{
                            numPiece = -1;
                        }
                        etatPrecedent = 2;
                    }
                }
                if(etatPrecedent ==2 && boutons ==0){
                    etatPrecedent = 0;
                }
            }
            printf("%d\n", etatPrecedent);  
            printf("%d \n", numPiece );

            if(boutons !=1 && etatPrecedent ==1){
                etatPrecedent = 0;
            }

            if(boutons == 4 && etatPrecedentRigthButton==0){
                for(int i=0; i<16; i++){
                    int x_rect = listePieces[i].screenRect.x;
                    int y_rect = listePieces[i].screenRect.y;
                    int w_rect = listePieces[i].screenRect.w;
                    int h_rect = listePieces[i].screenRect.h;


                    if(x_souris >= x_rect && x_souris < x_rect + w_rect && y_souris >= y_rect && y_souris < y_rect + h_rect){
                        printf("%d \n", x_rect );
                        initPiece(&listePieces[i], &listePieces[i].imageDecoupe, &listePieces[i].screenRect, listePieces[i].entierAngle + 1);
                        etatPrecedentRigthButton = 1;
                        break;
                    }
                }

            }
            if(boutons !=4 && etatPrecedentRigthButton ==1){
                etatPrecedentRigthButton = 0;
            }
            // printf("%d \n", numPiece );

        }
        printf("\n");
        /* On a traité les événements, on peut continuer le jeu */

        /* On ralentit un peu le programme */
        // SDL_Delay(10);

        renderPuzzle(listePieces, texture, renderer);
        SDL_RenderPresent(renderer);    
    }


    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit();
 
    return 0;
}
