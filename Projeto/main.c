#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    MENU,
    PLACAR,
    JOGO,
    FIM
} EstadosTela;

typedef enum {
    CARREGADA,
    RECARREGANDO
} EstadosArma;

typedef enum {
    FUNCIONAL,
    DESTRUÍDA
} EstadosNave;

typedef struct {
    SDL_Rect posicao;
    SDL_Rect projetil;
    EstadosArma arma;
    EstadosNave nave;
} Jogador;

typedef struct {
    SDL_Rect posicao;
    SDL_Rect projetil;
    EstadosArma arma;
    EstadosNave nave;
    float minDelayMovimento;
    float maxDelayMovimento;
    float minDelayDisparo;
    float maxDelayDisparo;
} Inimigo;


int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms);

void gerarInimigo(Inimigo* inimigo);
void reiniciarJogo(Jogador* jogador, Inimigo* inimigos);


int main (int argc, char* args[])
{
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    TTF_Init();

    SDL_Window* win = SDL_CreateWindow("SPACE INVADERS 2.0",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        550, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_Texture* img = IMG_LoadTexture(ren, "sprites3.png");
    assert(img != NULL);

    TTF_Font* fnt = TTF_OpenFont("tiny.ttf", 40);
    assert(fnt != NULL);

    srand(time(NULL));

    FILE* fp;
    fp = fopen("scoreboard.txt", "r+");


    /* EXECUÇÃO */
    SDL_Rect spriteNave = { 0,0, 490,230 };

    Jogador jogador = {
        .posicao = (SDL_Rect) { 250,400, 50,50 },
        .projetil = (SDL_Rect) { 0,-20, 4,15 },
        .arma = CARREGADA,
        .nave = FUNCIONAL
    };

    Inimigo inimigos[5];
    for (int i = 0; i < sizeof(inimigos)/sizeof(inimigos[0]); i++) {
        gerarInimigo(&inimigos[i]);
    }

    float delaySpawn = 0.00;

    int score = 0;
    SDL_Rect scoreRect = { 10,5, 160,40 };

    EstadosTela telaAtual = MENU;

    SDL_Rect nomeJogo = { 30,50, 500,150 };
    SDL_Rect botaoJogar = { 200,225, 150,45 };
    SDL_Rect botaoPlacar = { 200,290, 150,45 };
    SDL_Rect botaoSair = { 200,355, 150,45 };

    SDL_Rect primeiroLugar = { 200,120, 150,45 };
    SDL_Rect segundoLugar = { 200,185, 150,45 };
    SDL_Rect terceiroLugar = { 200,250, 150,45 };
    SDL_Rect botaoVoltar = { 200,355, 150,45 };

    SDL_Rect scoreFim = { 30,50, 500,150 };
    SDL_Rect botaoVoltarMenu = { 200,225, 150,45 };
    SDL_Rect botaoSairFim = { 200,290, 150,45 };

    int espera = 10;
    int running = 1;
    while (running) {
        /* EVENTOS */
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            switch (evt.type) {
                case SDL_MOUSEBUTTONDOWN:
                    SDL_Point mouse = { evt.button.x, evt.button.y };
                    
                    if (telaAtual == MENU && SDL_PointInRect(&mouse, &botaoJogar)) {
                        telaAtual = JOGO;
                    }
                    if (telaAtual == MENU && SDL_PointInRect(&mouse, &botaoPlacar)) {
                        telaAtual = PLACAR;
                    }
                    if (telaAtual == MENU && SDL_PointInRect(&mouse, &botaoSair)) {
                        // running = 0;
                        exit(0);
                    }

                    if (telaAtual == PLACAR && SDL_PointInRect(&mouse, &botaoVoltar)) {
                        telaAtual = MENU;
                    }

                    if (telaAtual == FIM && SDL_PointInRect(&mouse, &botaoVoltarMenu)) {
                        reiniciarJogo(&jogador, inimigos);
                        score = 0;
                        telaAtual = MENU;
                    }
                    if (telaAtual == FIM && SDL_PointInRect(&mouse, &botaoSairFim)) {
                        // running = 0;
                        exit(0);
                    }
                    
                    break;

                case SDL_QUIT:
                    // running = 0;
                    exit(0);
                    break;

                case SDL_KEYDOWN:
                    switch (evt.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_w:
                            if (telaAtual == JOGO && jogador.posicao.y - 10 >= 0)
                                jogador.posicao.y -= 10;
                            break;

                        case SDLK_DOWN:
                        case SDLK_s:
                            if (telaAtual == JOGO && jogador.posicao.y + 10 <= 450)
                                jogador.posicao.y += 10;
                            break;

                        case SDLK_LEFT:
                        case SDLK_a:
                            if (telaAtual == JOGO && jogador.posicao.x - 10 >= 0)
                                jogador.posicao.x -= 10;
                            break;

                        case SDLK_RIGHT:
                        case SDLK_d:
                            if (telaAtual == JOGO && jogador.posicao.x + 10 <= 500)
                                jogador.posicao.x += 10;
                            break;

                        case SDLK_SPACE:
                            if (telaAtual == JOGO && jogador.arma == CARREGADA) {
                                jogador.projetil = (SDL_Rect) {
                                    jogador.posicao.x + 23,jogador.posicao.y - 15, 4,15
                                };
                                jogador.arma = RECARREGANDO;
                            }
                            break;
                    }
            }
        } else {
            espera = 10;

            if (jogador.arma == RECARREGANDO) {
                jogador.projetil.y -= 5;

                if (jogador.projetil.y + 15 <= 0) {
                    jogador.arma = CARREGADA;
                }
            }


            /* NAVES INIMIGAS */
            if (telaAtual == JOGO) {
                if (delaySpawn < 5) {
                    delaySpawn += 0.05;
                } else {
                    for (int i = 0; i < sizeof(inimigos)/sizeof(inimigos[0]); i++) {
                        if (inimigos[i].nave == DESTRUÍDA) {
                            inimigos[i].nave = FUNCIONAL;
                            inimigos[i].posicao = (SDL_Rect) {
                                (rand() % 11) * 40 + 50,(rand() % 11) * 5 + 50 , 50,50
                            };
                            break;
                        }
                    }
                    delaySpawn = 0.00;
                }

                for (int i = 0; i < sizeof(inimigos)/sizeof(inimigos[0]); i++) {
                    if (inimigos[i].nave == FUNCIONAL) {
                        if (inimigos[i].minDelayMovimento < inimigos[i].maxDelayMovimento) {
                            inimigos[i].minDelayMovimento += 0.1;
                        } else {
                            int esq = 0, dir = 0;
                            if (telaAtual == JOGO && inimigos[i].posicao.x - 10 >= 0) {
                                esq = -10;
                            }
                            if (telaAtual == JOGO && inimigos[i].posicao.x + 10 <= 500) {
                                dir = 10;
                            }
                            inimigos[i].posicao.x += rand() % 2 ? esq : dir;
                            inimigos[i].minDelayMovimento = 0.00;
                        }
                    }

                    if (inimigos[i].arma == CARREGADA) {
                        inimigos[i].projetil = (SDL_Rect) {
                                        inimigos[i].posicao.x + 23,inimigos[i].posicao.y + 15, 4,15
                                    };
                        inimigos[i].minDelayDisparo = 0.00;
                        inimigos[i].arma = RECARREGANDO;
                    } else {
                        inimigos[i].projetil.y += 5;
                        inimigos[i].minDelayDisparo += 0.1;

                        if (inimigos[i].minDelayDisparo >= inimigos[i].maxDelayDisparo) {
                            inimigos[i].arma = CARREGADA;
                        }
                    }
                }
            }
        }


        /* COLISÕES */
        for (int i = 0; i < sizeof(inimigos)/sizeof(inimigos[0]); i++) {
            if (SDL_HasIntersection(&jogador.projetil, &inimigos[i].posicao)) {
                score += 10;
                jogador.projetil.y = -20;
                jogador.arma = CARREGADA;

                gerarInimigo(&inimigos[i]);

                delaySpawn = 0.00;
            }

            if (SDL_HasIntersection(&jogador.projetil, &inimigos[i].projetil)) {
                jogador.projetil.y = -20;
                jogador.arma = CARREGADA;

                inimigos[i].arma = CARREGADA;
            }

            if (SDL_HasIntersection(&jogador.posicao, &inimigos[i].posicao) ||
                SDL_HasIntersection(&jogador.posicao, &inimigos[i].projetil)) {
                    jogador.nave = DESTRUÍDA;
                    jogador.projetil.y = -20;
                    telaAtual = FIM;
            }

            /* TEMPORÁRIO */
            for (int j = i + 1; j < sizeof(inimigos)/sizeof(inimigos[0]); j++) {
                if ((inimigos[i].nave == FUNCIONAL || inimigos[j].nave == FUNCIONAL) &&
                    SDL_HasIntersection(&inimigos[i].posicao, &inimigos[j].posicao)) {
                        gerarInimigo(&inimigos[j]);
                }
            }
        }


        /* PONTUAÇÃO */
        char str[14] = "Score: ";
        char scoreChar[8];
        sprintf(scoreChar, "%d", score);
        strcat(str, scoreChar);
        SDL_Surface* sfcScore = TTF_RenderText_Blended(fnt, str, (SDL_Color) {0xFF,0xFF,0xFF,0xFF});
        assert(sfcScore != NULL);
        SDL_Texture* txtScore = SDL_CreateTextureFromSurface(ren, sfcScore);
        assert(txtScore != NULL);
        SDL_FreeSurface(sfcScore);


        /* RENDERIZAÇÃO */
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderClear(ren);

        switch (telaAtual) {
            case MENU:
                SDL_RenderFillRect(ren, &nomeJogo);
                SDL_Surface* sfcNomeJogo = TTF_RenderText_Blended(fnt, " SPACE INVADERS 2.0 ", (SDL_Color) {0xFF,0xFF,0xFF,0xFF});
                assert(sfcNomeJogo != NULL);
                SDL_Texture* txtNomeJogo = SDL_CreateTextureFromSurface(ren, sfcNomeJogo);
                assert(txtNomeJogo != NULL);
                SDL_FreeSurface(sfcNomeJogo);
                SDL_RenderCopy(ren, txtNomeJogo, NULL, &nomeJogo);
                SDL_DestroyTexture(txtNomeJogo);

                SDL_SetRenderDrawColor(ren, 0x43,0xA0,0x4B,0x00);
                SDL_RenderFillRect(ren, &botaoJogar);
                SDL_Surface* sfcJogar = TTF_RenderText_Blended(fnt, " JOGAR ", (SDL_Color) {0xF4,0xF4,0xF4,0xFF});
                assert(sfcJogar != NULL);
                SDL_Texture* txtJogar = SDL_CreateTextureFromSurface(ren, sfcJogar);
                assert(txtJogar != NULL);
                SDL_FreeSurface(sfcJogar);
                SDL_RenderCopy(ren, txtJogar, NULL, &botaoJogar);
                SDL_DestroyTexture(txtJogar);

                SDL_SetRenderDrawColor(ren, 0xFF,0xDF,0x00,0x00);
                SDL_RenderFillRect(ren, &botaoPlacar);
                SDL_Surface* sfcPlacar = TTF_RenderText_Blended(fnt, " PLACAR ", (SDL_Color) {0x2C,0x2C,0x2C,0xFF});
                assert(sfcPlacar != NULL);
                SDL_Texture* txtPlacar = SDL_CreateTextureFromSurface(ren, sfcPlacar);
                assert(txtPlacar != NULL);
                SDL_FreeSurface(sfcPlacar);
                SDL_RenderCopy(ren, txtPlacar, NULL, &botaoPlacar);
                SDL_DestroyTexture(txtPlacar);

                SDL_SetRenderDrawColor(ren, 0x8B,0x00,0x00,0x00);
                SDL_RenderFillRect(ren, &botaoSair);
                SDL_Surface* sfcSair = TTF_RenderText_Blended(fnt, " SAIR ", (SDL_Color) {0xF4,0xF4,0xF4,0xFF});
                assert(sfcSair != NULL);
                SDL_Texture* txtSair = SDL_CreateTextureFromSurface(ren, sfcSair);
                assert(txtSair != NULL);
                SDL_FreeSurface(sfcSair);
                SDL_RenderCopy(ren, txtSair, NULL, &botaoSair);
                SDL_DestroyTexture(txtSair);
                break;

            case PLACAR:
                SDL_SetRenderDrawColor(ren, 0xDB,0xAC,0x34,0x00);
                SDL_RenderFillRect(ren, &primeiroLugar);
                SDL_Surface* sfcPrimeiroLugar = TTF_RenderText_Blended(fnt, " PrimeiroLugar ", (SDL_Color) {0x00,0x00,0x00,0xFF});
                assert(sfcPrimeiroLugar != NULL);
                SDL_Texture* txtPrimeiroLugar = SDL_CreateTextureFromSurface(ren, sfcPrimeiroLugar);
                assert(txtPrimeiroLugar != NULL);
                SDL_FreeSurface(sfcPrimeiroLugar);
                SDL_RenderCopy(ren, txtPrimeiroLugar, NULL, &primeiroLugar);
                SDL_DestroyTexture(txtPrimeiroLugar);

                SDL_SetRenderDrawColor(ren, 0xAA,0xA9,0xAD,0x00);
                SDL_RenderFillRect(ren, &segundoLugar);
                SDL_Surface* sfcSegundoLugar = TTF_RenderText_Blended(fnt, " SegundoLugar ", (SDL_Color) {0x00,0x00,0x00,0xFF});
                assert(sfcSegundoLugar != NULL);
                SDL_Texture* txtSegundoLugar = SDL_CreateTextureFromSurface(ren, sfcSegundoLugar);
                assert(txtSegundoLugar != NULL);
                SDL_FreeSurface(sfcSegundoLugar);
                SDL_RenderCopy(ren, txtSegundoLugar, NULL, &segundoLugar);
                SDL_DestroyTexture(txtSegundoLugar);

                SDL_SetRenderDrawColor(ren, 0xB8,0x73,0x33,0x00);
                SDL_RenderFillRect(ren, &terceiroLugar);
                SDL_Surface* sfcTerceiroLugar = TTF_RenderText_Blended(fnt, " TerceiroLugar ", (SDL_Color) {0x00,0x00,0x00,0xFF});
                assert(sfcTerceiroLugar != NULL);
                SDL_Texture* txtTerceiroLugar = SDL_CreateTextureFromSurface(ren, sfcTerceiroLugar);
                assert(txtTerceiroLugar != NULL);
                SDL_FreeSurface(sfcTerceiroLugar);
                SDL_RenderCopy(ren, txtTerceiroLugar, NULL, &terceiroLugar);
                SDL_DestroyTexture(txtTerceiroLugar);

                SDL_SetRenderDrawColor(ren, 0x8B,0x00,0x00,0x00);
                SDL_RenderFillRect(ren, &botaoVoltar);
                SDL_Surface* sfcVoltar = TTF_RenderText_Blended(fnt, " VOLTAR ", (SDL_Color) {0xF4,0xF4,0xF4,0xFF});
                assert(sfcVoltar != NULL);
                SDL_Texture* txtVoltar = SDL_CreateTextureFromSurface(ren, sfcVoltar);
                assert(txtVoltar != NULL);
                SDL_FreeSurface(sfcVoltar);
                SDL_RenderCopy(ren, txtVoltar, NULL, &botaoVoltar);
                SDL_DestroyTexture(txtVoltar);
                break;

            case JOGO:
                SDL_RenderCopy(ren, txtScore, NULL, &scoreRect);
                SDL_DestroyTexture(txtScore);
                SDL_RenderFillRect(ren, &jogador.posicao);
                SDL_RenderCopy(ren, img, &spriteNave, &jogador.posicao);
                for (int i = 0; i < sizeof(inimigos)/sizeof(inimigos[0]); i++) {
                    SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
                    SDL_RenderFillRect(ren, &inimigos[i].posicao);
                    SDL_RenderCopy(ren, img, &spriteNave, &inimigos[i].posicao);
                    SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
                    SDL_RenderFillRect(ren, &inimigos[i].projetil);
                }
                SDL_RenderFillRect(ren, &jogador.projetil);
                break;

            case FIM:
                // while(1) {
                //     int num;
                //     fscanf(fp, "%d", &num);
                //     if (feof(fp)) break;

                //     if (score > num) {
                //         fprintf(fp, "\n%d\n", score);
                //     }

                //     printf("%d\n", num);
                // }

                SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
                SDL_RenderFillRect(ren, &scoreFim);
                SDL_Surface* sfcScoreFim = TTF_RenderText_Blended(fnt, str, (SDL_Color) {0xFF,0xFF,0xFF,0xFF});
                assert(sfcScoreFim != NULL);
                SDL_Texture* txtScoreFim = SDL_CreateTextureFromSurface(ren, sfcScoreFim);
                assert(txtScoreFim != NULL);
                SDL_FreeSurface(sfcScoreFim);
                SDL_RenderCopy(ren, txtScoreFim, NULL, &scoreFim);
                SDL_DestroyTexture(txtScoreFim);

                SDL_SetRenderDrawColor(ren, 0x43,0xA0,0x4B,0x00);
                SDL_RenderFillRect(ren, &botaoVoltarMenu);
                SDL_Surface* sfcVoltarMenu = TTF_RenderText_Blended(fnt, " MENU ", (SDL_Color) {0xF4,0xF4,0xF4,0xFF});
                assert(sfcVoltarMenu != NULL);
                SDL_Texture* txtVoltarMenu = SDL_CreateTextureFromSurface(ren, sfcVoltarMenu);
                assert(txtVoltarMenu != NULL);
                SDL_FreeSurface(sfcVoltarMenu);
                SDL_RenderCopy(ren, txtVoltarMenu, NULL, &botaoVoltarMenu);
                SDL_DestroyTexture(txtVoltarMenu);

                SDL_SetRenderDrawColor(ren, 0x8B,0x00,0x00,0x00);
                SDL_RenderFillRect(ren, &botaoSairFim);
                SDL_Surface* sfcSairFim = TTF_RenderText_Blended(fnt, " SAIR ", (SDL_Color) {0xF4,0xF4,0xF4,0xFF});
                assert(sfcSairFim != NULL);
                SDL_Texture* txtSairFim = SDL_CreateTextureFromSurface(ren, sfcSairFim);
                assert(txtSairFim != NULL);
                SDL_FreeSurface(sfcSairFim);
                SDL_RenderCopy(ren, txtSairFim, NULL, &botaoSairFim);
                SDL_DestroyTexture(txtSairFim);
                break;
        }
        SDL_RenderPresent(ren);
    }


    /* FINALIZAÇÃO */
    fclose(fp);

    SDL_DestroyTexture(img);
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

void gerarInimigo(Inimigo* inimigo) {
    inimigo->posicao = (SDL_Rect) { -50,0 , 50,50 };
    inimigo->arma = RECARREGANDO;
    inimigo->nave = DESTRUÍDA;
    inimigo->minDelayMovimento = 0.00;
    inimigo->maxDelayMovimento = ((float) rand() / (float) RAND_MAX) * 2 + 1;
    inimigo->minDelayDisparo = 0.00;
    inimigo->maxDelayDisparo = ((float) rand() / (float) RAND_MAX) * 2 + 10;
}

void reiniciarJogo(Jogador* jogador, Inimigo* inimigos) {
    jogador->posicao = (SDL_Rect) { 250,400, 50,50 };
    jogador->projetil = (SDL_Rect) { 0,-20, 4,15 };
    jogador->arma = CARREGADA;
    jogador->nave = FUNCIONAL;

    for (int i = 0; i < sizeof(inimigos); i++) {
        gerarInimigo(&inimigos[i]);
        inimigos[i].projetil = (SDL_Rect) { -10,-20, 4,15 };
    }
}
