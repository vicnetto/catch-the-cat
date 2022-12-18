#include "file_information.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

long int get_file_size(char *full_path) {
    struct stat buffer;
    int status;

    status = stat(full_path, &buffer);

    if(status == 0) {
        return buffer.st_size;
    } else
        return -1;

	// FILE *fp = fopen(full_path, "ra");

	// if (fp == NULL)
	// 	return -1;

	// if (fseek(fp, 0, SEEK_END) < 0) {
	// 	fclose(fp);
	// 	return -1;
	// }

	// long size = ftell(fp);

	// fclose(fp);
	// return size;
}

time_t get_file_last_access(char *full_path) {
	struct stat filestat;

	stat(full_path, &filestat);

	return filestat.st_mtime;
}
