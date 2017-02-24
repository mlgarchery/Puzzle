#include <stdio.h>
#include <SDL.h>
#include <math.h>

struct sprite{
int x;
int y;
int h;
int w;
int color[3];
};

void drawSprite(SDL_Renderer * rend, struct sprite s);
void deplacerSprite(struct sprite *s, double angle, int d);

int main(){

    double newAngle;
    int newDistance;
    scanf("%lf", &newAngle);
    scanf("%i", &newDistance);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf( "SDL_Init Error: %s\n", SDL_GetError() );
        return 1;
    }
    
    // Create a window
    int size_x= 900;
    int size_y= 500;
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, size_x, size_y, SDL_WINDOW_SHOWN);
    if (win == NULL){
        printf( "SDL_CreateWindow Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return 1;
    }

    // Create a SDL_Renderer associate to the window (structure allowing drawing)
    SDL_Renderer * rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (rend == NULL){
        printf( "SDL_CreateRenderer Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // set a black background
    SDL_SetRenderDrawColor( rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);      /* Clear the entire screen to our selected color. */

    // draw a red rectangle
    /*SDL_SetRenderDrawColor( rend, 180, 71, 154, 255);
    SDL_Rect rect;
    rect.x = 100;
    rect.y = 50;
    rect.w = 200;
    rect.h = 150;*/
    
    struct sprite sprite1;
    sprite1.x = 100;
    sprite1.y = 50;
    sprite1.w = 200;
    sprite1.h = 150;
    sprite1.color[0]  = 0;
    sprite1.color[1]  = 0;
    sprite1.color[2]  = 255;

    struct sprite sprite2;
    sprite2.x = 250;
    sprite2.y = 80;
    sprite2.w = 100;
    sprite2.h = 50;
    sprite2.color[0]  = 255;
    sprite2.color[1]  = 0;
    sprite2.color[2]  = 0;
    
    drawSprite(rend, sprite1);
    drawSprite(rend, sprite2);

    // SDL_RenderFillRect( rend, &rect);

    // actualize the window with the drawn render.
    SDL_RenderPresent(rend);

    //Attend puis clear
    SDL_Delay(500);
    SDL_SetRenderDrawColor( rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);   

    //Actualise les positions des sprites

    /*double newAngle;
    int newDistance;
    scanf("%lf", &newAngle);
    scanf("%i", &newDistance);*/
    deplacerSprite(&sprite2, newAngle, newDistance);

    drawSprite(rend, sprite1);
    drawSprite(rend, sprite2);
    SDL_RenderPresent(rend);

    // If everything ok, wait 10 seconds.
    SDL_Delay(3000);

    // Properly terminate the programme (free SDL ressources)
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    // End ok
    printf("ok\n");
    return 0;
}

void drawSprite(SDL_Renderer * rend, struct sprite s){
	SDL_SetRenderDrawColor( rend, 180, 
		s.color[0], s.color[1], s.color[2] ); 
    SDL_Rect rect;
    rect.x = s.x;
    rect.y = s.y;
    rect.w = s.w;
    rect.h = s.h; 
    printf( "Salut Rect: %d %d %d %d \n", rect.x, rect.y, rect.w, rect.h );
    SDL_RenderFillRect( rend, &rect);
}

void deplacerSprite(struct sprite *s, double angle, int d){
    (*s).x = (*s).x + d*cos(angle);
    (*s).y = (*s).y + d*sin(angle);
}
