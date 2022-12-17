CC=gcc

CFLAGS=-Wall -g3

ARCHIVESdotO=obj/main.o obj/MegaMimes.o

all: create_object_directory ftc

create_object_directory:
	mkdir -p obj

ftc: $(ARCHIVESdotO)
	$(CC) $(ARCHIVESdotO) -o ftc

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o obj/main.o -c src/main.c

obj/MegaMimes.o: src/lib/MegaMimes.c
	$(CC) $(CFLAGS) -o obj/MegaMimes.o -c src/lib/MegaMimes.c