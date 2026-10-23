#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>

typedef enum AudioMixerStatus {
    AUDIO_MIXER_FAILURE,
    AUDIO_MIXER_SUCCESS
} AudioMixerStatus;

typedef struct Mixer {
    Mix_Music* music;
    bool audio_open;
} Mixer;

typedef struct AudioMixer {
    int audioRate;
    Uint16 audioFormat;
    int audioChannels;
    int audioBuffers;
    int audioVolume;

    Mixer mixer;
} AudioMixer;

AudioMixerStatus audioMixerInit(AudioMixer* audioMixer);

void playAudio(const char* file, AudioMixer* audioMixer);
void cleanUp(AudioMixer* audioMixer);

#endif
