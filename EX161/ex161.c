#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <string.h>

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms);

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.6",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    TTF_Font* fnt = TTF_OpenFont("../tiny.ttf", 20);
    assert(fnt != NULL);


    SDL_Rect r1 = { 20,10, 10,10 };
    SDL_Rect r2 = { 20,40, 10,10 };
    SDL_Rect r3 = { 20,70, 10,10 };
    int r1Chegou = 0;
    int r2Chegou = 0;
    int r3Chegou = 0;
    int vencedor = 0;
    SDL_Rect chegada = { 180,0, 10,100 };
    int espera = 200;
    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r1);
        SDL_SetRenderDrawColor(ren, 0x00,0xFF,0x00,0x00);
        SDL_RenderFillRect(ren, &r2);
        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r3);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &chegada);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            switch (evt.type) {
                case SDL_KEYDOWN:
                    switch (evt.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (!r2Chegou) {
                                r2.x -= 5;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (!r2Chegou) {
                                r2.x += 5;
                            }
                            break;
                    }
                    break;
                    
                case SDL_MOUSEMOTION:
                    if (!r3Chegou) {
                        SDL_GetMouseState(&r3.x, &r3.y);
                        r3.y = 70;
                    }
                    break;

                case SDL_QUIT:
                    running = 0;
                    break;
            }
        } else {
            espera = 200;
            if (!r1Chegou) {
                r1.x += 5;
            }
        }

        if (SDL_HasIntersection(&r1, &chegada)) {
            r1Chegou = 1;
            if (!vencedor) {
                vencedor = 1;
            }
        }
        if (SDL_HasIntersection(&r2, &chegada)) {
            r2Chegou = 1;
            if (!vencedor) {
                vencedor = 2;
            }
        }
        if (SDL_HasIntersection(&r3, &chegada)) {
            r3Chegou = 1;
            if (!vencedor) {
                vencedor = 3;
            }
        }

        if (r1Chegou && r2Chegou && r3Chegou) {
            char str[20];
            SDL_Color clr;

            if (vencedor == 1) {
                strcpy(str, "Azul venceu!");
                clr = (SDL_Color) {0x00,0x00,0xFF,0xFF};
            }
            if (vencedor == 2) {
                strcpy(str, "Verde venceu!");
                clr = (SDL_Color) {0x00,0xFF,0x00,0xFF};
            }
            if (vencedor == 3) {
                strcpy(str, "Vermelho venceu!");
                clr = (SDL_Color) {0xFF,0x00,0x00,0xFF};
            }

            SDL_Surface* sfc = TTF_RenderText_Blended(fnt, str, clr);
            assert(sfc != NULL);
            SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sfc);
            assert(txt != NULL);
            SDL_FreeSurface(sfc);

            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_Rect vitoria = { 10,25, 180,50 };
            SDL_RenderCopy(ren, txt, NULL, &vitoria);
            SDL_RenderPresent(ren);

            r1.x = r2.x = r3.x = 20;
            r1Chegou = r2Chegou = r3Chegou = 0;
            vencedor = 0;

            SDL_DestroyTexture(txt);

            SDL_Delay(3000);
        }
    }


    TTF_CloseFont(fnt);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
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
