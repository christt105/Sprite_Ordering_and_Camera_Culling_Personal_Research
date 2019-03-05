#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
	
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	volumeMusic = config.child("volume").attribute("lvl").as_int(128);

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::list<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0F)
		{
			Mix_FadeOutMusic(int(fade_time * 150.0F));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}
	
	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0F)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0F)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = true;

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

// Fade out WAV
bool j1Audio::FadeOutFx(unsigned int id, int fade)
{
	bool ret = true;

	if (!active)
		return false;

	if (id > 0 && id <= fx.count())
	{
		Mix_FadeOutChannel(-1, fade);
	}

	return ret;
}


bool j1Audio::StopFx(unsigned int id)
{
	bool ret = true;

	if (!active)
		return false;

	if (id > 0 && id <= fx.count())
	{
		Mix_HaltChannel(-1);
	}
	return ret;
}


bool j1Audio::Save(pugi::xml_node& save) const{
	
	save.append_child("volume").append_attribute("lvl") = GetVolume();
	return true;
}

bool j1Audio::Load(pugi::xml_node& load) {

	volumeMusic = load.child("volume").attribute("lvl").as_int(MIX_MAX_VOLUME);
	return Mix_VolumeMusic(volumeMusic);
}

int j1Audio::SetVolume(float volume)
{
	return Mix_VolumeMusic(MIX_MAX_VOLUME*volume);
}

int j1Audio::SetFx(float volume)
{
	return Mix_Volume(-1,MIX_MAX_VOLUME*volume);
}

int j1Audio::GetVolume() const
{
	return Mix_VolumeMusic(-1);
}

//Set Volume
int j1Audio::RaiseVolume() {
	if (volumeMusic <= MIX_MAX_VOLUME)
		volumeMusic += 10;
	else { volumeMusic = MIX_MAX_VOLUME; }
	return Mix_VolumeMusic(volumeMusic);
}

int j1Audio::DecreaseVolume() {
	if (volumeMusic >= 10)
		volumeMusic -= 10;
	else { volumeMusic = 0; }
	return Mix_VolumeMusic(volumeMusic);
}