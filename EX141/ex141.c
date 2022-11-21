#include <SDL2/SDL.h>
#include <stdlib.h>

typedef struct {
    SDL_Rect ret;
    int R;
    int G;
    int B;
} retangulo;

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo - Ex. 1.4.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUCAO */
    SDL_Rect r = { 40,20, 10,10 };
    retangulo rArray[10];
    int i = 0;
    SDL_Event evt;
    int running = SDL_TRUE;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        for (int j = 0; j < i; j++) {
            SDL_SetRenderDrawColor(ren, rArray[j].R, rArray[j].G, rArray[j].B, 255);
            SDL_RenderFillRect(ren, &rArray[j].ret);
        }
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&evt);
        switch (evt.type) {
            case SDL_KEYDOWN:
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        if (r.y - 5 >= 0) r.y -= 5;
                        break;
                    case SDLK_DOWN:
                        if (r.y + 5 < 95) r.y += 5;
                        break;
                    case SDLK_LEFT:
                        if (r.x - 5 >= 0) r.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        if (r.x + 5 < 195) r.x += 5;
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (i == 10) break;
                rArray[i].ret.x = evt.button.x;
                rArray[i].ret.y = evt.button.y;
                rArray[i].ret.w = rand() % 100;
                rArray[i].ret.h = rand() % 50;
                rArray[i].R = rand() % 256;
                rArray[i].G = rand() % 256;
                rArray[i].B = rand() % 256;
                printf("Retângulo %d\n", ++i);
                break;

            case SDL_QUIT:
                running = SDL_FALSE;
                break;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}