#include "game_vars.h"
#include "estruturas.h"

// ===============================
// Definições das variáveis globais
// ===============================

bool debugEnabled;
u8 gRoom;
u8 gAlturaDoPiso;
u16 gInd_tileset;
u16 gBG_Width;
u16 gBG_Height;
s16 gScrollValue;
u32 gFrames;
bool gASG_system;

// Elementos gráficos globais como cursores, sparks, etc.
GraphicElement GE[25];

Player player[2];