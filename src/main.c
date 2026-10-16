#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include "config.h"

typedef struct ConfigFile ConfigFile;
typedef struct dirent dirent;

int main () {
    srand(time(NULL));
    double probability = 1/6 * 100;
    bool isJumpscare = false; //flag condition for jumpscare
    ConfigFile config;
    parseConfig(&config); //load photos from dir specified in ConfigFile
    DIR *dir;
    dirent *dp;

    if ((dir = opendir(config.imageDir)) == NULL) {
        perror("WTF OPPENHEIMER ABORT");
    }

    while ((dir = opendir(config.imageDir)) != NULL) {
        int index = 0;
        while (index <= 1024) {
            int currChange = rand() % 100;
            if ((double)currChange <= probability) {
                isJumpscare = true; //activate jumpscare
            }
            isJumpscare = false; //reset jumpscare
            index = (index + 1) % 1024; //increment photo index (with wrap around)
        }
    }
}