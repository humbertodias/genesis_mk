#include <genesis.h>

#include "intro_demo_room.h"
#include "typewriter_printer.h"
#include "estruturas.h"
#include "stages.h"
#include "gfx.h"
#include "sprites.h"
#include "sound.h"
#include "bio_textlines.h"
#include "game_vars.h"

Sprite *bioAnimation;
Sprite *headName;

void loadBrainAtWorkScreen();
void loadMidwayTitleMKScreen();
void loadGoroLivesScreen();
void loadBioScreen();

void clearVDP()
{
    SYS_disableInts();
    // VDP_clearPlane(BG_A, TRUE);
    // VDP_clearPlane(BG_B, TRUE);
    VDP_resetScreen();
    SYS_enableInts();
    gInd_tileset = 0;
}

void processIntro()
{
    loadBrainAtWorkScreen();

    loadMidwayTitleMKScreen();

    loadGoroLivesScreen();

    if (gFrames == 1200)
    {
        VDP_waitVSync();
        loadBioScreen();
    }
}

// TELA GORO LIVES
void loadGoroLivesScreen()
{
    if (gFrames == 700)
    {
        clearVDP();

        SND_PCM4_startPlay(mus_goro_lives, sizeof(mus_goro_lives), SOUND_PCM_CH1, FALSE);

        VDP_loadTileSet(goro_lives.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, goro_lives.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, goro_lives.palette->data, DMA);
        gInd_tileset += goro_lives.tileset->numTile;
    }

    // TELA GORO E HISTORIA
    if (gFrames == 855)
    {
        clearVDP();

        PAL_setColors(0, palette_black, 64, DMA);
        VDP_loadTileSet(goro_a.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, goro_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, goro_a.palette->data, DMA);
        gInd_tileset += goro_a.tileset->numTile;

        VDP_setTextPlane(BG_B);
        VDP_loadFontData(font_a.tiles, font_a.numTile, CPU);
        PAL_setPalette(PAL1, font_a_pal.data, DMA);
        VDP_setTextPalette(PAL1);
    }

    if (gFrames == 930)
    {
        // Fade da tela até o indice 3
        // PAL_fadeTo(0, 3, palette_black, 30, FALSE);

        for (u16 i = 0; i < sizeof(goroLines) / sizeof(goroLines[0]); i++)
        {
            typewriterEffect(goroLines[i].text, goroLines[i].x, goroLines[i].y, 0, BG_B, PAL1);
        }
    }

    if (gFrames == 1180)
    {
        for (u16 volume = 15; volume > 0; volume--)
        {
            SND_PCM4_setVolume(SOUND_PCM_CH1, volume);
            // VDP_waitVSync(); // Espera pelo próximo frame
        }
        SND_PCM4_stopPlay(SOUND_PCM_CH1);

        clearVDP();
        PAL_setColors(0, palette_black, 64, DMA);
    }
}

// TELA BRAIN AT WORK
void loadBrainAtWorkScreen()
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

        PAL_fadeIn(0, 15, baw_a.palette->data, 8, FALSE);
        PAL_fadeIn(16, 30, baw_b.palette->data, 8, FALSE);
    }

    if (gFrames == 200)
    {
        PAL_fadeOut(0, 15, 5, FALSE);
        PAL_fadeOut(16, 30, 5, FALSE);

        VDP_setBackgroundColor(0);

        clearVDP();
    }
}

void loadMidwayTitleMKScreen()
{
    // TELA MIDWAY
    if (gFrames == 230)
    {
        XGM2_playPCM(mus_midway, sizeof(mus_midway), SOUND_PCM_CH1_MSK);

        VDP_loadTileSet(midway.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, midway.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, midway.palette->data, DMA);
        gInd_tileset += midway.tileset->numTile;
    }

    // TELA TITULO MK
    if (gFrames == 370)
    {
        XGM2_playPCM(mus_title, sizeof(mus_title), SOUND_PCM_CH1_MSK);

        VDP_loadTileSet(mk_title.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, mk_title.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, mk_title.palette->data, DMA);
        gInd_tileset += mk_title.tileset->numTile;

        VDP_loadTileSet(mk_title_b.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_B, mk_title_b.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL1, mk_title_b.palette->data, DMA);
        gInd_tileset += mk_title_b.tileset->numTile;
    }
}

// TODO: Parametrizar esta função
void loadBioScreen()
{
    // plano de fundo
    VDP_loadTileSet(bio_b.tileset, gInd_tileset, DMA);
    VDP_setTileMapEx(BG_A, bio_b.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    PAL_setPalette(PAL0, bio_b.palette->data, DMA);
    gInd_tileset += bio_b.tileset->numTile;

    bioAnimation = SPR_addSprite(&scorpion_bio, 112, 48, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL1, scorpion_bio.palette->data, DMA);
    SPR_setAnimationLoop(bioAnimation, FALSE);
    // SPR_setAnim(bioAnimation, 0);
    SPR_setDepth(bioAnimation, 0);

    u16 size_name = (VDP_getScreenWidth() / 2) - (scorpion_name.w / 2);

    // nome do personagem
    headName = SPR_addSprite(&scorpion_name, size_name, 16, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL2, scorpion_name.palette->data, DMA);

    VDP_setTextPlane(BG_B);
    VDP_loadFontData(font_a.tiles, font_a.numTile, CPU);
    PAL_setPalette(PAL3, font_a_pal.data, DMA);
    VDP_setTextPalette(PAL3);

    XGM2_playPCM(loc_jc, sizeof(loc_jc), SOUND_PCM_CH2);
    typewriterWriteAllLines(scorpionLines, sizeof(scorpionLines) / sizeof(scorpionLines[0]), BG_B, PAL3);

    XGM2_play(mus_the_beginning);
}
