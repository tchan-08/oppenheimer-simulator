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
    JUMPSCARE_EPILEPSY,
    JUMPSCARE_FLASH,
    JUMPSCARE_ZOOM,
    JUMPSCARE_NORMAL
} jumpscareType;

int main () {
	//============ config read ============
	ConfigFile config;
	if (parseConfig(&config) < 0) { //load photos from dir specified in ConfigFile
		perror("parseConfig");
		return EXIT_FAILURE;
	}
	//=========== gui init ============
    srand(time(NULL));
    struct GuiState state;
    struct GuiWindow window;

    if (guiSdlInit() != GUI_INIT_SDL_SUCCESS) {
        return EXIT_FAILURE;
    }
    guiStateInit(&state);
    if (guiWindowInit(&window) != GUI_INIT_WINDOW_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (guiStateLoadImage(&state, window.renderer, "assets/oppenheimer.jpg") !=
        GUI_LOAD_IMAGE_SUCCESS) {
        return EXIT_FAILURE;
    }
    guiWindowShowImage(&window, &state);
	//============ mainloop ============
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (1) {
			//============ process sdl events ============
			SDL_Event event = {0};
			if (SDL_WaitEvent(&event) != 1) {
				fprintf(stderr,"SDL_WaitEvent: %s\n",SDL_GetError());
				return EXIT_FAILURE;
			}
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
			//============ should we jumpscare ============
			continue;
			//============ read the directory to pick an image ============
			int jumpscareProbability = 25;
			bool isJumpscare = false; //flag condition for jumpscare
			char currImg;
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
				}
			}
        }
        
    }

    SDL_DestroyWindow(window.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
