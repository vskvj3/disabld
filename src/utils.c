#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

/**
 * \brief list the input devices
 */
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
char* read_conf_file(int reload, char *app_name, char *conf_file_name)
{
	FILE *conf_file = NULL;
	int ret = -1;
    char * device_name;

	if (conf_file_name == NULL) return 0;

	conf_file = fopen(conf_file_name, "r");

	if (conf_file == NULL) {
		syslog(LOG_ERR, "Can not open config file: %s, error: %s",
				conf_file_name, strerror(errno));
		return -1;
	}

	ret = fscanf(conf_file, "%s", device_name);

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

	return device_name;
}

/**
 * \brief Print help section
 */
void print_help(char* app_name)
{
	printf("\n Usage: %s [OPTIONS]\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 Print this help\n");
	printf("   -c --conf_file filename   Set configuration file\n");
	printf("   -t --test_conf filename   Test configuration file\n");
	printf("   -l --log_file  filename   Set log file\n");
	printf("   -d --daemon               Start daemon\n");
	printf("   -p --pid_file  filename   set custom PID file\n");
	printf("\n");
}

