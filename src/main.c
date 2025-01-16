#include <genesis.h>

#include "estruturas.h"
#include "char_select_room.h"

// #include "stages.h"
// #include "sprites.h"

// -- DECLARACAO DE VARIAVEIS -- //

u8 gRoom;         // Sala atual
u32 gFrames;      // Frame Counter
u16 gInd_tileset; // Variable used to load background data
u16 prevInputP1 = 0, prevInputP2 = 0;
u16 curInputP1, curInputP2;
// Sprite *spr_p1_seletor; // Sprite de seleção de personagens
// Sprite *spr_p2_seletor; // Sprite de seleção de personagens
// u8 gP1Seletor, gP2Seletor;

// -- ESTRUTURAS -- //
// typedef struct
// {
//   u16 x;
//   u16 y;
// } PlayerOption;

// // KANO      76, 44
// // CAGE      20, 44
// // SUBZERO  188, 44
// // SONYA    244, 44
// // SCORPION 188, 108
// // LIU KANG 132, 108
// // RAYDEN    76, 108
// PlayerOption options[7] = {
//     {76, 44},
//     {20, 44},
//     {188, 44},
//     {244, 44},
//     {188, 108},
//     {132, 108},
//     {76, 108}
// };

int main(bool hardReset)
{
  SPR_init();
  gRoom = SELECAO_PERSONAGENS;
  gFrames = 0;
  gInd_tileset = 0;

  while (TRUE)
  {
    gFrames++;

    if (gRoom == SELECAO_PERSONAGENS)
    {
      processSelecaoPersonagens();
      // if (gFrames == 1)
      // {
      //   VDP_loadTileSet(stage_char_select.tileset, gInd_tileset, DMA);
      //   VDP_setTileMapEx(BG_A, stage_char_select.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
      //   PAL_setPalette(PAL0, stage_char_select.palette->data, DMA);
      //   gInd_tileset += stage_char_select.tileset->numTile;

      //   // gInd_tileset = loadCharSelectRoom(gInd_tileset);

      //   spr_p1_seletor = SPR_addSprite(&player_seletor, pOptions[0].x, pOptions[0].y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
      //   PAL_setPalette(PAL2, player_seletor.palette->data, DMA);
      //   gP1Seletor = 0;

      //   // p1Seletor = initPlayerOneSelector();

      //   spr_p2_seletor = SPR_addSprite(&player_seletor, pOptions[2].x, pOptions[2].y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
      //   PAL_setPalette(PAL2, player_seletor.palette->data, DMA);
      //   SPR_setAnim(spr_p2_seletor, 1);
      //   gP2Seletor = 2;
      // }

      // curInput = JOY_readJoypad(JOY_1);

      // // inputSeletor(JOY_readJoypad(JOY_1), prevInput, p1Seletor, spr_p1_seletor);

      // if (gP1Seletor == 0)
      // { // KANO

      //   if ((curInput & BUTTON_LEFT) && !(prevInput & BUTTON_LEFT))
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[1].x, pOptions[1].y);
      //     gP1Seletor = 1;
      //   }
      //   else if ((curInput & BUTTON_RIGHT) && !(prevInput & BUTTON_RIGHT))
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[2].x, pOptions[2].y);
      //     gP1Seletor = 2;
      //   }
      //   else if (curInput & BUTTON_DOWN)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[6].x, pOptions[6].y);
      //     gP1Seletor = 6;
      //   }
      // }
      // else if (gP1Seletor == 1)
      // { // cage
      //   if (curInput & BUTTON_RIGHT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[0].x, pOptions[0].y);
      //     gP1Seletor = 0;
      //   }
      // }
      // else if (gP1Seletor == 2)
      // { // subzero
      //   if (curInput & BUTTON_LEFT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[0].x, pOptions[0].y);
      //     gP1Seletor = 0;
      //   }
      //   else if (curInput & BUTTON_RIGHT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[3].x, pOptions[3].y);
      //     gP1Seletor = 3;
      //   }
      //   else if (curInput & BUTTON_DOWN)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[4].x, pOptions[4].y);
      //     gP1Seletor = 4;
      //   }
      // }
      // else if (gP1Seletor == 3)
      // { // sonya
      //   if (curInput & BUTTON_LEFT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[2].x, pOptions[2].y);
      //     gP1Seletor = 2;
      //   }
      // }
      // else if (gP1Seletor == 4)
      // { // SCORPION
      //   if (curInput & BUTTON_LEFT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[5].x, pOptions[5].y);
      //     gP1Seletor = 5;
      //   }
      //   else if (curInput & BUTTON_UP)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[2].x, pOptions[2].y);
      //     gP1Seletor = 2;
      //   }
      // }
      // else if (gP1Seletor == 5)
      // { // LIU
      //   if (curInput & BUTTON_LEFT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[6].x, pOptions[6].y);
      //     gP1Seletor = 6;
      //   }
      //   else if (curInput & BUTTON_RIGHT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[4].x, pOptions[4].y);
      //     gP1Seletor = 4;
      //   }
      // }
      // else if (gP1Seletor == 6)
      // { // RAYDEN
      //   if (curInput & BUTTON_UP)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[0].x, pOptions[0].y);
      //     gP1Seletor = 0;
      //   }
      //   else if (curInput & BUTTON_RIGHT)
      //   {
      //     SPR_setPosition(spr_p1_seletor, pOptions[5].x, pOptions[5].y);
      //     gP1Seletor = 5;
      //   }
      // }
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