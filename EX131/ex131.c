#include <SDL2/SDL.h>
#include <stdlib.h>

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.3.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    SDL_Rect r = { 50,20, 10,10 };
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, rand()%256,rand()%256,rand()%256,0x00);
        SDL_RenderFillRect(ren, &r);
        if (r.y == 20 && r.x <= 150) {
            r.x += 2;
        }
        if (r.x == 150 && r.y <= 80) {
            r.y += 2;
        }
        if (r.y == 80 && r.x >= 50) {
            r.x -= 2;
        }
        if (r.x == 50 && r.y >= 20) {
            r.y -= 2;
        }
        
        SDL_RenderPresent(ren);
        SDL_Delay(200);
    }


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}