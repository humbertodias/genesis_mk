#include <genesis.h>

#include "press_start_room.h"
#include "input_system.h"
#include "stages.h"
#include "sprites.h"
#include "game_vars.h"

#define ACELERACAO 2
// Variáveis Locais
static s16 vecTilesScreen[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// static s16 vecAceleracao[20] = {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2};

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
    bool sair = FALSE;
    // char stri[64];
    while (!sair)
    {
        newInputSystem();

        gFrames++;

        if (gFrames == 5)
        {
            VDP_setPlaneSize(64, 64, TRUE);

            VDP_loadTileSet(screen_opt_a.tileset, gInd_tileset, DMA);
            VDP_setTileMapEx(BG_A, screen_opt_a.tilemap,
                             TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 64, CPU);
            PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
            gInd_tileset += screen_opt_a.tileset->numTile;

            VDP_loadTileSet(screen_opt_b.tileset, gInd_tileset, DMA);
            VDP_setTileMapEx(BG_B, screen_opt_b.tilemap,
                             TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 64, CPU);
            PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
            gInd_tileset += screen_opt_b.tileset->numTile;

            VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_COLUMN);
        }

        if ((player[0].key_JOY_START_status > 0 || player[1].key_JOY_START_status > 0) && gFrames > 50)
        {
            sair = TRUE;
        }

        // TODO: testar performance
        // for (int i = 0; i < 20; i++)
        // {
        //     vecTilesScreen[i] -= vecAceleracao[i];
        // }
        vecTilesScreen[0] -= ACELERACAO;
        vecTilesScreen[1] -= ACELERACAO;
        vecTilesScreen[2] -= ACELERACAO;
        vecTilesScreen[17] -= ACELERACAO;
        vecTilesScreen[18] -= ACELERACAO;
        vecTilesScreen[19] -= ACELERACAO;

        if (vecTilesScreen[0] == -512)
        {
            vecTilesScreen[0] = 0;
            vecTilesScreen[1] = 0;
            vecTilesScreen[2] = 0;
            vecTilesScreen[17] = 0;
            vecTilesScreen[18] = 0;
            vecTilesScreen[19] = 0;
        }

        // sprintf(stri, "p1: %d", vecTilesScreen[0]);
        // VDP_drawText(stri, 7, 1);
        VDP_setVerticalScrollTile(BG_A, 0, vecTilesScreen, 20, CPU);
        VDP_setVerticalScrollTile(BG_B, 0, vecTilesScreen, 20, CPU);
        // SPR_update();
        SYS_doVBlankProcess();
    }

    SPR_clear();
    PAL_fadeOut(0, 15, 5, FALSE);
    PAL_fadeOut(16, 30, 5, FALSE);
    PAL_setColors(0, palette_black, 64, DMA);
    gFrames = 0;
    gRoom = SELECAO_PERSONAGENS;
    //--
    VDP_waitVSync();
    SYS_disableInts();
    VDP_resetScreen();
    // VDP_clearPlane(BG_A, TRUE);
    // VDP_clearPlane(BG_B, TRUE);
    // VDP_setBackgroundColor(0); // Define preto
    gInd_tileset = TILE_USER_INDEX;
    SYS_enableInts();
}