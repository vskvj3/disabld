#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void list_input_devices() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/sys/class/input");
    if (dir == NULL) {
        perror("Error opening input directory");
        exit(EXIT_FAILURE);
    }

    printf("Input devices:\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "event") != NULL) {
            char path[256];
            snprintf(path, sizeof(path), "/sys/class/input/%s/device/name", entry->d_name);

            FILE *file = fopen(path, "r");
            if (file != NULL) {
                char deviceName[256];
                fgets(deviceName, sizeof(deviceName), file);
                printf("/dev/input/%s\t: %s", entry->d_name, deviceName);
                fclose(file);
            }
        }
    }
    closedir(dir);
}

