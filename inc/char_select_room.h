#ifndef CHAR_SELECT_ROOM_H 
#define CHAR_SELECT_ROOM_H

#include "types.h"
#include "dma.h"
#include "vdp.h"
#include "vdp_bg.h"
#include "sprite_eng.h"
#include "joy.h"

#include "estruturas.h"

// Declaração das variáveis globais como extern 
extern u32 gFrames;
extern u16 gInd_tileset;
//extern PlayerOption pOptions[7];
extern u16 prevInputP1, prevInputP2;
extern u16 curInputP1, curInputP2;

void processSelecaoPersonagens();

#endif
