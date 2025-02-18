#include <genesis.h>

#include "estruturas.h"
#include "char_select_room.h"
#include "intro_demo_room.h"
#include "press_start_room.h"
#include "stages.h"
#include "gfx.h"

#define DEBUG TRUE

// -- DECLARACAO DE VARIAVEIS -- //
u8 gRoom;         // Sala atual
u32 gFrames;      // Frame Counter
u16 gInd_tileset; // Variable used to load background data
u16 prevInputP1 = 0, prevInputP2 = 0;
u16 gBG_Width;
u16 gBG_Height;
s16 gScrollValue; // Scrolling de Cenario
u16 curInputP1, curInputP2;
u16 palette[64];
Map *level_map;
Sprite *spr_p1; // Sprite de personagem no icone ativo
Sprite *spr_p2; // Sprite de personagem no icone ativo
s16 scrollOffset = 0;
s16 scrollValues[48];

void CLEAR_VDP();

int main(bool hardReset)
{
  SPR_init();
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  gRoom = PALACE_GATES;
  gFrames = 0;
  gInd_tileset = 0;

  while (TRUE)
  {
    gFrames++;

    if (TELA_DEMO_INTRO == gRoom)
    {
      // enquanto não pressionar start...
      processIntro();
    }

    if (TELA_TITULO == gRoom)
    {
      processPressStart();
    }

    if (SELECAO_PERSONAGENS == gRoom)
    {
      processSelecaoPersonagens();
    }

    if (PALACE_GATES == gRoom)
    {
      if (gFrames == 1)
      {
        CLEAR_VDP();

        // VDP_loadTileSet(&pg_bga_tileset, gInd_tileset, DMA);
        // level_map = MAP_create(&pg_bga_map, BG_A, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset));
        // PAL_setPalette(PAL0, pg_bga_pal.data, DMA);
        // gInd_tileset += pg_bga_tileset.numTile;
        // MAP_scrollTo(level_map, 464, 10);

        gBG_Width = 928;
        gBG_Height = 232;
        gScrollValue = (gBG_Width - VDP_getScreenWidth()) / 2;

        s16 startX = (VDP_getScreenWidth() - gBG_Width) / 2;
        s16 startY = -(gBG_Height - VDP_getScreenHeight());

        VDP_loadTileSet(pg_bga.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, pg_bga.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), startX / 8, startY / 8, 0, 0, gBG_Width / 8, 29, DMA);
        PAL_setPalette(PAL0, pg_bga.palette->data, DMA);
        gInd_tileset += pg_bga.tileset->numTile;

        VDP_loadTileSet(pg_bgb.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_B, pg_bgb.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, startY / 8, 0, 0, 64, 29, DMA);
        PAL_setPalette(PAL1, pg_bgb.palette->data, DMA);
        gInd_tileset += pg_bgb.tileset->numTile;

        spr_p1 = SPR_addSprite(&spr_subzero, 24, 96, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        PAL_setPalette(PAL2, spr_subzero.palette->data, DMA);

        spr_p2 = SPR_addSprite(&spr_subzero, 168, 96, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
        PAL_setPalette(PAL3, spr_subzero.palette->data, DMA);
        SPR_setHFlip(spr_p2, TRUE);
      }

      scrollOffset += 1;
      VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_COLUMN);

      for (int i = 0; i < 48; i++)
      {
        scrollValues[i] = scrollOffset;
      }

      VDP_setHorizontalScrollLine(BG_B, 0, scrollValues, 48, CPU);
    }

    // -- DEBUG -- //
    if (DEBUG)
    {
      char str[64];
      sprintf(str, "tiles nos BGs: %d", gInd_tileset);
      VDP_drawText(str, 1, 1);
      // sprintf(str, "contagem de frames: %ld", gFrames);
      // VDP_drawText(str, 1, 2);
    }

    // -- FINALIZAÇÕES -- //

    SPR_update();          // Atualização dos sprites na tela
    SYS_doVBlankProcess(); // Sincroniza com o VBlank
  }

  return 0;
}

void CLEAR_VDP()
{
  SYS_disableInts();
  SPR_reset();
  VDP_resetSprites();
  VDP_releaseAllSprites();
  SPR_defragVRAM();
  VDP_clearPlane(BG_A, TRUE);
  VDP_clearPlane(BG_B, TRUE);
  // VDP_setTextPlane(BG_B);
  // VDP_setHorizontalScroll(BG_B, 0);
  // VDP_setVerticalScroll(BG_B, 0);
  // VDP_setHorizontalScroll(BG_A, 0);
  // VDP_setVerticalScroll(BG_A, 0);
  VDP_setBackgroundColor(0);
  VDP_resetScreen();
  // PAL_setColors(0, palette_black, 64, DMA);
  SYS_enableInts();
  gInd_tileset = 0;
}