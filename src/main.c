#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "audio.h"
#include "config.h"
#include "gui.h"

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

static char *images[] = {
    "../assets/oppenheimer.jpg",
    "../assets/oppenheimer-cigar.jpg",
    "../assets/oppenheimer-hand.jpg"
};

#define IMAGE_COUNT (sizeof(images) / sizeof(images[0]))

static void show_random_image(struct GuiState *state, struct GuiWindow *window,
                              SDL_Renderer *renderer)
{
    size_t index = (size_t)(rand() % IMAGE_COUNT);

    if (guiStateLoadImage(state, renderer, images[index]) ==
        GUI_LOAD_IMAGE_SUCCESS) {
        guiWindowShowImage(window, state);
    }
}

typedef struct {
    Uint32 eventType;
} TimerData_t;

Uint32 jumpscare_timer_callback(Uint32 interval, void *param)
{
    TimerData_t *data = param;

    SDL_Event event = {0};
    event.type = data->eventType;

    SDL_PushEvent(&event);

    return interval; // repeat the timer
}


// int main () 
signed main(void) {
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
    AudioMixer_t AudioMixer = {0};

    audio_mixer_init(&AudioMixer);

    if (guiSdlInit() != GUI_INIT_SDL_SUCCESS) {
        return EXIT_FAILURE;
    }
    guiStateInit(&state);
    if (guiWindowInit(&window) != GUI_INIT_WINDOW_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (guiStateLoadImage(&state, window.renderer, "../assets/oppenheimer.jpg") !=
        GUI_LOAD_IMAGE_SUCCESS) {
        return EXIT_FAILURE;
    }
    guiWindowShowImage(&window, &state);
	//============ custom events ============
	// Uint32 JUMPSCARE = SDL_RegisterEvents(1);
	// Register custom events
	Uint32 events = SDL_RegisterEvents(2);

	if (events == (Uint32)-1) {
		fprintf(stderr, "SDL_RegisterEvents: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	Uint32 JUMPSCARE = events;
	Uint32 JUMPSCARE_TICK = events + 1;

	// Start periodic timer
	TimerData_t timerData = {
		.eventType = JUMPSCARE_TICK
	};

	SDL_TimerID jumpscareTimer = SDL_AddTimer(
		(Uint32)(config.interval * 1000),
		jumpscare_timer_callback,
		&timerData
	);

	if (jumpscareTimer == 0) {
		fprintf(stderr, "SDL_AddTimer: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	//============ mainloop ============
	play_audio(CAN_YOU_HEAR_THE_MUSIC, &AudioMixer);	
    int quit = 0;
    // SDL_Event e;
	while (!quit) {
		SDL_Event event = {0};

		if (SDL_WaitEvent(&event) != 1) {
			fprintf(stderr, "SDL_WaitEvent: %s\n", SDL_GetError());
			break;
		}

		if (event.type == SDL_QUIT) {
			quit = 1;
		}

		// Timer tick: decide whether to trigger a jumpscare
		else if (event.type == JUMPSCARE_TICK) {
			int currChange = rand() % 100;

			if (currChange < 75) { // 75% chance per timer tick
				SDL_Event jumpscareEvent = {0};
				jumpscareEvent.type = JUMPSCARE;

				int typeProbability = rand() % 100;

				if (typeProbability < config.chanceEpilepsy) {
					jumpscareEvent.user.code = JUMPSCARE_EPILEPSY;
				} else if (typeProbability <
						config.chanceEpilepsy + config.chanceZoom) {
					jumpscareEvent.user.code = JUMPSCARE_ZOOM;
				} else if (typeProbability <
						config.chanceEpilepsy + config.chanceZoom +
						config.chanceFlash) {
					jumpscareEvent.user.code = JUMPSCARE_FLASH;
				} else {
					jumpscareEvent.user.code = JUMPSCARE_NORMAL;
				}

				SDL_PushEvent(&jumpscareEvent);
			}
		}

		// Handle jumpscare events
		else if (event.type == JUMPSCARE) {
			switch (event.user.code) {
				case JUMPSCARE_NORMAL:
					show_random_image(&state, &window, window.renderer);
					play_audio(FOXY_JUMPSCARE, &AudioMixer);
					break;

				case JUMPSCARE_EPILEPSY:
					// Use image changes rather than rapid flashing.
					show_random_image(&state, &window, window.renderer);
					play_audio(SMILE_DOG_JUMPSCARE, &AudioMixer);
					break;

				case JUMPSCARE_FLASH:
					SDL_SetRenderDrawColor(
						window.renderer, 255, 255, 255, 255);
					SDL_RenderClear(window.renderer);
					SDL_RenderPresent(window.renderer);
					SDL_Delay(100);
					guiWindowShowImage(&window, &state);
					break;

				case JUMPSCARE_ZOOM: {
					SDL_Texture *texture = IMG_LoadTexture(
						window.renderer, images[rand() % IMAGE_COUNT]);

					if (texture != NULL) {
						int screenWidth, screenHeight;

						SDL_GetRendererOutputSize(
							window.renderer, &screenWidth, &screenHeight);

						SDL_Rect dest = {
							.x = -screenWidth / 4,
							.y = -screenHeight / 4,
							.w = screenWidth * 3 / 2,
							.h = screenHeight * 3 / 2
						};

						SDL_RenderClear(window.renderer);
						SDL_RenderCopy(
							window.renderer, texture, NULL, &dest);
						SDL_RenderPresent(window.renderer);

						SDL_Delay(300);
						SDL_DestroyTexture(texture);

						guiWindowShowImage(&window, &state);
					} else {
						fprintf(stderr, "IMG_LoadTexture: %s\n", IMG_GetError());
					}

					break;
				}
			}
		}
	}
    // while (!quit) {
    //     while (1) {
	// 		//============ process sdl events ============
	// 		SDL_Event event = {0};
	// 		if (SDL_WaitEvent(&event) != 1) {
	// 			fprintf(stderr,"SDL_WaitEvent: %s\n",SDL_GetError());
	// 			return EXIT_FAILURE;
	// 		}
	// 		if (event.type == SDL_QUIT) {
	// 			quit = 1;
	// 		}
	// 		//============ should we jumpscare ============
	// 		else if (event.type == JUMPSCARE) {
	// 			// switch (event.user.code) {
	// 			// 	case JUMPSCARE_NORMAL:
	// 			// 		//normal
	// 			// 		break;
	// 			// 	case JUMPSCARE_EPILEPSY:
	// 			// 		//epilepsy
	// 			// 		break;
	// 			// 	case JUMPSCARE_FLASH:
	// 			// 		//flash
	// 			// 		break;
	// 			// 	case JUMPSCARE_ZOOM:
	// 			// 		//flash
	// 			// 		break;
	// 			// }
	// 			switch (event.user.code) {
	// 				case JUMPSCARE_NORMAL:
	// 					show_random_image(&state, &window, window.renderer);
	// 					play_audio(FOXY_JUMPSCARE, &AudioMixer);
	// 					break;

	// 				case JUMPSCARE_EPILEPSY:
	// 					show_random_image(&state, &window, window.renderer);
	// 					SDL_Delay(300);
	// 					show_random_image(&state, &window, window.renderer);
	// 					SDL_Delay(300);
	// 					show_random_image(&state, &window, window.renderer);
	// 					play_audio(SMILE_DOG_JUMPSCARE, &AudioMixer);
	// 					break;

	// 				case JUMPSCARE_FLASH:
	// 					SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
	// 					SDL_RenderClear(window.renderer);
	// 					SDL_RenderPresent(window.renderer);
	// 					SDL_Delay(100);

	// 					guiWindowShowImage(&window, &state);
	// 					break;

	// 				case JUMPSCARE_ZOOM: {
	// 					SDL_Texture *texture =
	// 						IMG_LoadTexture(window.renderer, images[rand() % IMAGE_COUNT]);

	// 					if (texture != NULL) {
	// 						int width, height;
	// 						int screenWidth, screenHeight;

	// 						SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	// 						SDL_GetRendererOutputSize(
	// 							window.renderer, &screenWidth, &screenHeight);

	// 						SDL_Rect dest = {
	// 							.x = -screenWidth / 4,
	// 							.y = -screenHeight / 4,
	// 							.w = screenWidth * 3 / 2,
	// 							.h = screenHeight * 3 / 2
	// 						};

	// 						SDL_RenderClear(window.renderer);
	// 						SDL_RenderCopy(window.renderer, texture, NULL, &dest);
	// 						SDL_RenderPresent(window.renderer);

	// 						SDL_Delay(300);
	// 						SDL_DestroyTexture(texture);

	// 						guiWindowShowImage(&window, &state);
	// 					} else {
	// 						fprintf(stderr, "IMG_LoadTexture: %s\n", IMG_GetError());
	// 					}
	// 					break;
	// 				}
	// 			}
	// 			SDL_PollEvent(&event); //advance queue
	// 		}
	// 		//============ read the directory to pick an image ============
	// 		int jumpscareProbability = 75; // 25;
	// 		bool isJumpscare = false; // flag condition for jumpscare
	// 		char currImg;
	// 		DIR *dir;
	// 		dirent *dp;
	// 		if ((dir = opendir(config.imageDir)) == NULL) {
	// 			perror("opendir(config.imageDir) == NULL\n"); //check if directory exists
	// 		}
	// 		while ((dir = opendir(config.imageDir)) != NULL) { //open directory stream
	// 			int index = 0;
	// 			while (index <= 1024) {
	// 				struct dirent *currFile = readdir(dir); //read directory stream, get pointer to image
	// 				// printf("got file success\n");
	// 				bool notImg = false;
	// 				if (currFile != NULL) { //not the end of the directory
	// 					int currChange = rand() % 100;
	// 					// printf("%d, %d\n", 
	// 					// 	strcmp(currFile->d_name, "."), 
	// 					// 	strcmp(currFile->d_name, ".."));

	// 					// if (strcmp(currFile->d_name, ".") == 0 || 
	// 					// 	strcmp(currFile->d_name, "..") == 0) {
	// 					// 	// printf("Not an image\n");
	// 					// 	notImg = true;
	// 					// }
	// 					if (!notImg) {
	// 						// printf("%s\n", currFile->d_name);
	// 						if ( currChange < jumpscareProbability // currChange <= jumpscareProbability
	// 							) {
	// 							isJumpscare = true;
	// 							SDL_Event jumpscareEvent = {0};
	// 							jumpscareEvent.type = JUMPSCARE;
	// 							//the GUI should activate jumpscare on this if-else condition
	// 							int typeProbability = rand() % 100;
	// 							if (typeProbability < config.chanceEpilepsy // typeProbability <= config.chanceEpilepsy
	// 								) {
	// 								jumpscareEvent.user.code = JUMPSCARE_EPILEPSY;
	// 								// printf("epilepsy\n");
	// 							} else if (typeProbability < config.chanceZoom // typeProbability <= config.chanceEpilepsy
	// 								) {
	// 								jumpscareEvent.user.code = JUMPSCARE_ZOOM;
	// 								// printf("zoom\n");
	// 							} else if (typeProbability < config.chanceFlash // typeProbability <= config.chanceEpilepsy
	// 								) {
	// 								jumpscareEvent.user.code = JUMPSCARE_FLASH;
	// 								// printf("flash\n");
	// 							} else {	
	// 								jumpscareEvent.user.code = JUMPSCARE_NORMAL;
	// 								// printf("normal :(\n");
	// 							}
	// 							SDL_PushEvent(&jumpscareEvent); //push jumpscare
	// 						}
	// 						index++; // index = (index + 1) % 1025; //increment photo index (with wrap around)
	// 					}
	// 				}

	// 				if (currFile == NULL) {
	// 					// printf("reset dir position back to beginning\n");
	// 					rewinddir(dir); //reset directory stream as we reached the end of the dir
	// 				}
	// 			}

	// 			closedir(dir);
	// 		}
    //     }
        
    // }
    
    clean_up(&AudioMixer);
    SDL_DestroyWindow(window.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
