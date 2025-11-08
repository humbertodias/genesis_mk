#include <genesis.h>

#include "game_vars.h"
#include "fighters.h"
#include "estruturas.h"
#include "cenarios.h"
#include "input_system.h"
#include "gfx.h"
#include "sprites.h"
#include "anima_system.h"

#define DEBUG FALSE

// -- DECLARACAO DE VARIAVEIS -- //
u8 gRoom; // Sala atual
u16 gDistancia; // Distancia entre os Players
bool gPodeMover = TRUE;
// u16 palette[64];
s16 gMeioDaTela = 0;     // MEio da Câmera em X
s16 camPosX = 0;         // Posicao da Camera
s16 camPosXanterior = 0; // Posicao da Camera no frame Anterior
// Map *level_map;
// Sprite *spr_p1; // Sprite de personagem no icone ativo
// Sprite *spr_p2; // Sprite de personagem no icone ativo
s16 scrollOffset = 0;
s16 scrollValues[48];

// typedef void (*PlayerStateFunc)(int, u16);

// const PlayerStateFunc PLAYER_STATE_FUNCS[7] = {
//     playerState_Johnny,
//     playerState_Kano,
//     playerState_Rayden,
//     playerState_LiuKang,
//     playerState_SubZero,
//     playerState_Scorpion,
//     playerState_Sonya};

void resetGraphicElements();
void CLEAR_VDP();
void playerState2(int Player, u16 State);

int main(bool hardReset)
{
  SPR_init();
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  gRoom = SELECAO_PERSONAGENS;
  gFrames = 0;
  gInd_tileset = 0;

  if (!hardReset)
  {
    SYS_hardReset();
  }

  resetGraphicElements();

  while (TRUE)
  {
    gFrames++;

    newInputSystem();

    if (TELA_DEMO_INTRO == gRoom)
    {
      // enquanto não pressionar start...
      processIntro();
    }

    if (TELA_START == gRoom)
    {
      processPressStart();
    }

    if (SELECAO_PERSONAGENS == gRoom)
    {
      processSelecaoPersonagens();
      anima();
    }

    if (PALACE_GATES == gRoom)
    {
      if (gFrames == 1)
        CLEAR_VDP();

      initPalaceGatesRoom();

      player[0].sprite = SPR_addSprite(&spr_subzero, 24, 96, TILE_ATTR(PAL2, 0, FALSE, FALSE));
      PAL_setPalette(PAL2, spr_subzero.palette->data, DMA);

      player[1].sprite = SPR_addSprite(&spr_subzero, 168, 96, TILE_ATTR(PAL3, 0, FALSE, TRUE));
      PAL_setPalette(PAL3, spr_subzero.palette->data, DMA);

      player[0].id = SUBZERO;
      player[1].id = SUBZERO;
      player[0].state = PARADO;
      player[1].state = PARADO;
      player[0].paleta = PAL2;
      player[1].paleta = PAL3;

      scrollOffset += 1;
      VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_COLUMN);

      for (int i = 0; i < 48; i++)
      {
        scrollValues[i] = scrollOffset;
      }

      VDP_setHorizontalScrollLine(BG_B, 0, scrollValues, 48, CPU);
    }

    // -- DEBUG -- //
    if (DEBUG)
    {
      char str[64];
      sprintf(str, "tiles nos BGs: %d", gInd_tileset);
      VDP_drawText(str, 1, 1);
      // sprintf(str, "contagem de frames: %ld", gFrames);
      // VDP_drawText(str, 1, 2);
    }

    // -- FINALIZAÇÕES -- //

    SPR_update();          // Atualização dos sprites na tela
    SYS_doVBlankProcess(); // Sincroniza com o VBlank
  }

  return 0;
}

void CLEAR_VDP()
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

void playerState2(int numPlayer, u16 State)
{/*
  if (player[numPlayer].sprite)
  {
    SPR_releaseSprite(player[numPlayer].sprite);
    player[numPlayer].sprite = NULL;
  }

  player[numPlayer].animFrame = 1;
  player[numPlayer].frameTimeAtual = 1;
  player[numPlayer].dataAnim[1] = 1;
  player[numPlayer].animFrameTotal = 1;
  player[numPlayer].state = State;

  if (PLAYER_STATE_FUNCS[player[numPlayer].id])
  {
    PLAYER_STATE_FUNCS[player[numPlayer].id](numPlayer, State);
  }

  // Flipa o sprite
  SPR_setHFlip(player[numPlayer].sprite, (player[numPlayer].direcao == 1) ? FALSE : TRUE);

  SPR_setAnimAndFrame(player[numPlayer].sprite, 0, player[numPlayer].animFrame - 1);
  player[numPlayer].frameTimeTotal = player[numPlayer].dataAnim[1];

  if (player[numPlayer].sprite)
  {
    // FUNCAO_DEPTH
  }
  // FUNCAO_SPR_POSITION
  // ajusta posicao do sprite
  if (player[numPlayer].direcao == 1)
  {
    SPR_setPosition(player[numPlayer].sprite,
                    player[numPlayer].x - (player[numPlayer].w - player[numPlayer].axisX) - camPosX,
                    player[numPlayer].y - player[numPlayer].axisY);
  }
  if (player[numPlayer].direcao == -1)
  {
    SPR_setPosition(player[numPlayer].sprite,
                    player[numPlayer].x - player[numPlayer].axisX - camPosX,
                    player[numPlayer].y - player[numPlayer].axisY);
  }
                    */
}

void resetGraphicElements()
{
  for (int ind = 0; ind < 25; ind++)
  {
    if (GE[ind].sprite)
    {
      SPR_releaseSprite(GE[ind].sprite);
      GE[ind].sprite = NULL;
    }
  }
}