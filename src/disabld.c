#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "pid_mng.h"
#include "utils.h"

void enable_keyboard(const char *);
void disable_keyboard(const char *);
void list_input_devices();

int main(int argc, char *argv[])
{
    if (argc == 1 || argc > 3 ||
        (strcmp(argv[1], "-d") != 0 && strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-ls") != 0))
    {
        printf("Usage:\n");
        printf("%s -d /dev/input/eventX: Disable keyboard\n", argv[0]);
        printf("%s -e /dev/input/eventX: Enable keyboard\n", argv[0]);
        printf("%s -ls: List input devices\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *flag = argv[1];
    const char *device = argv[2];

    if (strcmp(flag, "-ls") == 0)
    {
        list_input_devices();
        return EXIT_SUCCESS;
    }
    else if (strcmp(flag, "-d") == 0)
    {
        disable_keyboard(device);
    }
    else if (strcmp(flag, "-e") == 0)
    {
        enable_keyboard(device);
    }

    return EXIT_SUCCESS;
}

void disable_keyboard(const char *device)
{
    pid_t pid = getpid();
    if (device == NULL)
    {
        printf("\nEmpty device, give a device name");
        exit(EXIT_FAILURE);
    }

    int keyboard_fd = open(device, O_RDONLY);
    if (keyboard_fd == -1)
    {
        perror("Error opening keyboard device");
        exit(EXIT_FAILURE);
    }

    int result = ioctl(keyboard_fd, EVIOCGRAB, 1);
    if (result == -1)
    {
        perror("Error disabling keyboard");
        close(keyboard_fd);
        exit(EXIT_FAILURE);
    }
    
    store_pid(device, pid);

    printf("Keyboard disabled using pid: %d\n", getpid());
    // close(keyboard_fd);

    if (daemon(0, 0))
    {
        perror("this daemon don't work!!");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        sleep(60);
    }
}

void enable_keyboard(const char *device)
{
    int keyboard_fd = open(device, O_RDONLY);
    if (keyboard_fd == -1)
    {
        perror("Error opening keyboard device");
        exit(EXIT_FAILURE);
    }

    int result = ioctl(keyboard_fd, EVIOCGRAB, 0);
    if (result == -1)
    {
        perror("Error enabling keyboard");
        close(keyboard_fd);
        exit(EXIT_FAILURE);
    }

    printf("Keyboard enabled.\n");
    close(keyboard_fd);
}