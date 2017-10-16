#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Defs.h"
#include "Globals.h"
#include "Funcoes.h"

int main(int argc, char *argv[]){
	int i, j;			//control variables
	IMAG limite, score;
	SDL_Rect scoreRect;
	SDL_Rect vidaRect;
	SDL_Rect nomeRect;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	
	//Setting up screen
	switch(argc){
		case 2  : sscanf(argv[1],"%s",nome);
		//Defaults' mode 640x480
		case 1  : { 
					if(init("640","480")==1){
						return 1;
		            }
		        	break;
		         }
		case 4  : { 
					if(init(argv[1],argv[2]) == 1){
						 return 1;
		            }
					sscanf(argv[3],"%s",nome);
                    break;
				 }
		default :
			puts("Inicializar com Largura Altura Nome OU Nome apenas");
			puts("./Breakout WIDTH HEIGH username");
		    return 1;	 
	}

	if(loadSound() != 0){
		return 1;
	}

	menu();
	
	//Loading screen limits for the game
	if (loadMedia(&limite.texture,"borda.png"))
		return 1;
	limite.rect.x = (WIDTH / 4) - 10;
	limite.rect.y = 0;
	limite.rect.w = (WIDTH / 4) * 3 + 10;
	limite.rect.h = HEIGHT;
	
	//Loading score
	if (loadMedia(&score.texture,"score.png"))
		return 1;
	score.rect.x = 0;
	score.rect.y = 0;
	score.rect.w = 157;
	score.rect.h = HEIGHT;
	
	scoreRect.y = 0;
	createNPC(&ball,WIDTH/2,HEIGHT/2,20,20,0,5,"ball.png");
   	createBricks(lvl);

	//Creating Plataform
	createNPC(&plataform,WIDTH/2,0.9*HEIGHT,100,20,20,20,"plataform.png");
	
	while(play){
		//Moving to right
		if (state[SDL_SCANCODE_RIGHT] ) {
			if(plataform.rect.x > WIDTH - plataform.rect.w - WIDTH / 32){
				//when hits the right corner
				plataform.rect.x += 0;
			}else{
				plataform.rect.x += plataform.velX/2.5;
			}
		//Moving to left
		}else if(state[SDL_SCANCODE_LEFT]){
			if(plataform.rect.x < WIDTH / 4  + 7){
				//when hits the left corner				
				plataform.rect.x -= 0;
			}else{
				plataform.rect.x -= plataform.velX/2.5;
			}
		}
		if(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					play = 0;
					return 1;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:
							menu();
							break;
					}
					break;
			}
		}

		
		//Moving ball
		moveNPC(&ball);
	
		if (vida <= 0)
			gameOver();
		//Wins a life in 10K points
		if (pointsForLife >= 10000)
		{
			vida += 1;
			pointsForLife -= 10000;
		}

		sprintf(scoreStr, "%d", points);
		sprintf(vidaStr, "%d", vida);
		
        createFontTexture(&scoreTexture,gFont, 255, 0, 0, scoreStr);
        createFontTexture(&vidaTexture,gFont, 0, 255, 0, vidaStr);
        createFontTexture(&nomeTexture,gFont,0,255,0,nome);
        SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h);
        SDL_QueryTexture(vidaTexture, NULL, NULL, &vidaRect.w, &vidaRect.h);
        SDL_QueryTexture(nomeTexture,NULL,NULL,&nomeRect.w,&nomeRect.h);
		scoreRect.x = WIDTH / 12;
		scoreRect.y = HEIGHT / 8;
		vidaRect.x = WIDTH / 10;
		vidaRect.y = (HEIGHT / 8) * 7;
		nomeRect.x = WIDTH/18;
		nomeRect.y = HEIGHT/5;

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
        SDL_RenderCopy(gRenderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(gRenderer, vidaTexture, NULL, &vidaRect);
        SDL_RenderCopy(gRenderer,nomeTexture,NULL,&nomeRect);
        SDL_RenderPresent(gRenderer);
		SDL_Delay(30);	//TODO checar

	}

	SDL_Quit();

	return 0;
}
