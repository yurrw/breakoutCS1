#ifndef GLOBALS_H
#define GLOBALS_H

extern int WIDTH;
extern int HEIGHT;
extern int play;
extern int lvl;             // Actual player`s level
extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;
extern SDL_Event event;
extern TTF_Font *gFont;
extern SDL_Texture *scoreTexture;

extern int points;
extern int vida;
extern char vidaStr[10];
extern char scoreStr[10];

extern NPC ball;
extern NPC plataform;
extern int COLS;
extern int ROWS;
extern BRICK **brick;
extern Mix_Chunk *sideHit;
extern Mix_Chunk *destroyBrick;
extern Mix_Music *music;


#endif
