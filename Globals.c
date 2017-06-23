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
int COLS;
int ROWS;
SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Event event;
TTF_Font *gFont;
NPC ball;
NPC plataform;
BRICK brick;
