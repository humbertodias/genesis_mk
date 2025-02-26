#include "char_select_room.h"
#include "stages.h"
#include "sprites.h"

#define TILEMAP_WIDTH 40
#define TILEMAP_HEIGHT 28
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define LINE_HEIGHT 2 // Altura da linha em pixels

void drawBackground();
void initScrollLine();
void revealBackground();
void updateSelector(Player *player, int ind);
void playCursor();
void initPlayer();

static s16 scrollLine[SCREEN_HEIGHT]; // usado para fazer o efeito de persiana
// CAGE      20, 44
// KANO      76, 44
// RAYDEN    76, 108
// LIU KANG 132, 108
// SUBZERO  188, 44
// SCORPION 188, 108
// SONYA    244, 44
static const u8 OPTIONS_X[7] = {20, 76, 76, 132, 188, 188, 244};
static const u8 OPTIONS_Y[7] = {44, 44, 108, 108, 44, 108, 44};

void processSelecaoPersonagens()
{
  if (gFrames == 1)
  {
    VDP_setPlaneSize(128, 64, TRUE);
    VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
    SYS_disableInts();

    initScrollLine();
    drawBackground();
  }
  if (gFrames == 30)
    XGM2_playPCMEx(snd_gongo, sizeof(snd_gongo), SOUND_PCM_CH2, 0, FALSE, 0);

  if (gFrames == 40)
  {
    revealBackground();

    XGM2_play(mus_select_player);

    initPlayer();

    // indice 0 e 1 do GE serão usados para carregar o gráfico do seletor
    GE[0].sprite = SPR_addSprite(&player_seletor,
                                 OPTIONS_X[KANO], OPTIONS_Y[KANO],
                                 TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    SPR_setDepth(GE[0].sprite, 1);

    GE[1].sprite = SPR_addSprite(&player_seletor,
                                 OPTIONS_X[SUBZERO], OPTIONS_Y[SUBZERO],
                                 TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    SPR_setAnim(GE[1].sprite, 1);
    SPR_setDepth(GE[1].sprite, 2);

    SYS_enableInts();
  }

  for (int ind = 0; ind < 2; ind++)
  {
    updateSelector(&player[ind], ind);
  }

  // TODO: ajustar isso depois e remover
  if (player[0].id != SUBZERO && player[0].sprite)
  {
    SPR_releaseSprite(player[0].sprite);
    player[0].sprite = NULL;
  }
  else if (player[0].id == SUBZERO && !player[0].sprite)
  {
    player[0].sprite = SPR_addSprite(&spr_subzero, 24, 104, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL2, spr_subzero.palette->data, DMA);
    SPR_setAnim(player[0].sprite, 0);
    SPR_setDepth(player[0].sprite, SPR_MIN_DEPTH);
  }
  if (player[1].id == SUBZERO && !player[1].sprite)
  {
    player[1].sprite = SPR_addSprite(&spr_subzero, 168, 104, TILE_ATTR(PAL3, FALSE, FALSE, TRUE));
    PAL_setPalette(PAL3, spr_subzero.palette->data, DMA);
    SPR_setAnim(player[1].sprite, 0);
    SPR_setDepth(player[1].sprite, SPR_MIN_DEPTH);
  }
  else if (player[1].id != SUBZERO && player[1].sprite)
  {
    SPR_releaseSprite(player[1].sprite);
    player[1].sprite = NULL;
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
  gInd_tileset = 1;
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
 * @param ind indice usado pelo GraphicElements
 */
void updateSelector(Player *player, int ind)
{
  switch (player->id)
  {
  case KANO:

    if (player->key_JOY_LEFT_status == 1) // se apertar para esquerda, seleciona CAGE
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[CAGE], OPTIONS_Y[CAGE]);
      player->id = CAGE;
    }
    else if (player->key_JOY_RIGHT_status == 1) // se apertar para direita, Subzero
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[SUBZERO], OPTIONS_Y[SUBZERO]);
      player->id = SUBZERO;
    }
    else if (player->key_JOY_DOWN_status == 1) // se apertar para baixo, Raiden
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[RAIDEN], OPTIONS_Y[RAIDEN]);
      player->id = RAIDEN;
    }
    break;

  case CAGE:

    if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[KANO], OPTIONS_Y[KANO]);
      player->id = KANO;
    }
    break;

  case SUBZERO:

    if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[KANO], OPTIONS_Y[KANO]);
      player->id = KANO;
    }
    else if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[SONYA], OPTIONS_Y[SONYA]);
      player->id = SONYA;
    }
    else if (player->key_JOY_DOWN_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[SCORPION], OPTIONS_Y[SCORPION]);
      player->id = SCORPION;
    }
    break;

  case SONYA:
    if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[SUBZERO], OPTIONS_Y[SUBZERO]);
      player->id = SUBZERO;
    }
    break;

  case SCORPION:
    if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[LIU_KANG], OPTIONS_Y[LIU_KANG]);
      player->id = LIU_KANG;
    }
    else if (player->key_JOY_UP_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[SUBZERO], OPTIONS_Y[SUBZERO]);
      player->id = SUBZERO;
    }
    break;

  case LIU_KANG:
    if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[SCORPION], OPTIONS_Y[SCORPION]);
      player->id = SCORPION;
    }
    else if (player->key_JOY_LEFT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[RAIDEN], OPTIONS_Y[RAIDEN]);
      player->id = RAIDEN;
    }
    break;

  case RAIDEN:
    if (player->key_JOY_RIGHT_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[LIU_KANG], OPTIONS_Y[LIU_KANG]);
      player->id = LIU_KANG;
    }
    else if (player->key_JOY_UP_status == 1)
    {
      playCursor();
      SPR_setPosition(GE[ind].sprite, OPTIONS_X[KANO], OPTIONS_Y[KANO]);
      player->id = KANO;
    }
    break;

  default:
    break;
  }
}

void initPlayer()
{
  // for (int ind = 0; ind < 2; ind++)
  // {
  //   player[ind].key_JOY_countdown[2] = 0;
  //   player[ind].key_JOY_countdown[4] = 0;
  //   player[ind].key_JOY_countdown[6] = 0;
  //   player[ind].key_JOY_countdown[8] = 0;
  // }

  player[0].id = KANO;
  player[0].state = PARADO;
  player[0].paleta = PAL2;
  player[0].direcao = 1;

  player[0].key_JOY_countdown[2] = 0;
  player[0].key_JOY_countdown[4] = 0;
  player[0].key_JOY_countdown[6] = 0;
  player[0].key_JOY_countdown[8] = 0;

  player[1].id = SUBZERO;
  player[1].state = PARADO;
  player[1].paleta = PAL3;
  player[1].direcao = -1;

  player[1].key_JOY_countdown[2] = 0;
  player[1].key_JOY_countdown[4] = 0;
  player[1].key_JOY_countdown[6] = 0;
  player[1].key_JOY_countdown[8] = 0;
}
/**
 * @brief Executa o áudio do cursor */
void playCursor()
{
  XGM2_playPCMEx(snd_cursor, sizeof(snd_cursor), SOUND_PCM_CH2, 0, FALSE, 0);
}
