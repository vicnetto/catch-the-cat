all:
	mkdir -p obj
	gcc -std=c11 -Wall -g3 -o obj/main.o -c src/main.c -I src/include
	gcc -std=c11 -Wall -g3 -o ftc obj/main.o -I src/include

test:
	gcc src/main.c -o ftc && ./ftc /home/vicnetto/personal/6-victor/4-telecom-nancy/annee_2/systeme/tp -test Edgar

name:
	gcc src/main.c -o ftc && ./ftc /home/vicnetto/personal/6-victor/4-telecom-nancy/annee_2/systeme/tp -name haha

size:
	gcc src/main.c -o ftc && ./ftc /home/vicnetto/personal/6-victor/4-telecom-nancy/annee_2/systeme/tp -size +10k
