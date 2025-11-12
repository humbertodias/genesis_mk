#ifndef INTRO_DEMO_ROOM_H
#define INTRO_DEMO_ROOM_H

#include <genesis.h>
#include "estruturas.h"

typedef struct
{
    const SpriteDefinition *bio;
    const SpriteDefinition *name;
    const u8 *voice;
    const u16 voice_size;
    const TextLine *lines;
    const u16 num_lines;
} BioData;

void processIntro();

#endif