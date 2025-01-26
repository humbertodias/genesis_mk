#ifndef INTRO_DEMO_ROOM_H 
#define INTRO_DEMO_ROOM_H

#include "types.h"
#include "vdp.h"
#include "vdp_bg.h"
#include "vdp_tile.h"
#include "dma.h"
#include "pal.h"
#include "sprite_eng.h"
#include "typewriter_printer.h"
#include "estruturas.h"

#include "stages.h"
#include "gfx.h"
#include "sprites.h"

// Declaração das variáveis globais como extern 
extern u32 gFrames;
extern u16 gInd_tileset;

// Variáveis Locais

void processIntro();

void loadBioScreen();

#endif