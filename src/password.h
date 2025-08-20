#ifndef PASSWORD_H
#define PASSWORD_H
#include "party.h"
#include "map.h"
#include "npc.h"

void encode_password(char out[17], Party* p, UINT8 map, UINT8 px, UINT8 py);
UINT8 decode_password(const char in[17], Party* p, UINT8* map, UINT8* px, UINT8* py);

#endif
