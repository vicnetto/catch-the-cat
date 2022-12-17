CC=gcc

CFLAGS=-Wall -g3

INCLUDE=-lmagic

ARCHIVESdotO=obj/main.o

all: ftc

ftc: $(ARCHIVESdotO)
	$(CC) $(ARCHIVESdotO) -o ftc $(INCLUDE)

main.o: src/main.c
	$(CC) $(CFLAGS) -o obj/main.o -c src/main.c -I $(INCLUDE)

# MegaMimes.o: src/lib/MegaMimes.c
# 	$(CC) $(CFLAGS) -o obj/MegaMimes.o -c src/lib/MegaMimes.c -I $(INCLUDE)

# ftc: $(ARCHIVESdotO)
# 	$(CC) $(ARCHIVESdotO) -o ftc $(INCLUDE)

# main.o: src/main.c
# 	$(CC) $(CFLAGS) src/main.c $(INCLUDE)

# MegaMimes.o: src/lib/MegaMimes.c
# 	$(CC) $(CFLAGS) src/lib/MegaMimes.c $(INCLUDE)

# all:
# 	mkdir -p obj
# 	gcc -Wall -g3 -lmagic -o obj/main.o -c src/main.c
# 	gcc -Wall -g3 -lmagic -o obj/MegaMimes.o -c src/lib/MegaMimes.c
# 	# gcc -std=c11 -Wall -I -g3 -o obj/main.o -c src/main.c -I src/include
# 	gcc -Wall -g3 -lmagic obj/main.o obj/MegaMimes.o -o ftc
# 	# gcc -std=c11 -Wall -I g3 -o ftc obj/main.o -I src/include