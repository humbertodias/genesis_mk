/**
 *  \file typewriter_printer.h
 *  \brief 
 *  \author Nil Obermuller Schaupp
 */
#ifndef _TYPEWRITER_P_H_
#define _TYPEWRITER_P_H_

#include "string.h"

#include "types.h"
#include "sys.h"
#include "vdp_tile.h"

/**
 * @brief
 *
 * @param text The string
 * @param x X position (in tile).
 * @param y Y position (in tile).
 * @param delay time to delay
 * @param plane Plane where we want to load tilemap.<br>
 *      Accepted values are:<br>
 *      - BG_A<br>
 *      - BG_B<br>
 *      - WINDOW<br> - not tested yet
 * @param numPal Palette number: PAL0, PAL1, PAL2 or PAL3
 */
void typewriterEffect(const char *text, u16 x, u16 y, u16 delay, VDPPlane plane, u16 numPal)
{
    u16 len = strlen(text);
    for (u16 i = 0; i < len; i++)
    {
        VDP_setTileMapXY(plane, TILE_ATTR_FULL(numPal, TRUE, FALSE, FALSE, TILE_FONT_INDEX + text[i] - ' '), x + i, y);
        SYS_doVBlankProcess(); // Espera o V-Blank
        for (u16 j = 0; j < delay; j++)
        {
            SYS_doVBlankProcess(); // Atraso adicional
        }
    }
}

#endif