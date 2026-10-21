#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "audio.h"
#include "config.h"
#include "gui.h"

#define CAN_YOU_HEAR_THE_MUSIC "assets/can-you-hear-the-music/'Can you Hear the Music' - Oppenheimer.mp3"
#define FOXY_JUMPSCARE "assets/audio/jumpscare-sounds/foxy-scream-fnaf-1.mp3"

typedef struct ConfigFile ConfigFile;
typedef struct dirent dirent;

typedef enum jumpscareType {
    JUMPSCARE_EPILEPSY,
    JUMPSCARE_FLASH,
    JUMPSCARE_ZOOM,
    JUMPSCARE_NORMAL
} jumpscareType;

int main () {
    srand(time(NULL));
    struct GuiState state;
    struct GuiWindow window;
    AudioMixer_t AudioMixer;

    audio_mixer_init(&AudioMixer);

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

    play_audio(CAN_YOU_HEAR_THE_MUSIC, &AudioMixer);

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
                            //the GUI should activate jumpscare on this if-else condition
                            int typeProbability = rand() % 100;
                            if (typeProbability <= config.chanceEpilepsy) {
                                printf("epilepsy\n");
                            } else if (typeProbability <= config.chanceZoom) {

                                printf("zoom\n");
                            } else if (typeProbability <= config.chanceFlash) {
                                printf("flash\n");
                            } else {
                                //config.chanceNormal
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
    
    clean_up(&AudioMixer);
    SDL_DestroyWindow(window.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}