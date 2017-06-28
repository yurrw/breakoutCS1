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
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) != 0){
		printf("Erro ao abrir o canal de audio:%s\n",SDL_GetError());
		return 1;
	}

    if( TTF_Init() == -1 )  {
        printf( "Erro ao iniciar o controlador de fontes: %s\n", TTF_GetError() );
        return  1;
    }

    gFont = TTF_OpenFont("FSEX300.ttf", 28);

	return 0;

}

int createFontTexture(SDL_Texture **tex, TTF_Font *font, int r, int g, int b, char *string)
{
    SDL_Color color = { r, g, b };
    SDL_Surface * surface = TTF_RenderText_Solid(font, string, color);
    *tex = SDL_CreateTextureFromSurface(gRenderer, surface);
    return 0;

}

int loadMedia(SDL_Texture **tex,char path[]){
	SDL_Surface *imageLoader;
	imageLoader = IMG_Load(path);
	if(!imageLoader){
		printf("Nao foi possivel carregar a imagem : %s\n",SDL_GetError());
		return 1;
	}

    	*tex = SDL_CreateTextureFromSurface(gRenderer,imageLoader);
	SDL_FreeSurface(imageLoader);
	if(!tex){
		printf("Nao foi possivel carregar a textura : %s\n",SDL_GetError());
		return 1;
	}

	return 0;


}
void createBricks(int lvl){
   
    srand((unsigned int)time(NULL)); // random`s seed
    int i,j,y,x;                     // control variables
    i = j = 0; // x axis , y axis
    x = (WIDTH / 4);

    y = 35; //espaço inicial em cima antes dos blocos para a bola quebrar varios de uma vez     
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
        for( j =0 ; j < COLS ; j++){
        //generate columns` of the row
            randomLife = 1 + rand() % lvl;

            brick[i][j].existance=1;       // block`s check
            brick[i][j].lives=randomLife;  // block`s life
            brick[i][j].x= x;
            brick[i][j].y= y; 
            brick[i][j].rect.x= x;
            brick[i][j].rect.y = y;
            brick[i][j].rect.w = WIDTH/10;
            brick[i][j].rect.h = 35;
            
            //adicionei cores dependendo da vida
            if(randomLife == 1 || (lvl == 1 && randomLife !=1)){
                loadMedia(&brick[i][j].texture,"brick_green.png");
            }else if(randomLife == 2|| (lvl == 2 && randomLife !=2)){
                loadMedia(&brick[i][j].texture,"brick_yellow.png");
            }else{
                loadMedia(&brick[i][j].texture,"brick_red.png");
            }
            //loadMedia(&brick[i][j].texture,"plataform.png");
            x = x + (WIDTH/10) + 3;
            if (x >= WIDTH - 30)
                break;
            else 
                continue;
        }
        //consertando o erro da bola atravessar alguns blocos da primeira linha
        y +=35; 
        x = (WIDTH / 4);
    }


 }

void createNPC(NPC *p,int x,int y,int w,int h,int velX,int velY,char path[]){

	p->velX = velX;
	p->velY = velY;
	p->rect.x = x;
	p->rect.y = y;
	p->rect.w = w;
	p->rect.h = h;
	loadMedia(&p->texture,path);
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
	if(p->rect.x > WIDTH - p->rect.w || p->rect.x < WIDTH / 4){
		Mix_PlayChannel(-1,sideHit,0);
		p->velX = (-1)*p->velX;
		p->rect.x += p->velX;
	}
	/*teto*/
	if(p->rect.y < 0){
		p->velY = (-1)*p->velY;
		p->rect.y += p->velY;
      if(p->velX == 0){
                            p->velX = 3;
                        }
  
	}
    
    int teste;
    /*plataforma*/
    //@TODO = corrigir direcao da bola depois de bater na plataforma
    //          desmembrar esse if pra por corretamente
    if(p->rect.y > 0.9*HEIGHT-p->rect.w && distance < maxDistance && distance > minDistance && dx > 0 && p->velY > 0){
       
            teste =  (( ((p->rect.x -  (plataform.rect.x + plataform.rect.w/2)) /25) * p->velX ) %300); 
        if(p->rect.x >( plataform.rect.x + (plataform.rect.w) /2 ) ){
            p->velY = -p->velY;
            p->rect.y += p->velY;
                p->velX = teste;
                 p->rect.x +=  teste  == 0 ? p->velX : teste ;
            if(p->velX < 0 ){
                p->velX = - p->velX;
                p->rect.x += p->velX;
            }
        
        }else if(p->rect.x < ( plataform.rect.x + (plataform.rect.w) /2 ) ){
            p->velY = -p->velY;
            p->rect.y += p->velY;
            p->velX =  teste;
            p->rect.x +=   teste  == 0 ? p->velX : teste;

        if(p->velX > 0 ){
                p->velX = - p->velX ;
                p->rect.x += p->velX;
            }
            
        }else{
            p->velY = -p->velY;
            p->rect.y += p->velY;
            if(p->velX > 0 ){
                p->velX = 0;
                p->rect.x += p->velX;
            }
 

            
        }
 
    }
    /*nextlevel*/
    printf("%d\n",pointsTMP );
        if (pointsTMP == 100 * ROWS * COLS){
        lvl++;
        points+=1000;
        nextlevel(p,lvl);
        printf("passou de fase\n");

    }

    /*fim de jogo*/
	if(p->rect.y > HEIGHT - p->rect.h ){
		vida--;
		p->rect.x = 0.5*WIDTH;
		p->rect.y = 0.5*HEIGHT;
	}
    // hits bricks 
    if(p->rect.y <= 6*brick[0][0].rect.h  + 30){
        trackCollision(p,3); 
    }
}
    void nextlevel(NPC *b,int level){
        b->velX =0;
        b->velY =5;
        b->rect.x=WIDTH/2;
        b->rect.y=HEIGHT/2;
        pointsTMP =0;
        free(brick);
        createBricks(lvl);

        printf("nextleve\n");

}
int trackCollision(NPC *p,int opt){
    int i,j;
   
    for (i = 0; i < ROWS ; i++){
        for (j =0; j < COLS;j++){
           // SDL_RenderCopy(gRenderer,brick[i][j].texture,NULL,&brick[i][j].rect;
            if(!brick[i][j].existance){
                continue;             
            }else{
               if((p->rect.y  >= brick[i][j].rect.y - 35
                           &&  p->rect.y  <= brick[i][j].rect.y + 35 ) ){ 
                   if(  (p->rect.x  >= brick[i][j].rect.x - brick[i][j].rect.w  &&
                               p->rect.x  <= brick[i][j].rect.x + brick[i][j].rect.w ) ){
                        //decrementa dps testa a existencia, antes a bolinha atravessava e nao rebatia
                        brick[i][j].lives--; 
                        if(brick[i][j].lives == 0){
                            points +=100;
                            pointsTMP +=100;
                            pointsForLife += 100;
                            Mix_PlayChannel(-1,destroyBrick,0);
                            brick[i][j].texture = NULL;                            
                            brick[i][j].existance  = 0; 
                        }
                        else if(brick[i][j].lives == 2){
                            loadMedia(&brick[i][j].texture,"brick_yellow.png");
                        }
                        else if(brick[i][j].lives == 1){
                            loadMedia(&brick[i][j].texture,"brick_green.png");
                        }

                        //colliding

                        int ballcentery = p->rect.y + 0.5*p->rect.h;
                        int ballcenterx = p->rect.x + 0.5*p->rect.w;
                        printf("%d",p->rect.h); 
                        int brickcenterx = brick[i][j].rect.x + 0.5*brick[i][j].rect.w;
                        int brickcentery = brick[i][j].rect.y + 0.5*brick[i][j].rect.h;
                        int ymin = 0;
                         if (brick[i][j].rect.y > p->rect.y) {
                                ymin = brick[i][j].rect.y;
                         } else {
                                 ymin = p->rect.y;
                         }


                                   int ymax = 0;
                    if (brick[i][j].rect.y+brick[i][j].rect.h < p->rect.y+p->rect.h) {
                        ymax = brick[i][j].rect.y+brick[i][j].rect.h;
                    } else {
                        ymax = p->rect.y+p->rect.h;
                    }


                    float ysize =  ymax -ymin;
                         // Calculate xsize
                    float xmin = 0;
                    if (brick[i][j].rect.x > p->rect.x) {
                        xmin = brick[i][j].rect.x;
                    } else {
                        xmin = p->rect.x;
                    }

                    float xmax = 0;
                    if (brick[i][j].rect.x+brick[i][j].rect.w < p->rect.x+p->rect.w) {
                        xmax = brick[i][j].rect.x+brick[i][j].rect.w;
                    } else {
                        xmax = p->rect.x+p->rect.w;
                    }

                    float xsize = xmax - xmin;
 
                               if (xsize > ysize) {
                        if (ballcentery > brickcentery) {
                            // Bottom
                             //p->rect.y += ysize; // Move out of collision
                         printf("b\n");
                          //  BallBrickResponse(3);
                       brickCollision(3); 
                        } else {
                            // Top
                            p->rect.y -= ysize; // Move out of collision
                         //   BallBrickResponse(1);
                            brickCollision(1);
                            printf("t\n");
                        }
                    } else {
                        if (ballcenterx < brickcenterx) {
                            // Leftb
                            p->rect.x -= xsize; // Move out of collision
                         printf("l\n");
                         brickCollision(0);
                         //   BallBrickResponse(0);
                        } else {
                            // Right
                         printf("r\n");
                            p->rect.x += xsize; // Move out of collision
                      //      BallBrickResponse(2);
                         brickCollision(2);
                        }
                    }





















                //      p->velY = (-1)*p->velY;
                 //       p->rect.y += p->velY;
                        if(p->velX == 0){
                            p->velX = 3;
                            p->velY  = -p->velY;
                            p->rect.y += p->velY;

                        }

                        if(p->velY ==0){
                            p->velY=3;
                            p->velX = -p->velX;
                            p->rect.x += p->velX;

                    
                        }
                    }
                }
            }
        }       
    }
    return 0;
}
int menu() {
	int mouseX, mouseY;
	SDL_Texture *menuImg;
	loadMedia(&menuImg,"menu.png");
	while(play){
        SDL_RenderCopy(gRenderer,menuImg,NULL,NULL);
        SDL_RenderPresent(gRenderer);
		SDL_GetMouseState(&mouseX, &mouseY);
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
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (mouseX > WIDTH / 4 && mouseX < (WIDTH / 4) * 3 && mouseY > HEIGHT / 7 && mouseY < (int)(HEIGHT / 2.7)) 
					{
						SDL_DestroyTexture(menuImg);
						return 1;
					}

					else if(mouseX > WIDTH / 4 && mouseX < (WIDTH / 4) * 3 && mouseY > (int)(HEIGHT / 1.4) && mouseY < (int)(HEIGHT / 1.08))
					{
						SDL_DestroyTexture(menuImg);
                        play = 0;
						break;
					}
                    else if(mouseX > WIDTH / 4 && mouseX < (WIDTH / 4) * 3 && mouseY > (int)(HEIGHT / 2.38) && mouseY < (int)(HEIGHT / 1.56))
                    {
                        ranking();
                        break;
                    }
			}
		}
	}

	return 0;
}
int brickCollision(int n){
    if (ball.velX > 0 ){
        if (ball.velY > 0){
            if ( n == 0 || n == 3){
            ball.velX =  - ball.velX;
             ball.rect.x += ball.velX;

           }else{
                ball.velY =  -ball.velY;
                ball.rect.y += ball.velY;
            }
        }else if (ball.velY < 0){
            if(n ==0 || n ==2){
             ball.velX = -ball.velX;
            ball.rect.x += ball.velX;
            }else{
       ball.velY =  -ball.velY;
     ball.rect.y += ball.velY;
               }
        } 
    }else if (ball.velX < 0){
        if(ball.velY > 0){
            if (n ==2 || n ==3){
             ball.velX = -ball.velX;
     ball.rect.x += ball.velX;

 }else{
      ball.velY =  -ball.velY;
     ball.rect.y += ball.velY;
            }
        }else if (ball.velY < 0){
            if(n == 1|| n ==2){
              ball.velX = -ball.velX;
     ball.rect.x += ball.velX;

}else{
     ball.velY =  -ball.velY;
     ball.rect.y += ball.velY;
            }
        }
    }
return 1;
}
int loadSound(){
	sideHit = Mix_LoadWAV("side_hit.wav");
	if(sideHit == NULL){
		printf("Erro ao carregar sideHit.wav : %s\n",Mix_GetError());
		return 1;
	}
	destroyBrick = Mix_LoadWAV("destroy_brick.wav");
	if(destroyBrick == NULL){
		printf("Erro ao carregar destroyBrick.wav : %s\n",Mix_GetError());
		return 1;
	}
	music = Mix_LoadMUS("main.mp3");
	if(music == NULL){
		printf("Erro ao carregar main.mp3:%s\n",Mix_GetError());
		return 1;
	}
	Mix_PlayMusic(music,-1);

	return 0;
}

int gameOver() {
    writeRank(nome,points);
    SDL_Texture *overImg;
    loadMedia(&overImg,"gameover.png");
    SDL_RenderCopy(gRenderer,overImg,NULL,NULL);
    SDL_RenderPresent(gRenderer);
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
                    }
                    break;
            }
        }
    }
    SDL_DestroyTexture(overImg);
    return 0;
}

int ranking(){
    SDL_Texture *rankingImg;
    
    loadMedia(&rankingImg,"ranking.png");
    showRankT();
    SDL_RenderCopy(gRenderer,rankingImg,NULL,NULL);
    SDL_RenderPresent(gRenderer);
    while(play){
        if(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    play = 0;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            return 0;
                    }
                    break;
                

            
            }
        }
    }
    SDL_DestroyTexture(rankingImg);
    return 0;
}
void showRankT(){
   
    printf("aqui\n");
    IMAG rRank;
    SDL_Rect rankRect; 
    FILE *rankbin;
    int i=0;
    rankbin = fopen("rank.bin","r+b"); //Read actual rank
  
    if (rankbin == NULL){
        fputs("Erro ao abrir rank",stderr);
        exit(1);
    }else{
        fseek(rankbin,0,SEEK_SET); //set pointer to the beggining
        for (i =0; i < 5; i++){

               fread(&rkdados[i].name, sizeof(char), 4, rankbin);
               fread(&rkdados[i].pontuacao, sizeof(int),1, rankbin);
       
             }
        for(i=0;i<5;i++){
            printf("%s %d\n",rkdados[i].name,rkdados[i].pontuacao);
        }
        //PRINTAR O RKDADOS
        rRank.rect.x = (WIDTH/4) -10;
        rRank.rect.y =0;
        rRank.rect.w = 300;
        rRank.rect.h = 400;
        rankRect.x = 300;
        rankRect.y = 300;

        fclose(rankbin);        
    }
    
        
}
int writeRank(char usr[],int pontuacao){
    
    FILE *rankbin;
    int i=0;
    rankbin = fopen("rank.bin","rw+b"); //Read actual rank
  
    if (rankbin == NULL){
        fputs("Erro ao abrir rank",stderr);
        exit(1);
    }else{
        fseek(rankbin,0,SEEK_SET); //set pointer to the beggining
        for (i =0; i < 5; i++){

               fread(&rkdados[i].name, sizeof(char), 4, rankbin);
               fread(&rkdados[i].pontuacao, sizeof(int),1, rankbin);
         }
            
            for(i=0;i<3;i++)
                rkdados[5].name[i] =  usr[i];
            rkdados[5].pontuacao = pontuacao ;
        qsort(rkdados,6,sizeof(RK),cmp); 
        fseek(rankbin,0,SEEK_SET); //set pointer to the beggining
        for ( i =0 ; i < 5; i++){
               fwrite (&rkdados[i].name , sizeof(char), 4,rankbin);
               fwrite (&rkdados[i].pontuacao, sizeof(int), 1, rankbin);
        }
      
        fseek(rankbin,0,SEEK_SET); //set pointer to the beggining
        for (i =0; i < 5; i++){

               fread(&rkdados[i].name, sizeof(char), 4, rankbin);
               fread(&rkdados[i].pontuacao, sizeof(int),1, rankbin);
         }
        printf("tega\n");
        for(i =0; i < 5; i++){
            printf("%s %d\n",rkdados[i].name,rkdados[i].pontuacao);
        }
 
    
    fclose(rankbin);
    }





/*
*/
    return 1;
}
int cmp(const void * a, const void * b)
{
        
        RK *pont1 = (RK *)a;
        RK *pont2 = (RK *)b;
       return ( pont2->pontuacao - pont1->pontuacao );
}

