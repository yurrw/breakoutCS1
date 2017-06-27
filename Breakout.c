#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Defs.h"
#include "Globals.h"
#include "Funcoes.h"

int main(int argc, char *argv[]){
	int i, j; // control
	IMAG limite, score;

	if(argc > 1){
		if(argc < 3 || argc > 3){
			puts("Quantidade de argumentos inválida, inicialize com - Largura  Altura - ");
		return 1;
		}
		if(init(argv[1],argv[2]) == 1){
			return 1;
		}	
	}else{
		if(init("640","480") == 1){
			return 1;
		}
	}
	
   
    //void createNPC(NPC *p,int x,int y,int w,int h,int velX,int velY,char path[]){
	menu();
	loadMedia(&limite.texture,"borda.png");
	limite.rect.x = (WIDTH / 4) - 10;
	limite.rect.y = 0;
	limite.rect.w = (WIDTH / 4) * 3 + 10;
	limite.rect.h = HEIGHT;
	loadMedia(&score.texture,"score.png");
	score.rect.x = 0;
	score.rect.y = 0;
	score.rect.w = 157;
	score.rect.h = HEIGHT;
	
	createNPC(&ball,WIDTH/2,HEIGHT/2,20,19,5,5,"ball.png");
   	createBricks(lvl);

	createNPC(&plataform,WIDTH/2,0.9*HEIGHT,200,25,23,23,"plataform.png");
	while(play){
		if(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					play = 0;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:
							play = 0;
							break;
						case SDLK_LEFT:
							if(plataform.rect.x < WIDTH / 4  + WIDTH / 48){
								plataform.rect.x -= 0;
							}else{
								plataform.rect.x -= plataform.velX;
							}
							break;
						case SDLK_RIGHT:
							if(plataform.rect.x > WIDTH - plataform.rect.w - WIDTH / 32){
								plataform.rect.x += 0;	
							}else{
								plataform.rect.x += plataform.velX;
							}
							break;
					}
					break;
			}
		}

		moveNPC(&ball);
        
		SDL_SetRenderDrawColor(gRenderer,0,0,0,0);
		SDL_RenderClear(gRenderer);
		//SDL_RenderCopy(borda);
		//colocar score
		SDL_RenderCopy(gRenderer,score.texture,NULL,&score.rect);
		SDL_RenderCopy(gRenderer,limite.texture,NULL,&limite.rect);
		SDL_RenderCopy(gRenderer,ball.texture,NULL,&ball.rect);
		SDL_RenderCopy(gRenderer,plataform.texture,NULL,&plataform.rect);
        for (i = 0; i < ROWS ; i++)
            for (j =0; j < COLS;j++){
                SDL_RenderCopy(gRenderer,brick[i][j].texture,NULL,&brick[i][j].rect);
	        }
        SDL_RenderPresent(gRenderer);
		SDL_Delay(16);
	}
	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_DestroyTexture(ball.texture);
	SDL_DestroyTexture(plataform.texture);
	for (i = 0; i < ROWS ; i++)
            for (j =0; j < COLS;j++){
                SDL_DestroyTexture(brick[i][j].texture);
            }
	SDL_Quit();

	return 0;
}
