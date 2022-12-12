#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms);

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.7.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         150, 150, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "../sprites.png");
    assert(img != NULL);


    int x = 0;
    int y = 0;
    int animacaoAndar = 1;
    int espera = 500;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        
        SDL_Rect r = { x,y, 64,64 };
        SDL_Rect c;
        
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (!isevt) {
            espera = 500;
            
            int spriteX = (animacaoAndar) ? 64 : 192;
            
            if (r.y == 0 && r.x < 80) {
                c = (SDL_Rect) { spriteX,128, 64,64 };
                x += 8;
            }
            if (r.x == 80 && r.y < 80) {
                c = (SDL_Rect) { spriteX,0, 64,64 };
                y += 8;
            }
            if (r.y == 80 && r.x > 0) {
                c = (SDL_Rect) { spriteX,64, 64,64 };
                x -= 8;
            }
            if (r.x == 0 && r.y > 0) {
                c = (SDL_Rect) { spriteX,192, 64,64 };
                y -= 8;
            }
            
            SDL_RenderCopy(ren, img, &c, &r);
            SDL_RenderPresent(ren);
            animacaoAndar = !animacaoAndar;
            
        } else if (evt.type == SDL_QUIT) {
            break;
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
