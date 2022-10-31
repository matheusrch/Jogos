#include <SDL2/SDL.h>

int main (int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.2.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_Rect r1 = { 60,20, 10,10 };
    SDL_RenderFillRect(ren, &r1);

    SDL_SetRenderDrawColor(ren, 100, 255, 0, 0);
    SDL_Rect r2 = { 140,50, 5,30 };
    SDL_RenderFillRect(ren, &r2);

    SDL_SetRenderDrawColor(ren, 50, 150, 150, 0);
    SDL_Rect r3 = { 90,30, 80,15 };
    SDL_RenderFillRect(ren, &r3);

    SDL_SetRenderDrawColor(ren, 220, 80, 80, 0);
    SDL_Rect r4 = { 10,40, 20,50 };
    SDL_RenderFillRect(ren, &r4);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_Rect r5 = { 100,10, 20,50 };
    SDL_RenderFillRect(ren, &r5);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderDrawLine(ren, 5,20, 35,80);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
    SDL_RenderDrawLine(ren, 160,20, 25,75);
    
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderDrawPoint(ren, 65,25);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderDrawPoint(ren, 20,70);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

