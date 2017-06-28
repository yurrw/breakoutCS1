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
int points = 0;
int pointsForLife = 0;
int vida = 1;
char vidaStr[10];
char scoreStr[10];
char nome[40];
int pointsTMP=0;
SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Event event;
TTF_Font *gFont;
SDL_Texture *scoreTexture;
SDL_Texture *vidaTexture;
SDL_Texture *nomeTexture;
SDL_Texture *rankTexture;
NPC ball;
NPC plataform;
BRICK **brick;
Mix_Chunk *sideHit;
Mix_Chunk *destroyBrick;
Mix_Music *music;
RK rkdados[6];
