#include "creatures.h"
#include <stdlib.h>

static const char* NAMES[SPEC_COUNT] = { "Sproutle", "Emberpup", "Aquabug" };
static const Move MOVES[MOVE_COUNT] = {
    {"Tackle", 6, 95, TYPE_GRASS},
    {"Burn",   8, 85, TYPE_FIRE},
    {"Splash", 5, 100, TYPE_WATER},
    {"Vine",   7, 90, TYPE_GRASS}
};

const char* species_name(Species s) {
    if ((UINT8)s >= SPEC_COUNT) return "Unknown";
    return NAMES[(UINT8)s];
}

const Move* move_get(MoveId id){
    if ((UINT8)id >= MOVE_COUNT) return &MOVES[0];
    return &MOVES[(UINT8)id];
}

const char* type_name(ElemType t){
    if (t==TYPE_GRASS) return "Grass";
    if (t==TYPE_FIRE) return "Fire";
    return "Water";
}

// returns -1 (not very), 0 (normal), +1 (super)
INT8 type_effect(ElemType atk, ElemType def){
    if (atk==TYPE_GRASS && def==TYPE_WATER) return +1;
    if (atk==TYPE_WATER && def==TYPE_FIRE) return +1;
    if (atk==TYPE_FIRE  && def==TYPE_GRASS) return +1;
    if (atk==TYPE_GRASS && def==TYPE_FIRE) return -1;
    if (atk==TYPE_WATER && def==TYPE_GRASS) return -1;
    if (atk==TYPE_FIRE  && def==TYPE_WATER) return -1;
    return 0;
}

static UINT8 rng8(void){ return DIV_REG; }

static void mon_assign_moves(Mon* m){
    m->move_count = 1;
    m->moves[0] = MOVE_TACKLE;
    if (m->species == SPEC_EMBERPUP) m->moves[m->move_count++] = MOVE_BURN;
    else if (m->species == SPEC_AQUABUG) m->moves[m->move_count++] = MOVE_SPLASH;
    else if (m->species == SPEC_SPROUTLE) m->moves[m->move_count++] = MOVE_VINE;
}

void mon_generate_wild(Mon* m, UINT8 area) {
    m->species = (Species)(rng8() % SPEC_COUNT);
    m->type = (ElemType)((UINT8)m->species % 3);
    m->level = 3 + (rng8() & 3) + (area ? 2 : 0);
    m->hp = 15 + m->level * 2 + (rng8() & 3);
    m->atk = 4 + m->level + (rng8() & 1);
    m->spd = 3 + m->level + (rng8() & 1);
    m->exp = 0;
    mon_assign_moves(m);
}

void mon_heal(Mon* m) { if (m->hp < 60) m->hp = 60; }

void mon_gain_exp(Mon* m, UINT8 amount){
    m->exp += amount;
    while (m->exp >= 10){
        m->exp -= 10; m->level++; m->hp += 3; m->atk += 1; m->spd += 1;
    }
}
