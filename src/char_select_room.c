#include "char_select_room.h"
#include "stages.h"
#include "sprites.h"

Sprite *spr_p1_seletor; // Sprite de seleção de personagens
Sprite *spr_p2_seletor; // Sprite de seleção de personagens
Sprite *spr_p1_fighter; // Sprite de personagem no icone ativo
Sprite *spr_p2_fighter; // Sprite de personagem no icone ativo
u8 gP1Seletor, gP2Seletor;

// KANO      76, 44
// CAGE      20, 44
// SUBZERO  188, 44
// SONYA    244, 44
// SCORPION 188, 108
// LIU KANG 132, 108
// RAYDEN    76, 108
PlayerOption pOptions[7] = {
    {76, 44},
    {20, 44},
    {188, 44},
    {244, 44},
    {188, 108},
    {132, 108},
    {76, 108}};

void updateSelector(u8 *selector, Sprite *sprite, u16 curInput, u16 prevInput)
{
  if (*selector == 0)
  { // KANO
    if ((curInput & BUTTON_LEFT) && !(prevInput & BUTTON_LEFT))
    {
      SPR_setPosition(sprite, pOptions[1].x, pOptions[1].y);
      *selector = 1;
    }
    else if ((curInput & BUTTON_RIGHT) && !(prevInput & BUTTON_RIGHT))
    {
      SPR_setPosition(sprite, pOptions[2].x, pOptions[2].y);
      *selector = 2;
    }
    else if (curInput & BUTTON_DOWN)
    {
      SPR_setPosition(sprite, pOptions[6].x, pOptions[6].y);
      *selector = 6;
    }
  }
  else if (*selector == 1)
  { // cage
    if (curInput & BUTTON_RIGHT)
    {
      SPR_setPosition(sprite, pOptions[0].x, pOptions[0].y);
      *selector = 0;
    }
  }
  else if (*selector == 2)
  { // subzero
    if (curInput & BUTTON_LEFT)
    {
      SPR_setPosition(sprite, pOptions[0].x, pOptions[0].y);
      *selector = 0;
    }
    else if (curInput & BUTTON_RIGHT)
    {
      SPR_setPosition(sprite, pOptions[3].x, pOptions[3].y);
      *selector = 3;
    }
    else if (curInput & BUTTON_DOWN)
    {
      SPR_setPosition(sprite, pOptions[4].x, pOptions[4].y);
      *selector = 4;
    }
  }
  else if (*selector == 3)
  { // sonya
    if (curInput & BUTTON_LEFT)
    {
      SPR_setPosition(sprite, pOptions[2].x, pOptions[2].y);
      *selector = 2;
    }
  }
  else if (*selector == 4)
  { // SCORPION
    if (curInput & BUTTON_LEFT)
    {
      SPR_setPosition(sprite, pOptions[5].x, pOptions[5].y);
      *selector = 5;
    }
    else if (curInput & BUTTON_UP)
    {
      SPR_setPosition(sprite, pOptions[2].x, pOptions[2].y);
      *selector = 2;
    }
  }
  else if (*selector == 5)
  { // LIU
    if (curInput & BUTTON_LEFT)
    {
      SPR_setPosition(sprite, pOptions[6].x, pOptions[6].y);
      *selector = 6;
    }
    else if (curInput & BUTTON_RIGHT)
    {
      SPR_setPosition(sprite, pOptions[4].x, pOptions[4].y);
      *selector = 4;
    }
  }
  else if (*selector == 6)
  { // RAYDEN
    if (curInput & BUTTON_UP)
    {
      SPR_setPosition(sprite, pOptions[0].x, pOptions[0].y);
      *selector = 0;
    }
    else if (curInput & BUTTON_RIGHT)
    {
      SPR_setPosition(sprite, pOptions[5].x, pOptions[5].y);
      *selector = 5;
    }
  }
}

// void desenrolarBackground() { 
//   for (u16 y = 0; y < 28; y+=4) { 
//     VDP_setTileMapEx(BG_A, stage_char_select.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, y, 0, y, 40, 4, DMA_QUEUE); 
//     PAL_setPalette(PAL1, stage_char_select.palette->data, DMA); 
//     SYS_doVBlankProcess(); // Sincroniza com o VBlank para criar o efeito gradual
//   }
// }

void processSelecaoPersonagens()
{
  if (gFrames == 1)
  {
    VDP_setBackgroundColor(0);
    VDP_loadTileSet(stage_char_select_a.tileset, gInd_tileset, DMA);
    VDP_setTileMapEx(BG_A, stage_char_select_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
    // VDP_loadTileSet(stage_char_select.tileset, gInd_tileset, DMA);
    // VDP_setTileMapEx(BG_A, stage_char_select.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    // PAL_setPalette(PAL1, stage_char_select.palette->data, DMA);
    // desenrolarBackground();
    gInd_tileset += stage_char_select_a.tileset->numTile;

    VDP_loadTileSet(stage_char_select_b.tileset, gInd_tileset, DMA);
    VDP_setTileMapEx(BG_B, stage_char_select_b.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    gInd_tileset += stage_char_select_b.tileset->numTile;

    spr_p1_seletor = SPR_addSprite(&player_seletor, pOptions[0].x, pOptions[0].y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    SPR_setDepth(spr_p1_seletor, 1);
    gP1Seletor = 0;

    spr_p2_seletor = SPR_addSprite(&player_seletor, pOptions[2].x, pOptions[2].y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    SPR_setAnim(spr_p2_seletor, 1);
    SPR_setDepth(spr_p2_seletor, 2);
    gP2Seletor = 2;
  }

  curInputP1 = JOY_readJoypad(JOY_1);
  curInputP2 = JOY_readJoypad(JOY_2);

  updateSelector(&gP1Seletor, spr_p1_seletor, curInputP1, prevInputP1);
  updateSelector(&gP2Seletor, spr_p2_seletor, curInputP2, prevInputP2);

  if (gP1Seletor == 2 && !spr_p1_fighter)
  {

    spr_p1_fighter = SPR_addSprite(&spr_subzero, 7, 61, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL2, spr_subzero.palette->data, DMA);
    SPR_setAnim(spr_p1_fighter, 0);
    SPR_setDepth(spr_p1_fighter, 0);
    // SPR_update();
  }
  else if (gP1Seletor!= 2 && spr_p1_fighter)
  {
    SPR_releaseSprite(spr_p1_fighter);
    spr_p1_fighter = NULL;
  }

    if (gP2Seletor == 2 && !spr_p2_fighter)
  {

    spr_p2_fighter = SPR_addSprite(&spr_subzero, 153, 61, TILE_ATTR(PAL3, FALSE, FALSE, TRUE));
    PAL_setPalette(PAL3, spr_subzero.palette->data, DMA);
    SPR_setAnim(spr_p2_fighter, 0);
    SPR_setDepth(spr_p2_fighter, 0);
    // SPR_update();
  }
  else if (gP2Seletor!= 2 && spr_p2_fighter)
  {
    SPR_releaseSprite(spr_p2_fighter);
    spr_p2_fighter = NULL;
  }
}
