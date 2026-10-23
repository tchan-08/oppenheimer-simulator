#include "audio.h"
#include <SDL2/SDL_mixer.h>

bool audioMixerInit(AudioMixer_t* AudioMixer) {
    AudioMixer->audio_rate = MIX_DEFAULT_FREQUENCY;
    AudioMixer->audio_format = MIX_DEFAULT_FORMAT;
    AudioMixer->audio_channels = MIX_DEFAULT_CHANNELS;
    AudioMixer->audio_volume = MIX_MAX_VOLUME;
    AudioMixer->audio_buffers = 4096;
    AudioMixer->Mixer.music = NULL;
    AudioMixer->Mixer.audio_open = false;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(
            AudioMixer->audio_rate,
            AudioMixer->audio_format,
            AudioMixer->audio_channels,
            AudioMixer->audio_buffers
        ) < 0) {
        SDL_Log("Couldn't open audio: %s", Mix_GetError());
        SDL_Quit();
        return false;
    }

    AudioMixer->Mixer.audio_open = true;
    Mix_VolumeMusic(AudioMixer->audio_volume);

    return true;
}

void cleanUp(AudioMixer_t* AudioMixer) {
    if(Mix_PlayingMusic()) {
        Mix_FadeOutMusic(1500);
        SDL_Delay(1500);
    }

    if (AudioMixer->Mixer.music) {
        Mix_FreeMusic(AudioMixer->Mixer.music);
        AudioMixer->Mixer.music = NULL;
    }

    if (AudioMixer->Mixer.audio_open) {
        Mix_CloseAudio();
        AudioMixer->Mixer.audio_open = 0;
    }
}

void playAudio(const char* file, AudioMixer_t* AudioMixer) {
    if (!audioMixerInit(AudioMixer)) {
        return;
    }

    AudioMixer->Mixer.music = Mix_LoadMUS(file);

    if (AudioMixer->Mixer.music == NULL) {
        SDL_Log("Couldn't load '%s': %s", file, Mix_GetError());
        cleanUp(AudioMixer);
        return;
    }

    if (Mix_PlayMusic(AudioMixer->Mixer.music, -1) < 0) {
        SDL_Log("Couldn't play music: %s", Mix_GetError());
        cleanUp(AudioMixer);
    }
}
