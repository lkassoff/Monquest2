#include "map.h"
#include "mapdata.h"
#include <gb/gb.h>

UINT8 encounter_enabled = 1;
UINT8 current_map = 0;

Map MAP_MEADOW = { .tiles = map_meadow };
Map MAP_CAVE   = { .tiles = map_cave   };
Map MAP_HUT    = { .tiles = map_hut    };

static const Map* active(void) {
    if (current_map==0) return &MAP_MEADOW;
    if (current_map==1) return &MAP_CAVE;
    return &MAP_HUT;
}

void map_load(UINT8 which) { current_map = which; map_draw(); }
void map_draw(void) { set_bkg_tiles(0, 0, MAP_W, MAP_H, active()->tiles); SHOW_BKG; }

UINT8 map_tile(UINT8 tx, UINT8 ty) {
    if (tx >= MAP_W || ty >= MAP_H) return 2;
    return active()->tiles[ty*MAP_W + tx];
}

UINT8 map_is_solid(UINT8 tx, UINT8 ty) {
    UINT8 t = map_tile(tx, ty);
    if (current_map==2 && t==5) return 0; // hut floor walkable
    return (t == 2);
}
