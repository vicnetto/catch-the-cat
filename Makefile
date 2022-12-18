CC=gcc

CFLAGS=-Wall -g3

ARCHIVESdotO=obj/main.o obj/MegaMimes.o obj/parameter.o obj/regex_evaluation.o obj/file_information.o

all: create_object_directory ftc

create_object_directory: 
	mkdir -p obj

ftc: $(ARCHIVESdotO)
	$(CC) $(ARCHIVESdotO) -o ftc

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o obj/main.o -c src/main.c

obj/MegaMimes.o: src/lib/MegaMimes.c
	$(CC) $(CFLAGS) -o obj/MegaMimes.o -c src/lib/MegaMimes.c

obj/parameter.o: src/parameter.c
	$(CC) $(CFLAGS) -o obj/parameter.o -c src/parameter.c

obj/regex_evaluation.o: src/regex_evaluation.c
	$(CC) $(CFLAGS) -o obj/regex_evaluation.o -c src/regex_evaluation.c

obj/file_information.o: src/file_information.c
	$(CC) $(CFLAGS) -o obj/file_information.o -c src/file_information.c

clean:
	rm -rf obj