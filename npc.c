#include "npc.h"
#include <gb/font.h>
#include <stdio.h>
#include "party.h"
#include "creatures.h"

UINT8 quest_has_orbkit = 0;
UINT8 quest_captured_any = 0;

static UINT8 npc_x=16, npc_y=1; // Meadow tile coords

void npc_draw(UINT8 x, UINT8 y){ (void)x; (void)y; }

void npc_check_interact(UINT8 px, UINT8 py){
    if (px==npc_x && py==npc_y){
        font_t f; font_init(); f = font_load(font_min); font_set(f);
        printf("\n Wise Wanderer:\n");
        if (!quest_has_orbkit){
            printf(" Take this Orb Kit!\n Press Start in battle\n to capture.\n\n(A)");
            quest_has_orbkit = 1;
            while(!(joypad()&J_A)) wait_vbl_done();
        } else if (quest_has_orbkit && !quest_captured_any){
            printf(" Capture a creature in\n the Cave, then visit me.\n\n(A)");
            while(!(joypad()&J_A)) wait_vbl_done();
        } else {
            printf(" Well done! I healed\n your party.\n\n(A)");
            for (UINT8 i=0;i<party.count;i++) mon_heal(&party.mons[i]);
            while(!(joypad()&J_A)) wait_vbl_done();
        }
    }
}
