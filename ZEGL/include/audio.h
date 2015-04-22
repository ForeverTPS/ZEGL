/*
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

#pragma once

#include <SDL_mixer.h>
#include <string>

/**
* Using a namespace rather than a static class or singleton etc to provide
* audio in the form of music and sound effects
*/
namespace Audio
{
	// MUSIC
	void AddMusic(std::string fileName, std::string key);

	void RemoveMusic(std::string key);

	void PlayMusic(std::string key, int numLoops = 0);

	void StopMusic();

	void PauseMusic();

	void ResumeMusic();

	void SetMusicVolume(int volume);

	// SFX
	void AddSoundEffect(std::string fileName, std::string key);

	void RemoveSoundEffect(std::string key);

	void PlaySoundEffect(std::string key, int numLoops = 0);

	void SetSoundEffectVolume(std::string key, int volume);

	void SetAllSoundEffectVolumes(int volume);

	// GENERAL
	void Init(int frequency = 44100, Uint16 format = AUDIO_S16SYS, int channels = 2, int chunksize = 1024);

	void Stop();

	void Pause();

	void Resume();

	void Clear();
}