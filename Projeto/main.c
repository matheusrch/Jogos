#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <string.h>

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms);

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);

    SDL_Window* win = SDL_CreateWindow("SPACE INVADERS",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        550, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "sprites3.png");
    assert(img != NULL);


    SDL_Rect jogador = { 250,400, 50,50 };
    SDL_Rect spriteNave = { 0,0, 490,230 };
    SDL_Rect projetil;
    
    int atirando = 0;

    int espera = 10;
    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderFillRect(ren, &projetil);
        SDL_RenderCopy(ren, img, &spriteNave, &jogador);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_w:
                        if (jogador.y - 10 >= 0)
                            jogador.y -= 10;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        if (jogador.y + 10 <= 450)
                            jogador.y += 10;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        if (jogador.x - 10 >= 0)
                            jogador.x -= 10;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (jogador.x + 10 <= 500)
                            jogador.x += 10;
                        break;
                    case SDLK_SPACE:
                        if (!atirando) {
                            atirando = 1;
                            projetil.x = jogador.x + 23;
                            projetil.y = jogador.y - 15;
                            projetil.w = 4;
                            projetil.h = 15;
                        }
                        break;
                }
            }
            else if (evt.type == SDL_QUIT) {
                running = 0;
            }
        } else {
            espera = 10;
            if (projetil.y + 15 >= 0 && projetil.y < 500) {
                projetil.y -= 5;
            }
            else {
                projetil.y = -20;
                atirando = 0;
            }
        }
    }


    SDL_DestroyTexture(img);
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
