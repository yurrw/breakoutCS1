#ifndef FUNCOES_H
#define FUNCOES_H

int  init(char arg[],char arg2[]);
int  loadMedia(SDL_Texture **texture,char path[]);
void createNPC(NPC *p,int x,int y,int w,int h,int velX,int velY,char path[]);
void moveNPC(NPC *p);
void createBricks(int lvl);
int  trackCollision(NPC *p,int opt);
int menu();
int loadSound();
int createFontTexture(SDL_Texture **tex, TTF_Font *font, int r, int g, int b, char *string);
int brickCollision(int n);
int gameOver();
#endif
