#include "audio.h"
#include <SDL2/SDL_mixer.h>


#define CAN_YOU_HEAR_THE_MUSIC "assets/can-you-hear-the-music/'Can you Hear the Music' - Oppenheimer.mp3"

void audio_mixer_int(AudioMixer_t* AudioMixer){
    AudioMixer->audio_rate = MIX_DEFAULT_FREQUENCY;
    AudioMixer->audio_format = MIX_DEFAULT_FORMAT;
    AudioMixer->audio_channels = MIX_DEFAULT_CHANNELS;
    AudioMixer->audio_buffers = 4096;
}

void play_audio(){

    AudioMixer_t AudioMixer;

    audio_mixer_int(&AudioMixer);

    /* Initialize the SDL library */
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s\n",SDL_GetError());
        return;
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("SDL2_mixer could not be initialized!\n"
                "SDL_Error: %s\n", SDL_GetError());
        return;
    }

    if (Mix_OpenAudio(AudioMixer.audio_rate, AudioMixer.audio_format, AudioMixer.audio_channels, AudioMixer.audio_buffers) < 0) {
        SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
        return;
    } else {
        Mix_QuerySpec(&AudioMixer.audio_rate, &AudioMixer.audio_format, &AudioMixer.audio_channels);
        SDL_Log("Opened audio at %d Hz %d bit%s %s %d bytes audio buffer\n", AudioMixer.audio_rate,
            (AudioMixer.audio_format&0xFF),
            (SDL_AUDIO_ISFLOAT(AudioMixer.audio_format) ? " (float)" : ""),
            (AudioMixer.audio_channels > 2) ? "surround" : (AudioMixer.audio_channels > 1) ? "stereo" : "mono",
            AudioMixer.audio_buffers);
    }

    AudioMixer.Mixer.audio_open = 1;

    Mix_VolumeMusic( AudioMixer.audio_volume);




}
