# GBDK-2020 Makefile for MonQuest v4
PROJECT_NAME = monquest
SRC = src/main.c src/map.c src/mapdata.c src/graphics.c src/party.c src/creatures.c src/menu.c src/password.c src/battle.c src/npc.c src/sfx.c

CFLAGS = -Wl-yp0x143=0x80 -Wm-yC -Wm-ynMonQuest -Wm-yo4

$(PROJECT_NAME).gb: $(SRC)
	lcc -o $@ $(CFLAGS) $(SRC)

clean:
	rm -f $(PROJECT_NAME).gb *.o *.lst *.map *.ihx
