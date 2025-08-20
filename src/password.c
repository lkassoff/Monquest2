#include "password.h"
#include <gb/gb.h>

extern UINT8 quest_has_orbkit;
extern UINT8 quest_captured_any;

static const char ALPH[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"; // 32

static void put5(char* out, UINT16 v){ out[0]=ALPH[v&31]; out[1]=ALPH[(v>>5)&31]; }
static UINT16 get5(const char* in){ UINT8 map[256]={0}; for(UINT8 i=0;i<32;i++) map[(UINT8)ALPH[i]]=i; return map[(UINT8)in[0]] | (map[(UINT8)in[1]]<<5); }

void encode_password(char out[17], Party* p, UINT8 map, UINT8 px, UINT8 py){
    UINT16 a = (p->count & 3) | ((map & 3)<<2) | ((px & 31)<<4);
    UINT16 b = (py & 31) | (((px+py+map+p->count) & 31)<<5);
    put5(out+0,a); put5(out+2,b);
    UINT8 idx=4;
    for(UINT8 i=0;i<p->count && i<2;i++){
        Mon* m=&p->mons[i];
        UINT16 s0=(m->species & 3)|((m->level & 63)<<2);
        UINT16 s1=(m->hp & 127)|((m->atk & 63)<<7);
        UINT16 s2=(m->spd & 63)|((m->exp & 63)<<6);
        put5(out+idx,s0); idx+=2;
        put5(out+idx,s1); idx+=2;
        put5(out+idx,s2); idx+=2;
    }
    UINT16 q = (quest_has_orbkit & 1) | ((quest_captured_any & 1)<<1);
    put5(out+idx,q); idx+=2;
    out[16]=0;
}

UINT8 decode_password(const char in[17], Party* p, UINT8* map, UINT8* px, UINT8* py){
    Party np; np.count=0; np.leader=0;
    UINT16 a=get5(in+0); UINT16 b=get5(in+2);
    UINT8 count = a & 3; *map=(a>>2)&3; *px=(a>>4)&31; *py=b&31;
    UINT8 chk=(b>>5)&31; if (chk != (UINT8)((*px+*py+*map+count)&31)) return 0;
    UINT8 idx=4;
    for(UINT8 i=0;i<count && i<2;i++){
        Mon m;
        UINT16 s0=get5(in+idx); idx+=2;
        UINT16 s1=get5(in+idx); idx+=2;
        UINT16 s2=get5(in+idx); idx+=2;
        m.species=(Species)(s0&3);
        m.type=(ElemType)((UINT8)m.species%3);
        m.level=(s0>>2)&63;
        m.hp=s1&127;
        m.atk=(s1>>7)&63;
        m.spd=s2&63;
        m.exp=(s2>>6)&63;
        m.move_count=1; m.moves[0]=MOVE_TACKLE;
        np.mons[np.count++]=m;
    }
    UINT16 q=get5(in+idx);
    quest_has_orbkit = q & 1;
    quest_captured_any = (q>>1)&1;

    *p = np;
    return 1;
}
