#include <genesis.h>

#include "estruturas.h"
#include "char_select_room.h"
#include "stages.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define FULL_SCREEN_HEIGHT 392
#define SCROLL_SPEED 1

#define DEBUG FALSE

// -- DECLARACAO DE VARIAVEIS -- //

u8 gRoom;         // Sala atual
u32 gFrames;      // Frame Counter
u16 gInd_tileset; // Variable used to load background data
u16 prevInputP1 = 0, prevInputP2 = 0;
u16 curInputP1, curInputP2;

int main(bool hardReset)
{
  SPR_init();
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  gRoom = TELA_START;
  gFrames = 0;
  gInd_tileset = 0;

  VDP_setPlaneSize(64, 64, TRUE);
  // VDP_loadTileSet(screen_opt_a.tileset, gInd_tileset, DMA_QUEUE);
  // VDP_setTileMapEx(BG_A, screen_opt_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 50, DMA_QUEUE);
  VDP_drawImageEx(BG_A, &screen_opt_a, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, FALSE, DMA);
  PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
  gInd_tileset += screen_opt_a.tileset->numTile;

  // VDP_loadTileSet(screen_opt_b.tileset, gInd_tileset, DMA_QUEUE);
  // VDP_setTileMapEx(BG_B, screen_opt_b.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 50, DMA_QUEUE);
  VDP_drawImageEx(BG_B, &screen_opt_b, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, DMA);
  PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
  gInd_tileset += screen_opt_b.tileset->numTile;

  VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_COLUMN);

  s16 vecTilesScreen[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  s16 vecAceleracao[20] = {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2};

  while (TRUE)
  {
    gFrames++;

    if (gRoom == TELA_START)
    {
      for (int i = 0; i < 20; i++)
      {
        vecTilesScreen[i] += vecAceleracao[i];
      }

      VDP_setVerticalScrollTile(BG_A, 0, vecTilesScreen, 20, DMA_QUEUE);
      VDP_setVerticalScrollTile(BG_B, 0, vecTilesScreen, 20, DMA_QUEUE);
    }

    if (gRoom == SELECAO_PERSONAGENS)
    {
      processSelecaoPersonagens();
    }

    // -- DEBUG -- //
    if (DEBUG)
    {
      char str[64];
      sprintf(str, "tiles usados nos BGs: %d", gInd_tileset);
      VDP_drawText(str, 1, 1);
      sprintf(str, "contagem de frames: %ld", gFrames);
      VDP_drawText(str, 1, 2);
    }

    // -- FINALIZAÇÕES -- //

    SPR_update();          // Atualização dos sprites na tela
    SYS_doVBlankProcess(); // Sincroniza com o VBlank
  }

  return 0;
}