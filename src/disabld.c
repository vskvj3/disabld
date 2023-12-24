#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void listInputDevices() {
    printf("Input devices:\n");
    system("");
}

int main(int argc, char *argv[]) {
    if (argc != 2 ) {
        printf("Usage:\n");
        printf("%s -ls: List input devices using evtest\n", argv[0]);
        printf("%s /dev/input/eventX: Disable and re-enable a specific input device\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-ls") == 0) {
        listInputDevices();
        return EXIT_SUCCESS;
    }

    const char *keyboard_device = argv[1];

    int keyboard_fd = open(keyboard_device, O_RDONLY);
    if (keyboard_fd == -1) {
        perror("Error opening keyboard device");
        return EXIT_FAILURE;
    }

    int result = ioctl(keyboard_fd, EVIOCGRAB, 1); 
    if (result == -1) {
        perror("Error disabling keyboard");
        close(keyboard_fd);
        return EXIT_FAILURE;
    }

    printf("Keyboard disabled. Press any key to re-enable.\n");
    getchar(); 

    result = ioctl(keyboard_fd, EVIOCGRAB, 0); 
    if (result == -1) {
        perror("Error enabling keyboard");
        close(keyboard_fd);
        return EXIT_FAILURE;
    }

    close(keyboard_fd);
    printf("Keyboard re-enabled.\n");

    return EXIT_SUCCESS;
}
