#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "gfc_audio.h"

void play_sound(char* filename, int channel);

void play_music(int whenToLoop);

void music_update();

#endif
