#ifndef FUNCOES_H
#define FUNCOES_H

int  init(char arg[],char arg2[]);
int  loadMedia(NPC *p,char path[]);
void createNPC(NPC *p,int x,int y,int w,int h,int velX,int velY,char path[]);
void moveNPC(NPC *p);
void createBricks(int lvl);
int  trackCollision(NPC *p,int opt);
int  loadBrick(BRICK *p,char path[]); // @TODO = agrupar com loadMedia
#endif
