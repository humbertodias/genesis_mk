#include <genesis.h>
#include "bonus_stage_room.h"
#include "stages.h"
#include "game_vars.h"


void processBonusStage()
{

    while(TRUE){

        gFrames++;

        if (gFrames == 1)
        {
            VDP_loadTileSet(tym_bga.tileset, gInd_tileset, DMA);
            VDP_setTileMapEx(BG_A, tym_bga.tilemap,
                             TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA);
            PAL_setPalette(PAL0, tym_bga.palette->data, DMA);
            gInd_tileset += tym_bga.tileset->numTile;

            VDP_loadTileSet(tym_bgb.tileset, gInd_tileset, DMA);
            VDP_setTileMapEx(BG_B, tym_bgb.tilemap,
                             TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA);
            PAL_setPalette(PAL1, tym_bgb.palette->data, DMA);
            gInd_tileset += tym_bgb.tileset->numTile;
        }

        SPR_update();
        SYS_doVBlankProcess();
    }
}