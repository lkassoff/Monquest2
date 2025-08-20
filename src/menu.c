#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "menu.h"
#include "party.h"
#include "password.h"
#include "map.h"
#include "npc.h"

extern UINT8 px, py;

static void wait_release(void){ while(joypad()) wait_vbl_done(); }

void menu_show_party(void) {
    HIDE_SPRITES; HIDE_BKG;
    party_print();
    printf("\n  A=Choose Leader\n  B=Exit");
    while(1){
        UINT8 j=joypad();
        if (j & J_A){ party_choose_leader(); party_print(); printf("\n  A=Choose Leader\n  B=Exit"); }
        else if (j & J_B){ break; }
        wait_vbl_done();
    }
    SHOW_BKG; SHOW_SPRITES; wait_release();
}

void menu_show_save(void) {
    char code[17]; encode_password(code, &party, current_map, px, py);
    HIDE_SPRITES; HIDE_BKG;
    font_t f; font_init(); f = font_load(font_min); font_set(f);
    printf("\n Save Password:\n\n  %s\n\n Write it down!\n\nB=Back", code);
    while(!(joypad() & J_B)) wait_vbl_done();
    SHOW_BKG; SHOW_SPRITES; wait_release();
}

void menu_open(void) {
    HIDE_SPRITES; HIDE_BKG;
    font_t f; font_init(); f = font_load(font_min); font_set(f);
    const char* items[] = {"Party", "Save", "Options", "Exit"};
    UINT8 sel = 0;
    while(1){
        printf("\n   MonQuest Menu\n\n");
        for(UINT8 i=0;i<4;i++){
            printf(" %c %s\n", (i==sel)?'>':' ', items[i]);
        }
        while(1){
            UINT8 j = joypad();
            if (j & J_DOWN){ if (sel<3) sel++; break; }
            if (j & J_UP){ if (sel>0) sel--; break; }
            if (j & J_A){ break; }
            if (j & J_B){ sel = 3; break; }
            wait_vbl_done();
        }
        if (sel==0){ menu_show_party(); }
        else if (sel==1){ menu_show_save(); }
        else if (sel==2){ printf("\n Encounters: %s\n\nB=Back", encounter_enabled?"ON":"OFF"); while(!(joypad()&J_B)){ if (joypad()&J_A){ encounter_enabled=!encounter_enabled; printf("\r Encounters: %s", encounter_enabled?"ON":"OFF"); } wait_vbl_done(); } }
        else if (sel==3){ break; }
    }
    SHOW_BKG; SHOW_SPRITES;
}
