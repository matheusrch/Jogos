#include <SDL2/SDL.h>
#include <stdlib.h>

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
    SDL_Rect rArray[10];
    int R[10], G[10], B[10];
    int i = 0;
    SDL_Event evt;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        for (int j = 0; j < i; j++) {
            SDL_SetRenderDrawColor(ren, R[j], G[j], B[j], 255);
            SDL_RenderFillRect(ren, &rArray[j]);
        }
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&evt);
        if (evt.type == SDL_KEYDOWN) {
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
        }
		else if (evt.type == SDL_MOUSEBUTTONDOWN && i < 10) {
    		rArray[i].x = evt.button.x; rArray[i].y = evt.button.y;
    		rArray[i].w = rand() % 100; rArray[i].h = rand() % 50;
    		R[i] = rand() % 256; G[i] = rand() % 256; B[i] = rand() % 256;
			printf("Retângulo %d\n", ++i);
		}
		else if (evt.type == SDL_QUIT) {
	    	break;
		}
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

