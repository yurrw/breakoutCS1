#ifndef DEFS_H
#define DEFS_H

typedef struct _BORDA{
	SDL_Texture *texture;
	SDL_Rect rect;
}BORDA;

typedef struct _NPC{
	int velX;
	int velY;
	SDL_Texture *texture;
	SDL_Rect rect;
}NPC;

typedef struct _BRICKS{
    int existance;
    int lives;  // bricks` lifes
    int x;      // X position
    int y;      // Y position
	SDL_Texture *texture;
	SDL_Rect rect;

}BRICK; 

//@TODO > players` structure 
#endif
