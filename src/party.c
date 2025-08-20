#include "party.h"
#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>

Party party;

void party_init(void) { party.count = 0; party.leader = 0; }
UINT8 party_is_full(void) { return party.count >= PARTY_MAX; }

UINT8 party_add(const Mon* m) {
    if (party_is_full()) return 0;
    party.mons[party.count] = *m;
    party.count++;
    return 1;
}

void party_print(void) {
    font_t f; font_init(); f = font_load(font_min); font_set(f);
    printf("\n Party (leader marked '*'):\n");
    if (party.count == 0) printf("  (empty)\n");
    else for (UINT8 i=0;i<party.count;i++)
        printf("  %c%u) %s Lv%u HP%u EXP%u (%s)\n",
            (i==party.leader?'*':' '), i+1, species_name(party.mons[i].species),
            party.mons[i].level, party.mons[i].hp, party.mons[i].exp, type_name(party.mons[i].type));
}

void party_choose_leader(void){
    if (party.count==0) return;
    UINT8 sel = party.leader;
    font_t f; font_init(); f = font_load(font_min); font_set(f);
    while(1){
        printf("\n Choose Leader:\n");
        for(UINT8 i=0;i<party.count;i++){
            printf(" %c %u) %s\n", (i==sel?'>':' '), i+1, species_name(party.mons[i].species));
        }
        printf("\n A=Select  B=Back");
        while(1){
            UINT8 j=joypad();
            if (j & J_DOWN){ if (sel+1<party.count) sel++; break; }
            if (j & J_UP){ if (sel>0) sel--; break; }
            if (j & J_A){ party.leader = sel; return; }
            if (j & J_B){ return; }
            wait_vbl_done();
        }
    }
}
