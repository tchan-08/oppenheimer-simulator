#include "audio.h"

void audio_mixer_init(AudioMixer_t* AudioMixer){
    AudioMixer->audio_rate = MIX_DEFAULT_FREQUENCY;
    AudioMixer->audio_format = MIX_DEFAULT_FORMAT;
    AudioMixer->audio_channels = MIX_DEFAULT_CHANNELS;
    AudioMixer->audio_buffers = 4096;

    AudioMixer->tag_title = NULL;
    AudioMixer->tag_artist = NULL;
    AudioMixer->tag_album = NULL;
    AudioMixer->tag_copyright = NULL;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s\n",SDL_GetError());
        return;
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("SDL2_mixer could not be initialized!\n"
                "SDL_Error: %s\n", SDL_GetError());
        return;
    }

    if (Mix_OpenAudio(AudioMixer->audio_rate, AudioMixer->audio_format, AudioMixer->audio_channels, AudioMixer->audio_buffers) < 0) {
        SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
        return;
    } else {
        Mix_QuerySpec(&AudioMixer->audio_rate, &AudioMixer->audio_format, &AudioMixer->audio_channels);
        SDL_Log("Opened audio at %d Hz %d bit%s %s %d bytes audio buffer\n", AudioMixer->audio_rate,
            (AudioMixer->audio_format&0xFF),
            (SDL_AUDIO_ISFLOAT(AudioMixer->audio_format) ? " (float)" : ""),
            (AudioMixer->audio_channels > 2) ? "surround" : (AudioMixer->audio_channels > 1) ? "stereo" : "mono",
            AudioMixer->audio_buffers);
    }

    AudioMixer->Mixer.audio_open = 1;

    Mix_VolumeMusic( AudioMixer->audio_volume);
}


void play_audio(const char* file, AudioMixer_t* AudioMixer){

    audio_mixer_init(AudioMixer);

    Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));

    AudioMixer->Mixer.music = Mix_LoadMUS_RW(SDL_RWFromFile(file,"rb"), SDL_TRUE);

}

void clean_up(AudioMixer_t* AudioMixer)
{
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
    SDL_Quit();
}
