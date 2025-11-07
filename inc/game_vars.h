#ifndef GAME_VARS_H
#define GAME_VARS_H

#include <genesis.h>
#include "estruturas.h"

extern u8 gAlturaDoPiso; // Altura do chão (posição Y dos personagens)
extern u16 gInd_tileset; // Variable used to load background data
extern u16 gBG_Width;    // Largura do Cenario em pixels
extern u16 gBG_Height;   // Altura do Cenario em pixels
extern s16 gScrollValue; // Scrolling de Cenario
extern u32 gFrames;      // Frame Counter

extern GraphicElement GE[25]; // Vetor global de elementos gráficos (GE)
extern Player player[2];      // Vetor global de jogadores (Player 1 e Player 2)

#endif
