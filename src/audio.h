#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>

typedef struct Mixer {
    Mix_Music* music;
    bool audio_open;
    int next_track;
} Mixer_t;

typedef struct AudioMixer {
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int audio_buffers;
    int audio_volume;

    const char *typ;
    const char *tag_title;
    const char *tag_artist;
    const char *tag_album;
    const char *tag_copyright;

    double loop_start, loop_end, loop_length, current_position;

    Mixer_t Mixer;
} AudioMixer_t;

void audio_mixer_init(AudioMixer_t* AudioMixer);

void play_audio(const char* file, AudioMixer_t* AudioMixer);
void clean_up(AudioMixer_t* AudioMixer);

#endif