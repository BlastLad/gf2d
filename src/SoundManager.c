#include "AudioManager.h"
#include "simple_logger.h"



static Sound* mainTheme;

static float musicLoopVal;

static int loopTicks;




void play_sound(char* filename, int channel) 
{
	Sound* failTune = gfc_sound_load(filename, 1, 2);
	if (channel == 2) 
	{
		gfc_sound_play(failTune, 0, 3, channel, -1);

	}
	else 
	{
		gfc_sound_play(failTune, 0, 1, channel, -1);
	}
	gfc_sound_free(failTune);
}

void play_music(int whenToLoop) 
{
	gfc_sound_free(mainTheme);
	
	mainTheme = gfc_sound_load("audio/bewitched.wav", 0.7, 0);	

	if (!mainTheme) 
	{
		slog("Warning audio file not found");
		return;
	}
	musicLoopVal = whenToLoop * 1000;
	loopTicks = SDL_GetTicks64() + musicLoopVal;

	gfc_sound_play(mainTheme, 0, 0.7, 0, -1);
	

}


void music_update() 
{
	if (!mainTheme) 
	{
		slog("No Music Currently playing");
		return;
	}

	if (SDL_GetTicks64() > loopTicks) 
	{
		loopTicks = SDL_GetTicks64() + musicLoopVal;	
		gfc_sound_play(mainTheme, 0, 0.7, 0, -1);
	}
}
