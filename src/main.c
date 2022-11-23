#include <stdio.h>
#include <string.h>

int main(int argv, char *argc[]) {

	if (argv < 2) {
		printf("The file needs to be specified!\n");
		return 1;
	}

	if (!strcmp(argc[2], "-test"))
		printf("La valeur du flag -xxxx est yyyy\n");
}
