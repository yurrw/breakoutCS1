#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Defs.h"
#include "Globals.h"

int WIDTH;
int HEIGHT;
int play = 1;
int lvl= 1;
int COLS = 7;
int ROWS = 5;
int score;
int vida;
char vidaStr[10];
char scoreStr[10];
SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Event event;
TTF_Font *gFont;
SDL_Texture *fontTexture;
NPC ball;
NPC plataform;
BRICK **brick;
Mix_Chunk *sideHit;
Mix_Chunk *destroyBrick;
Mix_Music *music;
