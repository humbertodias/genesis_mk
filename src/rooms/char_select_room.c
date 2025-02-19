#include "char_select_room.h"
#include "stages.h"
#include "sprites.h"

void updateSelector(Player *player);

void playCursor();

Sprite *spr_p1_fighter; // Sprite de personagem no icone ativo
Sprite *spr_p2_fighter; // Sprite de personagem no icone ativo

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

// void desenrolarBackground() {
//   for (u16 y = 0; y < 28; y+=4) {
//     VDP_setTileMapEx(BG_A, stage_char_select.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, y, 0, y, 40, 4, DMA_QUEUE);
//     PAL_setPalette(PAL1, stage_char_select.palette->data, DMA);
//     SYS_doVBlankProcess(); // Sincroniza com o VBlank para criar o efeito gradual
//   }
// }

void processSelecaoPersonagens()
{
  if (gFrames == 1)
  {
    XGM2_play(mus_select_player);
    VDP_setBackgroundColor(0);

    // desenrolarBackground();

    VDP_loadTileSet(stage_char_select_a.tileset, gInd_tileset, DMA);
    VDP_setTileMapEx(BG_A, stage_char_select_a.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    PAL_setPalette(PAL0, char_select_a_pal.data, DMA);
    gInd_tileset += stage_char_select_a.tileset->numTile;

    VDP_loadTileSet(stage_char_select_b.tileset, gInd_tileset, DMA);
    VDP_setTileMapEx(BG_B, stage_char_select_b.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
    PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    gInd_tileset += stage_char_select_b.tileset->numTile;

    player[0].sprite = SPR_addSprite(&player_seletor, pOptions[KANO].x, pOptions[KANO].y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
    SPR_setDepth(player[0].sprite, 1);
    // Player 1 olhando para Direita começando no KANO
    player[0].direcao = 1;
    player[0].id = KANO;

    player[1].sprite = SPR_addSprite(&player_seletor, pOptions[SUBZERO].x, pOptions[SUBZERO].y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    PAL_setPalette(PAL1, char_select_b_pal.data, DMA);
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

void playCursor()
{
  XGM2_playPCMEx(snd_cursor, sizeof(snd_cursor), SOUND_PCM_CH3, 0, FALSE, 0);
}
