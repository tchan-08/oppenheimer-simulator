#include <stdio.h>
#include <dirent.h>
#include "config.h"
typedef struct ConfigFile ConfigFile;
typedef struct dirent dirent;

int main () {
    double probability = 1/6;
    ConfigFile config;
    const char directory[] = "../assets/images/";
    DIR *dir;
    dirent *dp;

    if ((dir = opendir(directory)) == NULL) {
        perror("WTF OPPENHEIMER ABORT");
    }

    while ((dir = opendir(directory)) != NULL) {
        //do something
    }
}