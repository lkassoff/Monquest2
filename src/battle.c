#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "battle.h"
#include "party.h"
#include "sfx.h"

static Mon wild;
static Mon hero; // current leader

static UINT8 rng8(void){ return DIV_REG; }

static void draw_bar(UINT8 x, UINT8 y, UINT8 cur, UINT8 max){
    UINT8 len = 10;
    UINT8 filled = (cur*len)/ (max?max:1);
    gotoxy(x,y);
    printf("[");
    for(UINT8 i=0;i<len;i++) putchar(i<filled? '#' : '.');
    printf("]");
}

static void draw_battle(void){
    HIDE_BKG; HIDE_SPRITES;
    font_t f; font_init(); f = font_load(font_min); font_set(f);
    const Move* m1 = move_get(hero.moves[0]);
    const Move* m2 = (hero.move_count>1)? move_get(hero.moves[1]):0;
    printf("\n %s (%s) vs %s (%s)\n",
        species_name(hero.species), type_name(hero.type), species_name(wild.species), type_name(wild.type));
    draw_bar(2,3, hero.hp, hero.hp); // use current hp for both max & cur (demo)
    draw_bar(2,5, wild.hp, wild.hp);
    gotoxy(2,7); printf("A)%s  Sel)%s", m1->name, m2?m2->name:"None");
    gotoxy(2,8); printf("B)Run  Start)Orb");
}

static void update_hud(void){
    draw_bar(2,3, hero.hp, hero.hp);
    draw_bar(2,5, wild.hp, wild.hp);
}

static void apply_move(Mon* atk, Mon* def, const Move* mv){
    UINT8 roll = rng8()%100;
    if (roll < mv->acc){
        INT8 eff = type_effect(mv->type, def->type);
        UINT8 dmg = (mv->power + atk->atk/2) + (rng8() & 1);
        if (eff>0) dmg += 2;
        if (eff<0 && dmg>2) dmg -= 2;
        def->hp = (def->hp>dmg)? (def->hp-dmg):0;
        sfx_hit();
        gotoxy(2,9);
        if (eff>0) printf("It's super effective!   ");
        else if (eff<0) printf("Not very effective...   ");
        else printf("                        ");
    } else {
        gotoxy(2,9); printf("Miss!                   ");
    }
}

void battle_start(UINT8 area){
    if (party.count) hero = party.mons[party.leader];
    else { hero.level=5; hero.type=TYPE_GRASS; hero.hp=30; hero.atk=7; hero.spd=5; hero.exp=0; hero.species=SPEC_SPROUTLE; hero.moves[0]=MOVE_TACKLE; hero.move_count=1; }

    mon_generate_wild(&wild, area);
    draw_battle();

    while(1){
        wait_vbl_done();
        UINT8 j = joypad();
        if (j & J_A){
            apply_move(&hero, &wild, move_get(hero.moves[0]));
            update_hud();
            if (!wild.hp){
                gotoxy(2,10); printf("You won! +6 EXP        ");
                if (party.count){ mon_gain_exp(&party.mons[party.leader], 6); }
                while(!(joypad()&J_A)) wait_vbl_done();
                break;
            }
            apply_move(&wild, &hero, move_get(MOVE_TACKLE));
            update_hud();
            if (!hero.hp){
                gotoxy(2,10); printf("You fainted!           ");
                while(!(joypad()&J_A)) wait_vbl_done();
                break;
            }
        } else if ((j & J_SELECT) && hero.move_count>1){
            apply_move(&hero, &wild, move_get(hero.moves[1]));
            update_hud();
        } else if (j & J_START){
            UINT8 chance = 40 + (hero.level*2) + (255 - (wild.hp<<2))/8;
            UINT8 roll = rng8();
            if (roll < chance && !party_is_full()){
                party_add(&wild);
                sfx_capture();
                gotoxy(2,10); printf("Captured %s!          ", species_name(wild.species));
                while(!(joypad()&J_A)) wait_vbl_done();
                break;
            } else if (party_is_full()){
                gotoxy(2,10); printf("Party full!           ");
                while(!(joypad()&J_A)) wait_vbl_done();
            } else {
                gotoxy(2,10); printf("It broke free!        ");
                while(!(joypad()&J_A)) wait_vbl_done();
                update_hud();
            }
        } else if (j & J_B){
            if (hero.spd + (rng8() & 7) >= wild.spd){
                gotoxy(2,10); printf("Got away!             ");
                while(!(joypad()&J_A)) wait_vbl_done();
                break;
            } else {
                gotoxy(2,10); printf("Couldn't escape!      ");
                UINT8 dmg = wild.atk;
                hero.hp = (hero.hp>dmg)? (hero.hp-dmg):0;
                update_hud();
                if (!hero.hp){
                    gotoxy(2,10); printf("You fainted!         ");
                    while(!(joypad()&J_A)) wait_vbl_done();
                    break;
                }
            }
        }
    }
    SHOW_BKG; SHOW_SPRITES;
}
