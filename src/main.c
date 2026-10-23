#include "audio.h"
#include "config.h"
#include "gui.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CAN_YOU_HEAR_THE_MUSIC "../assets/audio/can-you-hear-the-music/Can you Hear the Music - Oppenheimer.mp3"
#define FOXY_JUMPSCARE "../assets/audio/jumpscare-sounds/foxy-scream-fnaf-1.mp3"
#define SMILE_DOG_JUMPSCARE "../assets/audio/jumpscare-sounds/jeremayjimenez-smile-dog-jumpscare-167171.mp3"

typedef struct ConfigFile ConfigFile;
typedef struct dirent dirent;

typedef enum jumpscareType {
  JUMPSCARE_EPILEPSY,
  JUMPSCARE_FLASH,
  JUMPSCARE_ZOOM,
  JUMPSCARE_NORMAL
} jumpscareType;

int main(void) {
  //============ config read ============
  ConfigFile config;
  if (parseConfig(&config) < 0) { // load photos from dir specified in
                                  // ConfigFile
    perror("parseConfig");
    return EXIT_FAILURE;
  }
  //=========== gui init ============
  srand(time(NULL));
  struct GuiImage image;
  struct GuiWindow window;
  AudioMixer_t AudioMixer;

  audio_mixer_init(&AudioMixer);

  if (guiSdlInit() != GUI_INIT_SDL_SUCCESS) {
    return EXIT_FAILURE;
  }
  guiImageInit(&image);
  if (guiWindowInit(&window) != GUI_INIT_WINDOW_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (guiImageLoad(&image, window.renderer, "assets/oppenheimer.jpg") !=
      GUI_LOAD_IMAGE_SUCCESS) {
    return EXIT_FAILURE;
  }
  guiWindowShowImage(&window, &image);

  //=========== CAN YOU HEAR THE MUSIC? ============
  play_audio(CAN_YOU_HEAR_THE_MUSIC, &AudioMixer);	

  //============ custom events ============
  Uint32 JUMPSCARE = SDL_RegisterEvents(1);
  //
  bool quit = false;
  SDL_Event e;
  while (!quit) {
    //============ process sdl events ============
    SDL_Event event = {0};
    if (SDL_WaitEvent(&event) != 1) {
      fprintf(stderr, "SDL_WaitEvent: %s\n", SDL_GetError());
      return EXIT_FAILURE;
    }
    if (e.type == SDL_QUIT) {
      quit = true;
    }
    //============ should we jumpscare ============
    if (event.type == JUMPSCARE) {
      switch (event.user.code) {
      case JUMPSCARE_NORMAL:
        // normal
        break;
      case JUMPSCARE_EPILEPSY:
        // epilepsy
        break;
      case JUMPSCARE_FLASH:
        // flash
        break;
      case JUMPSCARE_ZOOM:
        // flash
        break;
      }
      SDL_PollEvent(&event); // advance queue
    }
    guiWindowShowImage(&window, &image);

	//============ custom events ============
	Uint32 JUMPSCARE = SDL_RegisterEvents(1);
	//
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
			if (event.type == JUMPSCARE) {
				switch (event.user.code) {
					case JUMPSCARE_NORMAL:
						//normal
						break;
					case JUMPSCARE_EPILEPSY:
						//epilepsy
						break;
					case JUMPSCARE_FLASH:
						//flash
						break;
					case JUMPSCARE_ZOOM:
						//flash
						break;
				}
				SDL_PollEvent(&event); //advance queue
			}
			//============ read the directory to pick an image ============
			int jumpscareProbability = 25;
			bool isJumpscare = false; //flag condition for jumpscare
			char currImg;
			DIR *dir;
			dirent *dp;
			if ((dir = opendir(config.imageDir)) == NULL) {
				perror("opendir(config.imageDir) == NULL\n"); //check if directory exists
			} //open directory stream
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
						SDL_Event jumpscareEvent = {0};
						jumpscareEvent.type = JUMPSCARE;
						//the GUI should activate jumpscare on this if-else condition
						int typeProbability = rand() % 100;
						if (typeProbability <= config.chanceEpilepsy) {
							jumpscareEvent.user.code = JUMPSCARE_EPILEPSY;
							printf("epilepsy\n");
						} else if (typeProbability <= config.chanceZoom) {
							jumpscareEvent.user.code = JUMPSCARE_ZOOM;
							printf("zoom\n");
						} else if (typeProbability <= config.chanceFlash) {
							jumpscareEvent.user.code = JUMPSCARE_FLASH;
							printf("flash\n");
						} else {	
							jumpscareEvent.user.code = JUMPSCARE_NORMAL;
							printf("normal :(\n");
						}
						SDL_PushEvent(&jumpscareEvent); //push jumpscare
					}
				}
			}

			if (currFile == NULL) {
				printf("reset dir position back to beginning\n");
				rewinddir(dir); //reset directory stream as we reached the end of the dir
			}
        }
	}
	return EXIT_SUCCESS;
	}
}