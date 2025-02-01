#include "intro_demo_room.h"

Sprite *bioAnimation;
Sprite *headName;

TextLine goroLines[] = {
    {"A 2.000 YEAR OLD HALF HUMAN DRAGON", 3, 7},
    {"GORO REMAINS UNDERFEATED FOR THE PAST", 2, 9},
    {"500 YEARS. HE WON THE TITLE OF GRAND", 2, 11},
    {"CHAMPION BY DEFEATING KUNG LAO, A", 3, 13},
    {"SHAOLIN FIGHTING MONK. IT WAS DURING", 2, 15},
    {"THIS PERIOD THAT TOURNAMENT FELL", 2, 17},
    {"INTO SHANG TSUNG'S HANDS AND WAS", 4, 19},
    {"CORRUPTED", 15, 21},
};

TextLine jcLines[] = {
    {"A MARTIAL ARTS SUPERSTAR TRAINED BY", 3, 17},
    {"GREAT MASTERS FROM AROUND THE WORLD.", 2, 18},
    {"CAGE USES HIS TALENTS ON", 7, 19},
    {"THE BIG SCREEN,", 10, 20},
    {"HE IS THE CURRENT BOX-OFFICE CHAMP", 3, 21},
    {"AND STAR OF SUCH MOVIES AS DRAGON", 3, 22},
    {"FIST AND DRAGON FIST II AS WELL", 4, 23},
    {"AS THE AWARD", 13, 24},
    {"WINNING SUDDEN VIOLENCE.", 7, 25},
};

TextLine kanoLines[] = {
    {"A MERCENARY, THUG, EXTORTIONIST", 3, 17},
    {"THIEF - KANO LIVES A LIFE OF CRIME", 2, 18},
    {"AND INJUSTICE. HE IS A DEVOTED MEMBER", 7, 19},
    {"OF THE BLACK DRAGON, A DANGEROUS", 10, 20},
    {"GROUP OF CUT-THROAT MADMEN FEARED", 3, 21},
    {"AND RESPECTED THROUGHOUT ALL OF", 3, 22},
    {"CRIME'S INNER CIRCLES.", 4, 23},
};

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
    if (gFrames == 865)
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

void loadMidwayTitleMKScreen()
{
    // TELA MIDWAY
    if (gFrames == 230)
    { 
        XGM2_playPCM(mus_midway, sizeof(mus_midway), SOUND_PCM_CH_AUTO);

        VDP_loadTileSet(midway.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, midway.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, midway.palette->data, DMA);
        gInd_tileset += midway.tileset->numTile;
    }

    // TELA TITULO MK
    if (gFrames == 320)
    {
        VDP_loadTileSet(mk_title.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_A, mk_title.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL0, mk_title.palette->data, DMA);
        gInd_tileset += mk_title.tileset->numTile;

        VDP_loadTileSet(mk_title_b.tileset, gInd_tileset, DMA);
        VDP_setTileMapEx(BG_B, mk_title_b.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
        PAL_setPalette(PAL1, mk_title_b.palette->data, DMA);
        gInd_tileset += mk_title_b.tileset->numTile;

        XGM2_playPCM(mus_title, sizeof(mus_title), SOUND_PCM_CH_AUTO);
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

        clearVDP();
    }
}

void loadBioScreen()
{
    // plano de fundo
    VDP_loadTileSet(bio_b.tileset, gInd_tileset, DMA);
    VDP_setTileMapEx(BG_A, bio_b.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    PAL_setPalette(PAL0, bio_b.palette->data, DMA);
    gInd_tileset += bio_b.tileset->numTile;

    bioAnimation = SPR_addSprite(&jc_bio, 112, 48, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL1, jc_bio.palette->data, DMA);
    SPR_setAnimationLoop(bioAnimation, FALSE);
    // SPR_setAnim(bioAnimation, 0);
    SPR_setDepth(bioAnimation, 0);
    
    // nome do personagem
    headName = SPR_addSprite(&jc_name, 112, 16, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL2, jc_name.palette->data, DMA);

    VDP_setTextPlane(BG_B);
    VDP_loadFontData(font_a.tiles, font_a.numTile, CPU);
    PAL_setPalette(PAL3, font_a_pal.data, DMA);
    VDP_setTextPalette(PAL3);

    typewriterWriteAllLines(jcLines, sizeof(jcLines) / sizeof(jcLines[0]), BG_B, PAL3);

    XGM2_playPCM(loc_jc, sizeof(loc_jc), SOUND_PCM_CH2);
    XGM2_play(mus_the_beginning);
}
