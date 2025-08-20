#ifndef CREATURES_H
#define CREATURES_H
#include <gb/gb.h>

typedef enum { SPEC_SPROUTLE=0, SPEC_EMBERPUP=1, SPEC_AQUABUG=2, SPEC_COUNT } Species;
typedef enum { MOVE_TACKLE=0, MOVE_BURN, MOVE_SPLASH, MOVE_VINE, MOVE_COUNT } MoveId;
typedef enum { TYPE_GRASS=0, TYPE_FIRE=1, TYPE_WATER=2 } ElemType;

typedef struct {
    const char* name;
    UINT8 power;   // base damage
    UINT8 acc;     // 0-100
    ElemType type;
} Move;

typedef struct {
    Species species;
    ElemType type;
    UINT8 level;
    UINT8 hp;
    UINT8 atk;
    UINT8 spd;
    UINT8 exp;
    MoveId moves[2]; // up to 2 moves
    UINT8 move_count;
} Mon;

const char* species_name(Species s);
void mon_generate_wild(Mon* m, UINT8 area);
void mon_heal(Mon* m);
void mon_gain_exp(Mon* m, UINT8 amount);
const Move* move_get(MoveId id);
const char* type_name(ElemType t);
INT8 type_effect(ElemType atk, ElemType def);

#endif
