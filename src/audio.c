#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>


#define WAVES_SOUND "assets/waves-at-baltic-sea-shore/waves-at-baltic-sea-shore.wav"

#define CLAP_SOUND              "assets/claps-and-snares/clap.ogg"
#define SNARE_SOUND             "assets/claps-and-snares/snare.ogg"
#define TECHNO_CLAP_SNARE_SOUND "assets/claps-and-snares/techno-clap-snare.ogg"
#define REVERB_SNARE_SOUND      "assets/claps-and-snares/dubstep-reverb-snare.ogg"

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        printf("SDL can not disable compositor bypass!\n");
        return 0;
    }
#endif

//Initialize SDL2_mixer
if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
{
    printf("SDL2_mixer could not be initialized!\n"
            "SDL_Error: %s\n", SDL_GetError());
    return 0;
}

if (!SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS,"1")){
		sdlerror("SDL_SetHint");
		return 1;
	}
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0){
		sdlerror("SDL_Init");
		return 1;
	}
	if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 4096) != 0){
		sdlerror("Mix_OpenAudio");
		return 1;
	}


            // Load .WAV sound
            Mix_Chunk *waves = Mix_LoadWAV(WAVES_SOUND);
            if(!waves)
            {
                printf(".WAV sound '%s' could not be loaded!\n"
                       "SDL_Error: %s\n", WAVES_SOUND, SDL_GetError());
                return 0;
            }

            // Load .OGG sounds
            Mix_Music *clapnsnare1 = Mix_LoadMUS(CLAP_SOUND);
            Mix_Music *clapnsnare2 = Mix_LoadMUS(SNARE_SOUND);
            Mix_Music *clapnsnare3 = Mix_LoadMUS(TECHNO_CLAP_SNARE_SOUND);
            Mix_Music *clapnsnare4 = Mix_LoadMUS(REVERB_SNARE_SOUND);
            if(!clapnsnare1 || !clapnsnare2 || !clapnsnare3 || !clapnsnare4)
            {
                printf("One of the .OGG sounds could not be loaded!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                return 0;
            }

            // Play waves sound
            if(Mix_PlayChannel(-1, waves, 0) == -1)
            {
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(waves);
                return 0;
            }

            // Event loop exit flag
            bool quit = false;

            // Event loop
            while(!quit)
            {
                SDL_Event e;

                // Wait indefinitely for the next available event
                SDL_WaitEvent(&e);

                // User requests quit
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        if(Mix_Paused(-1))
                            Mix_Resume(-1);
                        else
                            Mix_Pause(-1);
                        break;

                    case SDLK_RIGHT:
                        if(Mix_PlayMusic(clapnsnare1, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;

                    case SDLK_LEFT:
                        if(Mix_PlayMusic(clapnsnare2, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;

                    case SDLK_UP:
                        if(Mix_PlayMusic(clapnsnare3, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;

                    case SDLK_DOWN:
                        if(Mix_PlayMusic(clapnsnare4, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;
                    }
                }

                Mix_FreeChunk(waves);
                Mix_FreeMusic(clapnsnare1);
                Mix_FreeMusic(clapnsnare2);
                Mix_FreeMusic(clapnsnare3);
                Mix_FreeMusic(clapnsnare4);


            }
    }

    // Quit SDL2_mixer
    Mix_CloseAudio();
