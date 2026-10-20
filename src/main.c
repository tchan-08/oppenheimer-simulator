#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "gui.h"

typedef struct ConfigFile ConfigFile;
typedef struct dirent dirent;

typedef enum jumpscareType {
    JUMPSCARE_FLASH = 25,
    JUMPSCARE_ZOOM = 20,
    JUMPSCARE_EPILEPSY = 10,
    JUMPSCARE_NORMAL = 45
} jumpscareType;

int main () {
    srand(time(NULL));
    struct GuiState state;
    struct GuiWindow window;

    if (guiSdlInit() != GUI_INIT_SDL_SUCCESS) {
        return 1;
    }

    guiStateInit(&state);

    if (guiWindowInit(&window) != GUI_INIT_WINDOW_SUCCESS) {
        return 1;
    }

    if (guiStateLoadImage(&state, window.renderer, "assets/oppenheimer.jpg") !=
        GUI_LOAD_IMAGE_SUCCESS) {
        return 1;
    }

    guiWindowShowImage(&window, &state);

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = 1;
        }
        int jumpscareProbability = 25;
        bool isJumpscare = false; //flag condition for jumpscare
        char currImg;
        ConfigFile config;
        parseConfig(&config); //load photos from dir specified in ConfigFile
        DIR *dir;
        dirent *dp;

        if ((dir = opendir(config.imageDir)) == NULL) {
            perror("opendir(config.imageDir) == NULL\n"); //check if directory exists
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
                        if (currChange <= jumpscareProbability) {
                            jumpscareType type;
                            int typeProbability = rand() % 100;
                            if (typeProbability <= JUMPSCARE_EPILEPSY) {
                                type = JUMPSCARE_EPILEPSY;
                                printf("epilepsy\n");
                            } else if (typeProbability <= JUMPSCARE_ZOOM) {
                                type = JUMPSCARE_ZOOM;
                                printf("zoom\n");
                            } else if (typeProbability <= JUMPSCARE_FLASH) {
                                type = JUMPSCARE_FLASH;
                                printf("flash\n");
                            } else {
                                type = JUMPSCARE_NORMAL;
                                printf("normal :(\n");
                            }
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
        
    }

    SDL_DestroyWindow(window.window);
    SDL_Quit();

    return EXIT_SUCCESS;
    srand(time(NULL));
    int jumpscareProbability = 25;
    bool isJumpscare = false; //flag condition for jumpscare
    char currImg;
    ConfigFile config;
    parseConfig(&config); //load photos from dir specified in ConfigFile
    DIR *dir;
    dirent *dp;

    if ((dir = opendir(config.imageDir)) == NULL) {
        perror("opendir(config.imageDir) == NULL\n"); //check if directory exists
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
                    if (currChange <= jumpscareProbability) {
                        jumpscareType type;
                        int typeProbability = rand() % 100;
                        if (typeProbability <= JUMPSCARE_EPILEPSY) {
                            type = JUMPSCARE_EPILEPSY;
                            printf("epilepsy\n");
                        } else if (typeProbability <= JUMPSCARE_ZOOM) {
                            type = JUMPSCARE_ZOOM;
                            printf("zoom\n");
                        } else if (typeProbability <= JUMPSCARE_FLASH) {
                            type = JUMPSCARE_FLASH;
                            printf("flash\n");
                        } else {
                            type = JUMPSCARE_NORMAL;
                            printf("normal :(\n");
                        }
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