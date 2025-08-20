#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/font.h>
#include <stdio.h>
#include "graphics.h"
#include "map.h"
#include "menu.h"
#include "battle.h"
#include "party.h"
#include "password.h"
#include "npc.h"

UINT8 px=2, py=2;

static void seed_rng(void){ UINT8 i,s=0; for(i=0;i<32;i++){ wait_vbl_done(); s ^= DIV_REG; } DIV_REG=s; }
static void set_palettes(void){
    UWORD pal_bkg[] = { RGB_WHITE, RGB(0,31,0), RGB(0,21,0), RGB_BLACK };
    UWORD pal_spr[] = { RGB_WHITE, RGB(31,0,0), RGB(0,0,31), RGB_BLACK };
    set_bkg_palette(0,1,pal_bkg);
    set_sprite_palette(0,1,pal_spr);
}
static void draw_player(UINT8 x, UINT8 y){
    set_sprite_tile(0,0); set_sprite_tile(1,1);
    move_sprite(0,x,y); move_sprite(1,x, y+8);
    SHOW_SPRITES;
}

static void title(void){
    HIDE_BKG; HIDE_SPRITES;
    font_t f; font_init(); f = font_load(font_min); font_set(f);
    printf("\n     MonQuest v4\n\n  A) New Game\n  Start) Load\n\n  Press A or Start");
    while(1){
        UINT8 j = joypad();
        if (j & J_A){ party_init(); px=2; py=2; map_load(0); SHOW_BKG; SHOW_SPRITES; return; }
        if (j & J_START){
            char code[17] = "AAAAAAAAAAAAAAAA";
            UINT8 cur=0;
            while(1){
                printf("\n Load Password:\n  %s\n  Use D-Pad, A/B, Start", code);
                UINT8 jj = joypad();
                if (jj & J_LEFT){ if(cur>0) cur--; }
                else if (jj & J_RIGHT){ if(cur<15) cur++; }
                else if (jj & J_UP){ code[cur] = (code[cur]<'Z')? (code[cur]+1):'A'; }
                else if (jj & J_DOWN){ code[cur] = (code[cur]>'A')? (code[cur]-1):'Z'; }
                else if (jj & J_B){ code[cur] = 'A'; }
                else if (jj & J_START){
                    UINT8 m,x,y;
                    if (decode_password(code, &party, &m, &x, &y)){
                        map_load(m); px=x; py=y; SHOW_BKG; SHOW_SPRITES; return;
                    } else {
                        printf("\n  Invalid code!\n  (A)"); while(!(joypad()&J_A)) wait_vbl_done();
                    }
                }
                wait_vbl_done();
            }
        }
        wait_vbl_done();
    }
}

static void try_warp(void){
    UINT8 t = map_tile(px, py);
    if (current_map==0 && px==1 && py==1){ map_load(2); px=18; py=1; } // enter hut at corner
    else if (current_map==2 && t==3){ map_load(0); px=2; py=1; }       // hut exit
    else if (t==3){
        if (current_map==0){ map_load(1); px=1; py=1; }
        else if (current_map==1){ map_load(0); px=18; py=1; }
    }
}

void main(void){
    DISPLAY_ON;
    seed_rng();
    gfx_load_all();
    set_palettes();
    set_sprite_data(0, 2, player_sprite);
    set_sprite_data(2, 2, npc_sprite);
    set_sprite_tile(2,0); set_sprite_tile(3,1);

    title();

    while(1){
        UINT8 j = joypad();
        if (j & J_LEFT){ if (!map_is_solid(px-1,py)) px--; }
        else if (j & J_RIGHT){ if (!map_is_solid(px+1,py)) px++; }
        else if (j & J_UP){ if (!map_is_solid(px,py-1)) py--; }
        else if (j & J_DOWN){ if (!map_is_solid(px,py+1)) py++; }
        else if (j & J_START){ menu_open(); }
        else if (j & J_A){
            if (current_map==0 && map_tile(px,py)==4){ npc_check_interact(px,py); }
            if (encounter_enabled && map_tile(px,py)==1 && ((DIV_REG & 15)==0)){
                battle_start(current_map);
                if (party.count) quest_captured_any = 1;
                map_draw();
            }
        }

        try_warp();
        move_bkg(8*px, 8*py);
        draw_player(8*px+8, 8*py+16);
        wait_vbl_done();
    }
}
