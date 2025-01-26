#include <genesis.h>

#include "estruturas.h"
#include "char_select_room.h"
#include "stages.h"
#include "gfx.h"
#include "typewriter_printer.h"

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
u16 palette[64];

struct
{
  const char *text;
  u16 x;
  u16 y;
} lines[] = {
    // {"GORO LIVES...", 14, 4},
    {"A 2.000 YEAR OLD HALF HUMAN DRAGON", 3, 7},
    {"GORO REMAINS UNDERFEATED FOR THE PAST", 2, 9},
    {"500 YEARS. HE WON THE TITLE OF GRAND", 2, 11},
    {"CHAMPION BY DEFEATING KUNG LAO, A", 3, 13},
    {"SHAOLIN FIGHTING MONK. IT WAS DURING", 2, 15},
    {"THIS PERIOD THAT TOURNAMENT FELL", 2, 17},
    {"INTO SHANG TSUNG'S HANDS AND WAS", 4, 19},
    {"CORRUPTED", 15, 21},
};


void CLEAR_VDP()
{
  SYS_disableInts();
  // SPR_reset();
  // VDP_resetSprites();
  // VDP_releaseAllSprites();
  // SPR_defragVRAM();
  VDP_clearPlane(BG_A, TRUE);
  VDP_clearPlane(BG_B, TRUE);
  // VDP_setTextPlane(BG_B);
  // VDP_setHorizontalScroll(BG_B, 0);
  // VDP_setVerticalScroll(BG_B, 0);
  // VDP_setHorizontalScroll(BG_A, 0);
  // VDP_setVerticalScroll(BG_A, 0);
  // VDP_setBackgroundColor(0);
  VDP_resetScreen();
  // PAL_setColors(0, palette_black, 64, DMA);
  SYS_enableInts();
  gInd_tileset = 0;
}

int main(bool hardReset)
{
  SPR_init();
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  gRoom = TELA_START;
  gFrames = 0;
  gInd_tileset = 0;

  s16 vecTilesScreen[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  s16 vecAceleracao[20] = {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2};

  while (TRUE)
  {
    gFrames++;

    if (gRoom == TELA_START)
    {
      if (gFrames == 1)
      {
        VDP_loadTileSet(baw_a.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, baw_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, baw_a.palette->data, DMA);
        gInd_tileset += baw_a.tileset->numTile;

        VDP_loadTileSet(baw_b.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_B, baw_b.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL1, baw_b.palette->data, DMA);
        gInd_tileset += baw_b.tileset->numTile;
      }

      if (gFrames == 200)
      {
        CLEAR_VDP();

        VDP_loadTileSet(midway.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, midway.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, midway.palette->data, DMA);
        gInd_tileset += midway.tileset->numTile;
      }

      if (gFrames == 400)
      {
        // CLEAR_VDP();

        VDP_loadTileSet(mk_title.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, mk_title.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, mk_title.palette->data, DMA);
        gInd_tileset += mk_title.tileset->numTile;
      }

      if(gFrames == 500)
      {
        VDP_loadTileSet(goro_lives.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, goro_lives.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, goro_lives.palette->data, DMA);
        gInd_tileset += goro_lives.tileset->numTile;
      }

      if (gFrames == 600)
      {
        CLEAR_VDP();
        
        PAL_setColors(0, palette_black, 64, DMA);
        VDP_loadTileSet(goro_a.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, goro_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, goro_a.palette->data, DMA);
        gInd_tileset += goro_a.tileset->numTile;

        VDP_setTextPlane(BG_B);
        VDP_loadFontData(font_a.tiles, font_a.numTile, CPU);
        PAL_setPalette(PAL1, font_a_pal.data, DMA);
        VDP_setTextPalette(PAL1);

        for (u16 i = 0; i < sizeof(lines)/ sizeof(lines[0]); i++)
        {
          typewriterEffect(lines[i].text, lines[i].x, lines[i].y, 2, BG_B, PAL1);
        }
      }
    }

    if (gRoom == TELA_TITULO)
    {
      CLEAR_VDP();
      if (gFrames == 800)
      {
        VDP_setPlaneSize(64, 64, TRUE);

        VDP_drawImageEx(BG_A, &screen_opt_a, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, FALSE, DMA);
        PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
        gInd_tileset += screen_opt_a.tileset->numTile;

        // VDP_loadTileSet(screen_opt_b.tileset, gInd_tileset, DMA_QUEUE);
        // VDP_setTileMapEx(BG_B, screen_opt_b.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 50, DMA_QUEUE);
        VDP_drawImageEx(BG_B, &screen_opt_b, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, DMA);
        PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
        gInd_tileset += screen_opt_b.tileset->numTile;

        VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_COLUMN);
      }
      for (int i = 0; i < 20; i++)
      {
        vecTilesScreen[i] -= vecAceleracao[i];
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