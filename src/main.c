#include <genesis.h>

#include "stages.h"
#include "sprites.h"

// -- DECLARACAO DE VARIAVEIS -- //

u8 gRoom;         // Sala atual
u32 gFrames;      // Frame Counter
u16 gInd_tileset; // Variable used to load background data
u16 prevInput = 0;
u16 curInput = 0;
Sprite *spr_p1_seletor; // Sprite de seleção de personagens
Sprite *spr_p2_seletor; // Sprite de seleção de personagens
u8 gP1Seletor, gP2Seletor;

// -- ESTRUTURAS -- //
typedef struct
{
  u16 x;
  u16 y;
  u8 index;
} PlayerOption;

// KANO      76, 44
// CAGE      20, 44
// SUBZERO  188, 44
// SONYA    244, 44
// SCORPION 188, 108
// LIU KANG 132, 108
// RAYDEN    76, 108
PlayerOption options[7] = {
    {76, 44, 0},
    {20, 44, 1},
    {188, 44, 2},
    {244, 44, 3},
    {188, 108, 4},
    {132, 108, 5},
    {76, 108, 6}};

int main(bool hardReset)
{
  SPR_init();
  gRoom = 1;
  gFrames = 0;
  gInd_tileset = 0;

  while (TRUE)
  {
    gFrames++;

    if (gRoom == 1)
    {
      if (gFrames == 1)
      {
        VDP_loadTileSet(stage_char_select.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, stage_char_select.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, stage_char_select.palette->data, DMA);
        gInd_tileset += stage_char_select.tileset->numTile;

        spr_p1_seletor = SPR_addSprite(&player_seletor, options[0].x, options[0].y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        PAL_setPalette(PAL2, player_seletor.palette->data, DMA);
        gP1Seletor = 0;

        spr_p2_seletor = SPR_addSprite(&player_seletor, options[2].x, options[2].y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        PAL_setPalette(PAL2, player_seletor.palette->data, DMA);
        SPR_setAnim(spr_p2_seletor, 1);
        gP2Seletor = 2;
      }

      curInput = JOY_readJoypad(JOY_1);

      if (gP1Seletor == 0)
      { // KANO

        if (curInput & BUTTON_LEFT)
        {
          SPR_setPosition(spr_p1_seletor, options[1].x, options[1].y);
          gP1Seletor = 1;
        }
        else if (curInput & BUTTON_RIGHT)
        {
          SPR_setPosition(spr_p1_seletor, options[2].x, options[2].y);
          gP1Seletor = 2;
        }
        else if (curInput & BUTTON_DOWN)
        {
          SPR_setPosition(spr_p1_seletor, options[6].x, options[6].y);
          gP1Seletor = 6;
        }
      }
      else if (gP1Seletor == 1)
      { // cage
        if (curInput & BUTTON_RIGHT)
        {
          SPR_setPosition(spr_p1_seletor, options[0].x, options[0].y);
          gP1Seletor = 0;
        }
      }
      else if (gP1Seletor == 2)
      { // subzero
        if (curInput & BUTTON_LEFT)
        {
          SPR_setPosition(spr_p1_seletor, options[0].x, options[0].y);
          gP1Seletor = 0;
        }
        else if (curInput & BUTTON_RIGHT)
        {
          SPR_setPosition(spr_p1_seletor, options[3].x, options[3].y);
          gP1Seletor = 3;
        }
        else if (curInput & BUTTON_DOWN)
        {
          SPR_setPosition(spr_p1_seletor, options[4].x, options[4].y);
          gP1Seletor = 4;
        }
      }
      else if (gP1Seletor == 3)
      { // sonya
        if (curInput & BUTTON_LEFT)
        {
          SPR_setPosition(spr_p1_seletor, options[2].x, options[2].y);
          gP1Seletor = 2;
        }
      }
      else if (gP1Seletor == 4)
      { // SCORPION
        if (curInput & BUTTON_LEFT)
        {
          SPR_setPosition(spr_p1_seletor, options[5].x, options[5].y);
          gP1Seletor = 5;
        }
        else if (curInput & BUTTON_UP)
        {
          SPR_setPosition(spr_p1_seletor, options[2].x, options[2].y);
          gP1Seletor = 2;
        }
      }
      else if (gP1Seletor == 5)
      { // LIU
        if (curInput & BUTTON_LEFT)
        {
          SPR_setPosition(spr_p1_seletor, options[6].x, options[6].y);
          gP1Seletor = 6;
        }
        else if (curInput & BUTTON_RIGHT)
        {
          SPR_setPosition(spr_p1_seletor, options[4].x, options[4].y);
          gP1Seletor = 4;
        }
      }
      else if (gP1Seletor == 6)
      { // RAYDEN
        if (curInput & BUTTON_UP)
        {
          SPR_setPosition(spr_p1_seletor, options[0].x, options[0].y);
          gP1Seletor = 0;
        }
        else if (curInput & BUTTON_RIGHT)
        {
          SPR_setPosition(spr_p1_seletor, options[5].x, options[5].y);
          gP1Seletor = 5;
        }
      }
    }

    // -- DEBUG -- //
    char str[64];
    sprintf(str, "tiles usados nos BGs: %d", gInd_tileset);
    VDP_drawText(str, 1, 1);
    sprintf(str, "contagem de frames: %ld", gFrames);
    VDP_drawText(str, 1, 2);

    // -- FINALIZAÇÕES -- //

    SPR_update();          // Atualização dos sprites na tela
    SYS_doVBlankProcess(); // Sincroniza com o VBlank
  }

  return 0;
}