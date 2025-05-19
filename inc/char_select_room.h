#ifndef CHAR_SELECT_ROOM_H
#define CHAR_SELECT_ROOM_H

#include "types.h"
#include "dma.h"
#include "vdp.h"
#include "vdp_bg.h"
#include "sprite_eng.h"
#include "input_system.h"
#include "estruturas.h"
#include "sound.h"

// Declaração das variáveis globais como extern
extern u32 gFrames;
extern u16 gInd_tileset;
extern GraphicElement GE[25];

void processSelecaoPersonagens();

void playMusic();

#endif
