#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <gb/gb.h>

extern const unsigned char tiles[];
extern const unsigned char player_sprite[];
extern const unsigned char npc_sprite[];

void gfx_load_all(void);
#endif
