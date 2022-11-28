#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> 
#include <stdbool.h>  

#define FULL_SIZE_OF_PATH 100

typedef struct {
	char* name;
	char* size;
	bool test;
	int quantity;
} Parameter;

void print_spaces(int depth) {
	for (int i = 0; i < depth; i++) {
		printf("    ");
	}
}

long int get_file_size(char *path, char *file) {
	char full_name[FULL_SIZE_OF_PATH];
	snprintf(full_name, FULL_SIZE_OF_PATH, "%s%s%s", path, "/",  file);

	FILE *fp = fopen(full_name, "ra");

	if (fp == NULL)
		return -1;

	if (fseek(fp, 0, SEEK_END) < 0) {
		fclose(fp);
		return -1;
	}

	long size = ftell(fp);
	// release the resources when not required
	
	fclose(fp);
	return size;
}

void print_file(char *path, char* file_name, int depth) {
	long int size = get_file_size(path, file_name);

	char name[FULL_SIZE_OF_PATH];
	snprintf(name, FULL_SIZE_OF_PATH, "%s%s%s", path, "/",  file_name);

	// print_spaces(depth);

	// printf("* (%d) %s\n", size, name);
	printf("%s\n", name);
}

void verify_if_can_be_printed(Parameter parameter, char *path, char *file_name, int depth) {
	int successeful_parameters = 0;

	if (parameter.name != NULL && strstr(file_name, parameter.name) != NULL) {
		successeful_parameters++;
	}

	if (parameter.size != NULL) {
		long int size = get_file_size(path, file_name);

		char unit = parameter.size[strlen(parameter.size) - 1];
		int multiplier = unit == 'k' ? 1024 : unit == 'M' ? 1024 * 1024 : unit == 'G' ? 1024 * 1024 * 1024 : 1;

		char threshold_as_string[FULL_SIZE_OF_PATH];
		strncpy(threshold_as_string, parameter.size + 1, strlen(parameter.size) - 2);
		long int threshold = (long) atoi(threshold_as_string) * multiplier;

		if (parameter.size[0] == '+') {
			if (size >= threshold) {
				successeful_parameters++;
			}
		} else {
			if (size <= threshold) {
				successeful_parameters++;
			}
		}
	}

	if (parameter.quantity == successeful_parameters)
		print_file(path, file_name, depth);

}

int show_files_in_specific_path(int depth, char *path, Parameter parameter) {

	DIR *d;
	struct dirent *dir;
	d = opendir(path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			// Print file.
			if (dir->d_type == DT_REG) {
				verify_if_can_be_printed(parameter, path, dir->d_name, depth);
			} else {
				// Print directory.
				if (strcmp("..", dir->d_name) && strcmp(".", dir->d_name)) {
					char name[FULL_SIZE_OF_PATH];
					snprintf(name, FULL_SIZE_OF_PATH, "%s%s%s", path, "/",  dir->d_name);

					show_files_in_specific_path(depth + 1, name, parameter);
				}
			}
		}

		closedir(d);
	}

	return 0;
}

// Verify if the first parameter is a file
int main(int argv, char *argc[]) {
	FILE* file;
	Parameter parameter;

	if (argv < 2) {
		printf("The path needs to be specified!\n");
		return 1;
	}

	if (argv >= 3) {

		parameter.name = NULL;
		parameter.test = false;
		parameter.size = NULL;
		parameter.quantity = 0;

		for (int i = 2; i < argv; i += 2) {
			if (!strcmp("-test", argc[i])) {
				parameter.test = true;
			}
		}

		for (int i = 2; i < argv; i += 2) {
			if (argc[i][0] != '-') {
				printf("Multiple parameters are not allowed, are you thing to use a flag? TIP: start with -\n");
				return 1;
			}

			if (!strcmp("-name", argc[i])) {
				parameter.name = argc[i + 1];
				parameter.quantity++;
			}
			// TODO: verification if the value is correct.
			else if (!strcmp("-size", argc[i])) {
				parameter.size = argc[i + 1];
				parameter.quantity++;
			}
			else if (!strcmp("-test", argc[i])) {
				i--;
				continue;
			}
			else {
				printf("Incorrect parameters, try again!\n");
				return 1;
			}

			if (parameter.test == true)
				printf("La valeur du flag %s est %s\n", argc[i], argc[i+1]);
		}
	}

	show_files_in_specific_path(0, argc[1], parameter);

	return 0;
}