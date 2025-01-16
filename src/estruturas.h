#ifndef _ESTRUTURAS_H_
#define _ESTRUTURAS_H_

#include "types.h"

enum GAME_ROOM
{
    TELA_TITULO,
    SELECAO_PERSONAGENS
};

typedef struct
{
  u16 x;
  u16 y;
} PlayerOption;

#endif