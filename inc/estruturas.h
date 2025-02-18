#ifndef _ESTRUTURAS_H_
#define _ESTRUTURAS_H_

#include "types.h"

enum GAME_ROOM
{
  TELA_DEMO_INTRO,
  TELA_TITULO,
  TELA_START,
  SELECAO_PERSONAGENS,
  PALACE_GATES
};

typedef struct
{
  u16 x;
  u16 y;
} PlayerOption;

typedef struct
{
  const char *text;
  u16 x;
  u16 y;
} TextLine;

#endif