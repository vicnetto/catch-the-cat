all:
	mkdir -p obj
	gcc -Wall -g3 -lmagic -o obj/main.o -c src/main.c
	# gcc -std=c11 -Wall -I -g3 -o obj/main.o -c src/main.c -I src/include
	gcc -Wall -g3 -lmagic -o ftc obj/main.o
	# gcc -std=c11 -Wall -I g3 -o ftc obj/main.o -I src/include

test:
	gcc src/main.c -o ftc && ./ftc /home/vicnetto/personal/6-victor/4-telecom-nancy/annee_2/systeme/tp -test Edgar

name:
	gcc src/main.c -o ftc && ./ftc /home/vicnetto/personal/6-victor/4-telecom-nancy/annee_2/systeme/tp -name haha

size:
	gcc src/main.c -o ftc && ./ftc /home/vicnetto/personal/6-victor/4-telecom-nancy/annee_2/systeme/tp -size +10k
