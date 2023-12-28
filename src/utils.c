#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

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

/**
 * \brief Read configuration from config file
 */
int read_conf_file(int reload, char *app_name, char *conf_file_name)
{
	FILE *conf_file = NULL;
	int ret = -1, delay;

	if (conf_file_name == NULL) return 0;

	conf_file = fopen(conf_file_name, "r");

	if (conf_file == NULL) {
		syslog(LOG_ERR, "Can not open config file: %s, error: %s",
				conf_file_name, strerror(errno));
		return -1;
	}

	ret = fscanf(conf_file, "%d", &delay);

	if (ret > 0) {
		if (reload == 1) {
			syslog(LOG_INFO, "Reloaded configuration file %s of %s",
				conf_file_name,
				app_name);
		} else {
			syslog(LOG_INFO, "Configuration of %s read from file %s",
				app_name,
				conf_file_name);
		}
	}

	fclose(conf_file);

	return ret;
}

