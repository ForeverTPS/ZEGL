/**
* Copyright(c) 2014, Phil Sampson (http://www.zamma.co.uk)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "audio.h"
#include "logger.h"
#include "util.h"
#include <unordered_map>

namespace
{
	std::unordered_map<std::string, Mix_Music*> s_music;
	std::unordered_map<std::string, Mix_Chunk*> s_soundEffects;
}

namespace Audio
{
	//////////////////////////////////////////////////////////////////////////
	// MUSIC
	//////////////////////////////////////////////////////////////////////////

	void AddMusic(std::string fileName, std::string key)
	{
		std::unordered_map<std::string, Mix_Music*>::const_iterator it = s_music.find(key);
		if (it != s_music.end())
		{
			LOG_WARNING("Attempting to load music with already existing key: Current - " << it->first.c_str() << " New - " << fileName << " Key - " << key);
			return;
		}
		else
		{
			Mix_Music* music = Mix_LoadMUS(fileName.c_str());

			if (music == nullptr)
			{
				LOG_ERROR("Music couldn't be loaded! File: " << fileName << " Key: " << key << " Error: " << SDL_GetError());
				return;
			}

			s_music.insert(std::pair<std::string, Mix_Music*>(key, music));
		}
	}

	void RemoveMusic(std::string key)
	{
		std::unordered_map<std::string, Mix_Music*>::const_iterator it = s_music.find(key);
		if (it != s_music.end())
		{
			Mix_FreeMusic(it->second);
			s_music.erase(it);
		}
		else
		{
			LOG_WARNING("The music can't be removed because it doesn't exist! Key: " << key);
		}
	}
	
	void PlayMusic(std::string key, int numLoops)
	{
		std::unordered_map<std::string, Mix_Music*>::const_iterator it = s_music.find(key);
		if (it != s_music.end())
		{
			Mix_PlayMusic(it->second, numLoops);
		}
		else
		{
			LOG_WARNING("Music couldn't be played because it doesn't exist! Key: " << key);
		}
	}

	void StopMusic()
	{
		Mix_HaltMusic();
	}

	void PauseMusic()
	{
		Mix_PauseMusic();
	}

	void ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	void SetMusicVolume(int volume)
	{
		Mix_VolumeMusic((MIX_MAX_VOLUME / 100) * Util::Clamp(volume, 0, 100));
	}

	//////////////////////////////////////////////////////////////////////////
	// SFX
	//////////////////////////////////////////////////////////////////////////

	void AddSoundEffect(std::string fileName, std::string key)
	{
		std::unordered_map<std::string, Mix_Chunk*>::const_iterator it = s_soundEffects.find(key);
		if (it != s_soundEffects.end())
		{
			LOG_WARNING("Attempting to load sfx with already existing key: Current - " << it->first.c_str() << " New - " << fileName << " Key - " << key);
			return;
		}
		else
		{
			Mix_Chunk* sfx = Mix_LoadWAV(fileName.c_str());

			if (sfx == nullptr)
			{
				LOG_ERROR("Music couldn't be loaded! File: " << fileName << " Key: " << key << " Error: " << SDL_GetError());
				return;
			}

			s_soundEffects.insert(std::pair<std::string, Mix_Chunk*>(key, sfx));
		}
	}

	void RemoveSoundEffect(std::string key)
	{
		std::unordered_map<std::string, Mix_Chunk*>::const_iterator it = s_soundEffects.find(key);
		if (it != s_soundEffects.end())
		{
			Mix_FreeChunk(it->second);
			s_soundEffects.erase(it);
		}
		else
		{
			LOG_WARNING("The sfx can't be removed because it doesn't exist! Key: " << key);
		}
	}
	
	void PlaySoundEffect(std::string key, int numLoops)
	{
		std::unordered_map<std::string, Mix_Chunk*>::const_iterator it = s_soundEffects.find(key);
		if (it != s_soundEffects.end())
		{
			Mix_PlayChannel(-1, it->second, numLoops);
		}
		else
		{
			LOG_WARNING("Sfx couldn't be played because it doesn't exist! Key: " << key);
		}
	}

	void SetSoundEffectVolume(std::string key, int volume)
	{
		std::unordered_map<std::string, Mix_Chunk*>::const_iterator it = s_soundEffects.find(key);
		if (it != s_soundEffects.end())
		{
			Mix_VolumeChunk(it->second, (MIX_MAX_VOLUME / 100) * Util::Clamp(volume, 0, 100));
		}
		else
		{
			LOG_WARNING("Sfx volume couldn't be set because it doesn't exist! Key: " << key);
		}
	}
	
	void SetAllSoundEffectVolumes(int volume)
	{
		Mix_Volume(-1, (MIX_MAX_VOLUME / 100) * Util::Clamp(volume, 0, 100));
	}

	//////////////////////////////////////////////////////////////////////////
	// GENERAL
	//////////////////////////////////////////////////////////////////////////

	void Init(int frequency, Uint16 format, int channels, int chunksize)
	{
		if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0)
		{
			LOG_ERROR("Error initializing SDL_mixer: " << Mix_GetError());
		}
	}

	void Stop()
	{
		Mix_HaltChannel(-1);
	}

	void Pause()
	{
		Mix_Pause(-1);
	}

	void Resume()
	{
		Mix_Resume(-1);
	}

	void Clear()
	{
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		std::unordered_map<std::string, Mix_Music*>::iterator it_music = s_music.begin();
		while(it_music != s_music.end())
		{
			Mix_FreeMusic(it_music->second);
			it_music++;
		}
		s_music.clear();

		std::unordered_map<std::string, Mix_Chunk*>::iterator it_sfx = s_soundEffects.begin();
		while (it_sfx != s_soundEffects.end())
		{
			Mix_FreeChunk(it_sfx->second);
			it_sfx++;
		}
		s_soundEffects.clear();
	}
}