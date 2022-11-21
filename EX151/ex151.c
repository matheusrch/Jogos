#include <SDL2/SDL.h>

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms);

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.5.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    SDL_Rect r1 = { 40,20, 10,10 };
    SDL_Rect r2 = { 100,50, 55,25 };
    SDL_Rect r3 = { 10,10, 30,45 };
    int espera = 500;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r1);
        SDL_SetRenderDrawColor(ren, 0x00,0xFF,0x00,0x00);
        SDL_RenderFillRect(ren, &r2);
        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        r2.y -= 5;
                        break;
                    case SDLK_DOWN:
                        r2.y += 5;
                        break;
                    case SDLK_LEFT:
                        r2.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        r2.x += 5;
                        break;
                }
            }
            else if (evt.type == SDL_QUIT) {
                break;
            }
        } else {
            r1.x += 2;
            r1.y += 2;
        }
        SDL_GetMouseState(&r3.x, &r3.y);
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
        *ms = 500;
        return 0;
    }
}