#ifndef MAP_H
#define MAP_H
#include <gb/gb.h>

#define MAP_W 20
#define MAP_H 18

typedef struct {
    const unsigned char *tiles; // MAP_W*MAP_H
} Map;

extern Map MAP_MEADOW;
extern Map MAP_CAVE;
extern Map MAP_HUT;

extern UINT8 encounter_enabled;
extern UINT8 current_map; // 0 meadow, 1 cave, 2 hut

void map_load(UINT8 which);
void map_draw(void);
UINT8 map_is_solid(UINT8 tx, UINT8 ty);
UINT8 map_tile(UINT8 tx, UINT8 ty);

#endif
