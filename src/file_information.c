#include "file_information.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int get_octal_chmod(char *full_path) {
	struct stat buf;
	stat(full_path, &buf);

	int permission = buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

	return permission;
}

long int get_file_size(char *full_path) {
    struct stat buffer;
    int status;

    status = stat(full_path, &buffer);

    if(status == 0) {
        return buffer.st_size;
    } else
        return -1;
}

time_t get_file_last_access(char *full_path) {
	struct stat filestat;

	stat(full_path, &filestat);

	return filestat.st_mtime;
}
