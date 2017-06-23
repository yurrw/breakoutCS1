#ifndef DEFS_H
#define DEFS_H

typedef struct _NPC{
	int velX;
	int velY;
	SDL_Texture *texture;
	SDL_Rect rect;
}NPC;

typedef struct _BRICKSONE{
    int posY,posX,lives;
    SDL_Texture *texture;
    SDL_Rect rext;
}BRICKONE;

typedef struct _BRICKS{
    BRICKONE brks;
}BRICK;
#endif
