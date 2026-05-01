#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

#define RECORDINGS_DIR "/home/petalinux/recordings"

static size_t get_name_from_time(char* file_name, size_t max_name_size)
{
    time_t current_time;
    time(&current_time);

    struct tm *time_info = localtime(&current_time);

    return strftime(file_name, max_name_size, "rec_%d-%m-%Y_%H-%M-%S.txt", time_info);
}

FILE* recordings_new()
{
    // create recordings folder if doesn't exists
    if(mkdir(RECORDINGS_DIR, 511) < 0) {
        if(errno != EEXIST) {
            perror("mkdir");
            return NULL;
        }
    }

    char file_name[64] = {0};
    get_name_from_time(file_name, sizeof(file_name));

    // Join folder path and filename
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s/%s", RECORDINGS_DIR, file_name);

    FILE *f_ptr = fopen(file_path, "a");
    if(f_ptr == NULL) {
        perror("recordings: fopen");
        return NULL;
    }

    return f_ptr;
}

int recordings_close(FILE *f_ptr) {
    return fclose(f_ptr);
}
