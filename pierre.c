#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>

#include <stdio.h>

int main(__attribute__((unused))int argc,__attribute__((unused)) char** argv)
{

    int x=0; //x souris
    int y=0; //y souris
    int x_rect = 0;
    int y_rect = 0;
    int spriteDeplace =0;

    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    {
        /* Création de la fenêtre */
        SDL_Window* pWindow = NULL;
        pWindow = SDL_CreateWindow("Ma première application SDL2",SDL_WINDOWPOS_UNDEFINED,
                                                                  SDL_WINDOWPOS_UNDEFINED,
                                                                  1280,
                                                                  720,
                                                                  SDL_WINDOW_SHOWN);

        if (pWindow)
        {
            SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED); // Création d'un SDL_Renderer utilisant l'accélération matérielle
           
            if(pRenderer){

                SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
                SDL_Surface* pSprite = SDL_LoadBMP("zozor.bmp");

                if (pSprite){
                char cont = 1; /* Détermine si on continue la boucle principale */
                SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer,pSprite); // Préparation du sprite
                        if ( pTexture )
                        {
                            while ( cont != 0 )
                            {
                                SDL_PumpEvents(); // On demande à./ la SDL de mettre à jour les états sur les périphériques


                                // Clavier
                                {
                                    const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                                    if ( pKeyStates[SDL_SCANCODE_ESCAPE] )
                                    {
                                        cont = 0;
                                    }
                                    // On peut vérifier d'autres touches, si on le souhaite

                                    SDL_Keymod mod = SDL_GetModState();
                                    if ( mod != KMOD_NONE )
                                    {
                                        fprintf(stdout,"Vous avez appuyé sur une touche spéciale : %d\n",mod);
                                    }
                                    if (pKeyStates[SDL_SCANCODE_RETURN]) {
                                        printf("La touche entree est appuyee.");
                                    }
                                }
                                fprintf(stdout,"\n");
                                // Souris
                                {
                                    Uint32 boutons = SDL_GetMouseState(&x,&y);

                                    fprintf(stdout, "Position de la souris : %d;%d\n",x,y);
                                    fprintf(stdout, "Bouton de la souris : %d\n",boutons);

                                    SDL_GetRelativeMouseState(&x,&y);
                                    fprintf(stdout, "Déplacement de la souris : %d;%d\n",x,y);
                                }
                                fprintf(stdout,"\n");

                                fprintf(stdout,"\n");

                                /* On a traité les événements, on peut continuer le jeu */

                                /* On ralentit un peu le programme */
                                SDL_Delay(10);

                                SDL_Rect dest = { x_rect, y_rect, pSprite->w, pSprite->h};



                                Uint32 boutons = SDL_GetMouseState(&x,&y);

                                if(spriteDeplace==1 && boutons ==1){
                                    spriteDeplace=-1;
                                }

                                if((spriteDeplace==0 && (boutons ==1 && abs(x-x_rect)<pSprite->w && abs(y-y_rect)<pSprite->h)) || spriteDeplace==1){
                                spriteDeplace=1;
                                x_rect = x - pSprite->w/2;
                                y_rect = y - pSprite->h/2;
                                dest.x = x_rect;
                                dest.y = y_rect;
                                }

                                if(spriteDeplace==-1){
                                    spriteDeplace=0;
                                }


                                SDL_RenderCopy(pRenderer,pTexture,NULL,&dest); // Copie du sprite grâce au SDL_Renderer
                                SDL_RenderPresent(pRenderer); // Affichage
                                SDL_RenderClear(pRenderer); //On nettoie l'écran

                            }
                            SDL_DestroyWindow(pWindow);
                        }
                        else
                        {
                            fprintf(stderr,"Erreur de création de la texture: %s\n",SDL_GetError());
                        }
                }
                else
                {
                    fprintf(stderr,"Erreur de création de la surface: %s\n",SDL_GetError());
                }
           
            }
            else
            {
                fprintf(stderr,"Erreur de création du renderer: %s\n",SDL_GetError());
            }
        }
        else
        {
            fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
        }
    }

    SDL_Quit();

    return 0;
}