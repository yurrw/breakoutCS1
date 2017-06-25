#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Defs.h"
#include "Globals.h"
#include "Funcoes.h"


int init(char w[],char h[]){
	WIDTH = atoi(w);
	HEIGHT = atoi(h);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0){
		printf("Erro ao iniciar o SDL : %s\n",SDL_GetError());
		return 1;
	}

	gWindow = SDL_CreateWindow("Breakout",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
	if(!gWindow){
		printf("Erro ao criar a janela : %s\n",SDL_GetError());
		return 1;
	}

	gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!gRenderer){
		printf("Error ao criar o renderer : %s\n",SDL_GetError());
		return 1;
	}

	if(!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)){
		printf("Erro ao carregar as flags das imagens : %s\n",SDL_GetError());
		return 1;
	}

	return 0;

}

int loadMedia(NPC *p,char path[]){
	SDL_Surface *imageLoader;
    printf("IMGAMGEM  ==> %s\n\n",path );
	imageLoader = IMG_Load(path);
	if(!imageLoader){
		printf("Nao foi possivel carregar a imagem : %s\n",SDL_GetError());
		return 1;
	}

	p->texture = SDL_CreateTextureFromSurface(gRenderer,imageLoader);
	if(!p->texture){
		printf("Nao foi possivel carregar a textura : %s\n",SDL_GetError());
		return 1;
	}

	return 0;

}
int loadBrick(BRICK *p,char path[]){
	SDL_Surface *imageLoader;
    printf("IMAGEBRICK %s\n\n",path);
	imageLoader = IMG_Load(path);
	if(!imageLoader){
		printf("Nao foi possivel carregar a imagem : %s\n",SDL_GetError());
		return 1;
	}

	p->texture = SDL_CreateTextureFromSurface(gRenderer,imageLoader);
	if(!p->texture){
		printf("Nao foi possivel carregar a textura : %s\n",SDL_GetError());
		return 1;
	}

	return 0;

}
void createBricks(int lvl){
   
    srand((unsigned int)time(NULL)); // random`s seed
    int i,j,y,x;                     // control variables
    i = j = y = x = 0;               // x axis , y axis
    brick=(BRICK **)malloc(ROWS * sizeof(BRICK *));
    
    for (i = 0; i < COLS ; ++i)    //create inner-array
        brick[i] = (BRICK *)malloc(COLS * sizeof(BRICK));
    
    if(brick == NULL){
        printf("erro ao alocar memoria para os blocos\n"); 
        return;  
    }

            int randomLife;                  // number of lives of a block
      for(i=0 ; i < ROWS ; i++ ){
        //generate rows of n cols
        y +=27 + 3; 
        for( j =0 ; j < COLS ; j++){
        //generate columns` of the row
            randomLife = rand() % 3;

            brick[i][j].lives=randomLife;  // block`s life
            brick[i][j].x= x;
            brick[i][j].y= y; 
            brick[i][j].rect.x= x;
            brick[i][j].rect.y = y;
            brick[i][j].rect.w = 55;
            brick[i][j].rect.h = 27;
            
            loadBrick(&brick[i][j],"plataform.png");
  
            x += 60;
            if (x >= WIDTH)
                break;
            else 
                continue;
        }
        x = 0;
    }


 };

void createNPC(NPC *p,int x,int y,int w,int h,int velX,int velY,char path[]){

	p->velX = velX;
	p->velY = velY;
	p->rect.x = x;
	p->rect.y = y;
	p->rect.w = w;
	p->rect.h = h;
	loadMedia(p,path);
}

void moveNPC(NPC *p){
	int dx,dy,maxDistance,minDistance,distance;
	dx = p->rect.x - plataform.rect.x;
	dy = p->rect.y - plataform.rect.y;
	distance = sqrt(dx*dx + dy*dy);
	maxDistance = sqrt( (plataform.rect.w - p->rect.w/2) * (plataform.rect.w - p->rect.w/2) + (p->rect.h)*(p->rect.h) );
	minDistance = sqrt( (p->rect.h)*(p->rect.h) + (p->rect.w/2)*(p->rect.w/2) );

	p->rect.x += p->velX;
	p->rect.y += p->velY;

	/*laterais*/
	if(p->rect.x > WIDTH - p->rect.w || p->rect.x < 0){
		p->velX = (-1)*p->velX;
		p->rect.x += p->velX;
	}
	/*teto*/
	if(p->rect.y < 0){
		p->velY = (-1)*p->velY;
		p->rect.y += p->velY;
	}
	/*plataforma*/
	if(p->rect.y > 0.9*HEIGHT-p->rect.w && distance < maxDistance && distance > minDistance && dx > 0 && p->velY > 0){
		p->velY = (-1)*p->velY;
		p->rect.y += p->velY;
	}
	/*fim de jogo*/
	if(p->rect.y > HEIGHT - p->rect.h ){
		SDL_Delay(2000);
		p->rect.x = 0.2*WIDTH;
		p->rect.y = 0.2*HEIGHT;
	}
}


