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
    char currImg;
    ConfigFile config;
    parseConfig(&config); //load photos from dir specified in ConfigFile
    DIR *dir;
    dirent *dp;

    if ((dir = opendir(config.imageDir)) == NULL) {
        perror("opendir(config.imageDir) == NULL\n");
    }

    while ((dir = opendir(config.imageDir)) != NULL) {
        int index = 0;
        while (index <= 1024) {
            if (readdir(dir) != 0) {
                perror(readdir(dir));
            }
            int currFile = readdir(dir);

            while (currFile != NULL) { //not the end of the directory
                int currChange = rand() % 100;
                if ((double)currChange <= probability) {
                    isJumpscare = true; //activate jumpscare
                }
                isJumpscare = false; //reset jumpscare
                index = (index + 1) % 1025; //increment photo index (with wrap around)
            }

            if (currFile == NULL) {
                rewinddir(dir); //WE'RE BACK AT THE STARTTTTTT
            }
        }
    }
}