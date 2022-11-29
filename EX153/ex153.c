#include <SDL2/SDL.h>
#include <stdlib.h>

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms);

typedef struct {
    SDL_Rect r;
    int R, G, B;
} retangulo;

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.5.3",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    retangulo ret;
    ret.r = (SDL_Rect) { 50,20, 10,10 };
    int espera = 200;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, ret.R,ret.G,ret.B,0x00);
        SDL_RenderFillRect(ren, &ret.r);
        SDL_RenderPresent(ren);
        
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (!isevt) {
            espera = 200;
            ret.R = rand()%256;
            ret.G = rand()%256;
            ret.B = rand()%256;
            if (ret.r.y == 20 && ret.r.x <= 150) {
                ret.r.x += 2;
            }
            if (ret.r.x == 150 && ret.r.y <= 80) {
                ret.r.y += 2;
            }
            if (ret.r.y == 80 && ret.r.x >= 50) {
                ret.r.x -= 2;
            }
            if (ret.r.x == 50 && ret.r.y >= 20) {
                ret.r.y -= 2;
            }
        } else if (evt.type == SDL_QUIT) {
            break;
        }
    }


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms) {
    int antes = SDL_GetTicks();
    int aux_isevt = SDL_WaitEventTimeout(evt, *ms);
    if (aux_isevt) {
        *ms -= (SDL_GetTicks() - antes);
        if (*ms < 0) {
            *ms = 0;
        }
        return 1;
    } else {
        return 0;
    }
}
