#include "audio.h"
#include <SDL2/SDL_mixer.h>

AudioMixerStatus audioMixerInit(AudioMixer* audioMixer) {
    audioMixer->audioRate = MIX_DEFAULT_FREQUENCY;
    audioMixer->audioFormat = MIX_DEFAULT_FORMAT;
    audioMixer->audioChannels = MIX_DEFAULT_CHANNELS;
    audioMixer->audioVolume = MIX_MAX_VOLUME;
    audioMixer->audioBuffers = 4096;
    audioMixer->mixer.music = NULL;
    audioMixer->mixer.audio_open = false;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return AUDIO_MIXER_FAILURE;
    }

    if (Mix_OpenAudio(
            audioMixer->audioRate,
            audioMixer->audioFormat,
            audioMixer->audioChannels,
            audioMixer->audioBuffers
        ) < 0) {
        SDL_Log("Couldn't open audio: %s", Mix_GetError());
        return AUDIO_MIXER_FAILURE;
    }

    audioMixer->mixer.audio_open = true;
    Mix_VolumeMusic(audioMixer->audioVolume);

    return AUDIO_MIXER_SUCCESS;
}

void cleanUp(AudioMixer* audioMixer) {
    if(Mix_PlayingMusic()) {
        Mix_FadeOutMusic(1500);
        SDL_Delay(1500);
    }

    if (audioMixer->mixer.music) {
        Mix_FreeMusic(audioMixer->mixer.music);
        audioMixer->mixer.music = NULL;
    }

    if (audioMixer->mixer.audio_open) {
        Mix_CloseAudio();
        audioMixer->mixer.audio_open = 0;
    }
}

void playAudio(const char* file, AudioMixer* audioMixer) {
    if (!audioMixerInit(audioMixer)) {
        return;
    }

    audioMixer->mixer.music = Mix_LoadMUS(file);

    if (audioMixer->mixer.music == NULL) {
        SDL_Log("Couldn't load '%s': %s", file, Mix_GetError());
        cleanUp(audioMixer);
        return;
    }

    if (Mix_PlayMusic(audioMixer->mixer.music, -1) < 0) {
        SDL_Log("Couldn't play music: %s", Mix_GetError());
        cleanUp(audioMixer);
    }
}
