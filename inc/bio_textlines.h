#ifndef _BIO_TEXTLINES_H_
#define _BIO_TEXTLINES_H_

#include <genesis.h>
#include "estruturas.h"

typedef struct
{
    const SpriteDefinition *bio;  // sprites da apresentação da biografia do personagem
    const SpriteDefinition *name; // sprite do nome do personagem
    const u8 *voice;              // locutor dizendo o nome
    const u16 voice_size;         // tamanho do audio do locutor
    const TextLine *lines_EN;     // textos da biografia em inglês
    const u16 num_lines_en;       // quantidade de linhas do texto em EN
    const TextLine *lines_BR;     // textos da biografia em Português
    const u16 num_lines_br;       // quantidade de linhas do texto em BR
} BioData;

// extern const TextLine johnnyCageLines_EN[];
// extern const TextLine johnnyCageLines_BR[];
// extern const TextLine kanoLines_EN[];
// extern const TextLine kanoLines_BR[];
// extern const TextLine liuKangLines_EN[];
// extern const TextLine liuKangLines_BR[];
// extern const TextLine scorpionLines_EN[];
// extern const TextLine scorpionLines_BR[];
// extern const TextLine subzeroLines_EN[];
// extern const TextLine subzeroLines_BR[];
// extern const TextLine raidenLines_EN[];
// extern const TextLine raidenLines_BR[];
// extern const TextLine sonyaLines_EN[];
// extern const TextLine sonyaLines_BR[];
extern const TextLine goroLines_EN[];
extern const TextLine goroLines_BR[];

const BioData* getFighterBio(u16 fighterIndex);

#endif