CC=gcc
CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
MIXER_FLAGS=-lSDL2_mixer
MATH_FLAGS=-lm
LFLAGS=-L/usr/lib/x86_64-linux-gnu 
IFLAGS=
SFLAGS=-lSDL2
SOURCES=Breakout.c
BINARIES=Breakout

all: $(BINARIES)

Breakout: Breakout.c
	$(CC) -o Breakout Breakout.c Funcoes.c Globals.c $(CFLAGS) $(SFLAGS) $(LFLAGS) $(IFLAGS) $(IMAGE_FLAGS) $(MIXER_FLAGS) $(MATH_FLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out

limpa:
	del *.o *.exe *.bak *.c~ $(BINARIES) core
