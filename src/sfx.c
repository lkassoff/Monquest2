#include "sfx.h"
#include <gb/hardware.h>

static void beep(UINT8 freq, UINT8 len){
    NR10_REG = 0x16;
    NR11_REG = 0x40;
    NR12_REG = 0xF3;
    NR13_REG = freq;
    NR14_REG = 0xC0;
    for(UINT8 i=0;i<len;i++) wait_vbl_done();
    NR12_REG = 0x00;
}

void sfx_hit(void){ beep(0xA0, 5); }
void sfx_capture(void){ beep(0x60, 12); }
