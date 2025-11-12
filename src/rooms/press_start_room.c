#include <genesis.h>

#include "press_start_room.h"
#include "stages.h"
#include "sprites.h"
#include "game_vars.h"

// Variáveis Locais
s16 vecTilesScreen[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
s16 vecAceleracao[20] = {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2};

Sprite *pressStart;

void clearVDP1()
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

void processPressStart()
{
    gFrames++;
    if (gFrames == 1)
    {
        clearVDP1();
    }
    if (gFrames == 5)
    {
        VDP_setPlaneSize(64, 64, TRUE);

        VDP_loadTileSet(screen_opt_a.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, screen_opt_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 64, DMA);
        PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
        gInd_tileset += screen_opt_a.tileset->numTile;

        VDP_loadTileSet(screen_opt_b.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_B, screen_opt_b.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 64, DMA);
        PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
        gInd_tileset += screen_opt_b.tileset->numTile;

        VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_COLUMN);

        // if(pressStart){
        //     SPR_releaseSprite(pressStart);
        //     pressStart = NULL;
        // } else {
        //     pressStart = SPR_addSprite(&spPressStart, 96, 184, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        //     PAL_setPalette(PAL2, spPressStart.palette->data, DMA);
        //     SPR_setAnim(pressStart, 0);
        //     SPR_update();
        // }
    }

    for (int i = 0; i < 20; i++)
    {
        vecTilesScreen[i] -= vecAceleracao[i];
    }

    VDP_setVerticalScrollTile(BG_A, 0, vecTilesScreen, 20, DMA_QUEUE);
    VDP_setVerticalScrollTile(BG_B, 0, vecTilesScreen, 20, DMA_QUEUE);
}