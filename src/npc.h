#ifndef NPC_H
#define NPC_H
#include <gb/gb.h>
extern UINT8 quest_has_orbkit;
extern UINT8 quest_captured_any;
void npc_draw(UINT8 x, UINT8 y);
void npc_check_interact(UINT8 px, UINT8 py);
#endif
