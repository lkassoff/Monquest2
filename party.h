#ifndef PARTY_H
#define PARTY_H
#include "creatures.h"
#define PARTY_MAX 3

typedef struct {
    Mon mons[PARTY_MAX];
    UINT8 count;
    UINT8 leader; // index used in battle
} Party;

extern Party party;

void party_init(void);
UINT8 party_is_full(void);
UINT8 party_add(const Mon* m);
void party_print(void);
void party_choose_leader(void);

#endif
