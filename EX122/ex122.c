#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.2.2",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 250, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderClear(ren);

    int round_rect = roundedRectangleRGBA(ren, 30,10, 290,230, 45, 0,200,100,255);
    int circle = filledCircleColor(ren, 160, 120, 100, 0xAA00FFFF);
    int ellipse = filledEllipseColor(ren, 160, 120, 120, 80, 0x5E493777);
    Sint16 vx[] = {450,100,400}; Sint16 vy[] = {100,200,200};
    int polygon = filledPolygonRGBA(ren, vx,vy, 3, 0,50,180,255);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}