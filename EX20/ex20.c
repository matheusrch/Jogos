#include <SDL2/SDL.h>

typedef struct {
	SDL_Rect r;
	SDL_Point origPos;
	int didDrag;
	int dragOrigin;
} retangulo;

int main (int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 2.0",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 300, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


	SDL_Point mouse;
	retangulo ret;
	ret.r = (SDL_Rect) { 60,40, 50,50 };
    ret.didDrag = 0;
    ret.dragOrigin = 0;
    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &ret.r);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        SDL_WaitEvent(&evt);
        switch (evt.type) {
			case SDL_KEYDOWN:
                switch (evt.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    	ret.dragOrigin = 0;
                    	ret.r.x = ret.origPos.x;
                    	ret.r.y = ret.origPos.y;
                        printf("Cancelled!\n");
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (ret.dragOrigin) {
                	ret.r.x = mouse.x;
                	ret.r.y = mouse.y;
                	ret.didDrag = 1;
                	//printf("Dragging...\n");
                }
                break;

			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouse.x, &mouse.y);
				if (SDL_PointInRect(&mouse, &ret.r)) {
					ret.origPos.x = ret.r.x;
					ret.origPos.y = ret.r.y;
					ret.dragOrigin = 1;
					ret.didDrag = 0;
				}
				break;
			
			case SDL_MOUSEBUTTONUP:
				ret.dragOrigin = 0;
				if (ret.didDrag) {
					printf("Dropped!\n");
				} else {
					printf("Clicked!\n");
				}
				break;
			
            case SDL_QUIT:
                running = 0;
                break;
        }
	}


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
