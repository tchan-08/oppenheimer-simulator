#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
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



    while ((dir = opendir(config.imageDir)) != NULL) { //open directory stream
        int index = 0;
        while (index <= 1024) {
            struct dirent *currFile = readdir(dir); //read directory stream, get pointer to image
            printf("got file success\n");
            bool notImg = false;
            if (currFile != NULL) { //not the end of the directory
                int currChange = rand() % 100;
                printf("%d, %d\n", strcmp(currFile->d_name, "."), strcmp(currFile->d_name, ".."));
                if (strcmp(currFile->d_name, ".") == 0 || strcmp(currFile->d_name, "..") == 0) {
                    printf("Not an image\n");
                    notImg = true;
                }
                if (!notImg) {
                    printf("%s\n", currFile->d_name);
                    if ((double)currChange <= probability) {
                        isJumpscare = true; //activate jumpscare
                    }
                    isJumpscare = false; //reset jumpscare
                    index = (index + 1) % 1025; //increment photo index (with wrap around)
                }
            }

            if (currFile == NULL) {
                printf("reset dir position back to beginning\n");
                rewinddir(dir); //reset directory stream as we reached the end of the dir
            }
            if (!notImg) {
                sleep(config.interval);
            }
        }
    }
}