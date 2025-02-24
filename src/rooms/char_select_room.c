#include "char_select_room.h"
#include "stages.h"
#include "sprites.h"

#define TILEMAP_WIDTH 40
#define TILEMAP_HEIGHT 28
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define LINE_HEIGHT 2 // Altura da linha em pixels

void drawBackground();
void revealBackground();
void initScrollLine();
void updateSelector(Player *player);
void playCursor();

static s16 scrollLine[SCREEN_HEIGHT]; // usado para fazer o efeito de persiana

Sprite *spr_p1_fighter; // Sprite de personagem 1 no icone ativo
Sprite *spr_p2_fighter; // Sprite de personagem 2 no icone ativo

// CAGE      20, 44
// KANO      76, 44
// RAYDEN    76, 108
// LIU KANG 132, 108
// SUBZERO  188, 44
// SCORPION 188, 108
// SONYA    244, 44
PlayerOption pOptions[7] = {
    {20, 44},
    {76, 44},
    {76, 108},
    {132, 108},
    {188, 44},
    {188, 108},
    {244, 44},
};

void processSelecaoPersonagens()
{
  VDP_setPlaneSize(128, 64, TRUE);
  VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);

  if (gFrames == 1)
  {
    initScrollLine();
    drawBackground();
  }

  if (gFrames == 40)
  {
    XGM2_playPCMEx(snd_gongo, sizeof(snd_gongo), SOUND_PCM_CH3, 0, FALSE, 0);

    revealBackground();

    XGM2_play(mus_select_player);

    player[0].sprite = SPR_addSprite(&player_seletor, pOptions[KANO].x, pOptions[KANO].y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    // PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    SPR_setDepth(player[0].sprite, 1);
    // Player 1 olhando para Direita começando no KANO
    player[0].direcao = 1;
    player[0].id = KANO;

    player[1].sprite = SPR_addSprite(&player_seletor, pOptions[SUBZERO].x, pOptions[SUBZERO].y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    // PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    SPR_setAnim(player[1].sprite, 1);
    SPR_setDepth(player[1].sprite, 2);
    // Player 2 olhando para Esquerda começando no SUBZERO
    player[1].direcao = -1;
    player[1].id = SUBZERO;
  }

  for (int ind = 0; ind < 2; ind++)
  {
    updateSelector(&player[ind]);
  }

  // TODO: ajustar isso depois e remover
  if (player[0].id != SUBZERO && spr_p1_fighter)
  {
    SPR_releaseSprite(spr_p1_fighter);
    spr_p1_fighter = NULL;
  }
  else if (player[0].id == SUBZERO && !spr_p1_fighter)
  {
    spr_p1_fighter = SPR_addSprite(&spr_subzero, 24, 104, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL2, spr_subzero.palette->data, DMA);
    SPR_setAnim(spr_p1_fighter, 0);
    SPR_setDepth(spr_p1_fighter, SPR_MIN_DEPTH);
  }
  if (player[1].id == SUBZERO && !spr_p2_fighter)
  {
    // spr_p2_fighter = SPR_addSprite(&spr_subzero, 153, 61, TILE_ATTR(PAL3, FALSE, FALSE, TRUE));
    spr_p2_fighter = SPR_addSprite(&spr_subzero, 168, 104, TILE_ATTR(PAL3, FALSE, FALSE, TRUE));
    PAL_setPalette(PAL3, spr_subzero.palette->data, DMA);
    SPR_setAnim(spr_p2_fighter, 0);
    SPR_setDepth(spr_p2_fighter, SPR_MIN_DEPTH);
  }
  else if (player[1].id != SUBZERO && spr_p2_fighter)
  {
    SPR_releaseSprite(spr_p2_fighter);
    spr_p2_fighter = NULL;
  }
  // Mostra os IDs dos personagens
  // char stri[64];
  // sprintf(stri, "p1: %d", player[0].id);
  // VDP_drawText(stri, 1, 1);
  // sprintf(stri, "p2: %d", player[1].id);
  // VDP_drawText(stri, 1, 2);
}

/**
 * @brief Carrega as informações dos backgrounds
 */
void drawBackground()
{
  // para evitar problemas ao carregar a tela carrego o tile preto no index 0
  VDP_loadTileSet(&black_tile, gInd_tileset, DMA);
  gInd_tileset += black_tile.numTile;

  // BACKGROUND A
  VDP_loadTileSet(stage_char_select_a.tileset, gInd_tileset, DMA);
  VDP_setTileMapEx(BG_A, stage_char_select_a.tilemap,
                   TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset),
                   0, 0,
                   0, 0,
                   TILEMAP_WIDTH, TILEMAP_HEIGHT, DMA);
  PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
  gInd_tileset += stage_char_select_a.tileset->numTile;

  // BACKGROUND B
  VDP_loadTileSet(stage_char_select_b.tileset, gInd_tileset, DMA);
  VDP_setTileMapEx(BG_B, stage_char_select_b.tilemap,
                   TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset),
                   0, 0,
                   0, 0,
                   TILEMAP_WIDTH, TILEMAP_HEIGHT, DMA);
  PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
  gInd_tileset += stage_char_select_b.tileset->numTile;
}

/**
 * @brief move a tela para fora da parte visível a esquerda
 * para iniciar o efeito de Persiana.
 */
void initScrollLine()
{
  for (int x = 0; x < SCREEN_HEIGHT; x++)
  {
    scrollLine[x] = -SCREEN_WIDTH; // inicia as linhas com 320px
  }
  VDP_setHorizontalScrollLine(BG_A, 0, scrollLine, SCREEN_HEIGHT, DMA);
  VDP_setHorizontalScrollLine(BG_B, 0, scrollLine, SCREEN_HEIGHT, DMA);
}

/**
 * @brief Revela a tela fazendo o efeito de persiana.
 */
void revealBackground()
{
  struct VenetianBlindsEffect
  {
    u16 startLine;   // linha onde começa o efeito
    u16 nextLine;    // linha onde deve começar o próximo efeito
    u16 endLine;     // linha onde termina o efeito
    u16 currentLine; // linha atual da iteração
  };

  struct VenetianBlindsEffect persiana[7] = {
      {0, 13, 31, 0},
      {32, 43, 63, 0},
      {64, 73, 95, 0},
      {96, 105, 127, 0},
      {128, 137, 159, 0},
      {160, 167, 191, 0},
      {192, 0, 223, 0},
  };

  for (int y = 0; y < SCREEN_HEIGHT / 2; y += 1)
  {
    // 0
    if (y >= persiana[0].startLine && y <= persiana[0].endLine)
    {
      persiana[0].currentLine = y;
      scrollLine[persiana[0].currentLine] = 0;
      scrollLine[persiana[0].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, y, &scrollLine[y], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, y, &scrollLine[y], LINE_HEIGHT, DMA);
    }
    // 1
    if (y >= persiana[0].nextLine && persiana[1].currentLine <= persiana[1].endLine)
    {
      if (persiana[0].nextLine == y)
        persiana[1].currentLine = persiana[1].startLine;
      else
        persiana[1].currentLine += 1;
      scrollLine[persiana[1].currentLine] = 0;
      scrollLine[persiana[1].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, persiana[1].currentLine, &scrollLine[persiana[1].currentLine], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, persiana[1].currentLine, &scrollLine[persiana[1].currentLine], LINE_HEIGHT, DMA);
    }
    // 2
    if (persiana[1].currentLine >= persiana[1].nextLine && persiana[2].currentLine <= persiana[2].endLine)
    {
      if (persiana[1].nextLine == persiana[1].currentLine)
        persiana[2].currentLine = persiana[2].startLine;
      else
        persiana[2].currentLine += 1;
      scrollLine[persiana[2].currentLine] = 0;
      scrollLine[persiana[2].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, persiana[2].currentLine, &scrollLine[persiana[2].currentLine], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, persiana[2].currentLine, &scrollLine[persiana[2].currentLine], LINE_HEIGHT, DMA);
    }
    // 3
    if (persiana[2].currentLine >= persiana[2].nextLine && persiana[3].currentLine <= persiana[3].endLine)
    {
      if (persiana[2].nextLine == persiana[2].currentLine)
        persiana[3].currentLine = persiana[3].startLine;
      else
        persiana[3].currentLine += 1;
      scrollLine[persiana[3].currentLine] = 0;
      scrollLine[persiana[3].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, persiana[3].currentLine, &scrollLine[persiana[3].currentLine], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, persiana[3].currentLine, &scrollLine[persiana[3].currentLine], LINE_HEIGHT, DMA);
    }
    // 4
    if (persiana[3].currentLine >= persiana[3].nextLine && persiana[4].currentLine <= persiana[4].endLine)
    {
      if (persiana[3].nextLine == persiana[3].currentLine)
        persiana[4].currentLine = persiana[4].startLine;
      else
        persiana[4].currentLine += 1;
      scrollLine[persiana[4].currentLine] = 0;
      scrollLine[persiana[4].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, persiana[4].currentLine, &scrollLine[persiana[4].currentLine], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, persiana[4].currentLine, &scrollLine[persiana[4].currentLine], LINE_HEIGHT, DMA);
    }
    // 5
    if (persiana[4].currentLine >= persiana[4].nextLine && persiana[5].currentLine <= persiana[5].endLine)
    {
      if (persiana[4].nextLine == persiana[4].currentLine)
        persiana[5].currentLine = persiana[5].startLine;
      else
        persiana[5].currentLine += 1;
      scrollLine[persiana[5].currentLine] = 0;
      scrollLine[persiana[5].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, persiana[5].currentLine, &scrollLine[persiana[5].currentLine], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, persiana[5].currentLine, &scrollLine[persiana[5].currentLine], LINE_HEIGHT, DMA);
    }
    // 6
    if (persiana[5].currentLine >= persiana[5].nextLine && persiana[6].currentLine <= persiana[6].endLine)
    {
      if (persiana[5].nextLine == persiana[5].currentLine)
        persiana[6].currentLine = persiana[6].startLine;
      else
        persiana[6].currentLine += 1;
      scrollLine[persiana[6].currentLine] = 0;
      scrollLine[persiana[6].currentLine + 1] = 0;
      VDP_setHorizontalScrollLine(BG_A, persiana[6].currentLine, &scrollLine[persiana[6].currentLine], LINE_HEIGHT, DMA);
      VDP_setHorizontalScrollLine(BG_B, persiana[6].currentLine, &scrollLine[persiana[6].currentLine], LINE_HEIGHT, DMA);
    }
    SYS_doVBlankProcess();
  }
}

/**
 * @brief Atualiza a posição do cursor de seleção de acordo com o ID
 *
 * @param player
 */
void updateSelector(Player *player)
{
  switch (player->id)
  {
  case KANO:

    if (player->key_JOY_LEFT_status == 1) // se apertar para esquerda, seleciona CAGE
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[CAGE].x, pOptions[CAGE].y);
      player->id = CAGE;
    }
    else if (player->key_JOY_RIGHT_status == 1) // se apertar para direita, Subzero
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[SUBZERO].x, pOptions[SUBZERO].y);
      player->id = SUBZERO;
    }
    else if (player->key_JOY_DOWN_status == 1) // se apertar para baixo, Raiden
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[RAIDEN].x, pOptions[RAIDEN].y);
      player->id = RAIDEN;
    }
    break;

  case CAGE:

    if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[KANO].x, pOptions[KANO].y);
      player->id = KANO;
    }
    break;

  case SUBZERO:

    if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[KANO].x, pOptions[KANO].y);
      player->id = KANO;
    }
    else if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[SONYA].x, pOptions[SONYA].y);
      player->id = SONYA;
    }
    else if (player->key_JOY_DOWN_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[SCORPION].x, pOptions[SCORPION].y);
      player->id = SCORPION;
    }
    break;

  case SONYA:
    if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[SUBZERO].x, pOptions[SUBZERO].y);
      player->id = SUBZERO;
    }
    break;

  case SCORPION:
    if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[LIU_KANG].x, pOptions[LIU_KANG].y);
      player->id = LIU_KANG;
    }
    else if (player->key_JOY_UP_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[SUBZERO].x, pOptions[SUBZERO].y);
      player->id = SUBZERO;
    }
    break;

  case LIU_KANG:
    if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[SCORPION].x, pOptions[SCORPION].y);
      player->id = SCORPION;
    }
    else if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[RAIDEN].x, pOptions[RAIDEN].y);
      player->id = RAIDEN;
    }
    break;

  case RAIDEN:
    if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[LIU_KANG].x, pOptions[LIU_KANG].y);
      player->id = LIU_KANG;
    }
    else if (player->key_JOY_UP_status == 1)
    {
      playCursor();
      SPR_setPosition(player->sprite, pOptions[KANO].x, pOptions[KANO].y);
      player->id = KANO;
    }
    break;

  default:
    break;
  }
}

/**
 * @brief Executa o áudio do cursor */
void playCursor()
{
  XGM2_playPCMEx(snd_cursor, sizeof(snd_cursor), SOUND_PCM_CH3, 0, FALSE, 0);
}
