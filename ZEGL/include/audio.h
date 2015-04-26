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
	//////////////////////////////////////////////////////////////////////////
	// MUSIC
	//////////////////////////////////////////////////////////////////////////

	/**
	* Loads a new music track to the library.
	*
	* \param[in] filename Filename of the music file to load
	* \param[in] key String key name to assign to the music
	*/
	void AddMusic(std::string fileName, std::string key);

	/**
	* Removes a music track from the library.
	*
	* \param[in] key String key name of the music to remove
	*/
	void RemoveMusic(std::string key);

	/**
	* Plays a music track.
	*
	* \param[in] key Name of the track to play
	* \param[in] numLoops Number of times to play the music (0 = infinite)
	*/
	void PlayMusic(std::string key, int numLoops = 0);

	/**
	* Stop any music that is playing.
	*/
	void StopMusic();

	/**
	* Pause any music that is playing.
	*/
	void PauseMusic();

	/**
	* Resume any music that was playing.
	*/
	void ResumeMusic();

	/**
	* Set the volume of the music output.
	*
	* \param[in] volume The volume level to set the music to (range 0 - 100)
	*/
	void SetMusicVolume(int volume);

	//////////////////////////////////////////////////////////////////////////
	// SFX
	//////////////////////////////////////////////////////////////////////////

	/**
	* Loads a new sound effect to the library.
	*
	* \param[in] filename Filename of the sound effect file to load
	* \param[in] key String key name to assign to the sound effect
	*/
	void AddSoundEffect(std::string fileName, std::string key);

	/**
	* Removes a sound effect from the library.
	*
	* \param[in] key String key name of the sound effect to remove
	*/
	void RemoveSoundEffect(std::string key);

	/**
	* Plays a sound effect.
	*
	* \param[in] key Name of the effect to play
	* \param[in] numLoops Number of times to play the effect (0 = infinite)
	*/
	void PlaySoundEffect(std::string key, int numLoops = 0);

	/**
	* Set the volume of a specific sound effect.
	*
	* \param[in] key Name of the effect to change
	* \param[in] volume The volume level to set the effect to (range 0 - 100)
	*/
	void SetSoundEffectVolume(std::string key, int volume);

	/**
	* Set the volume of all sound effects.
	*
	* \param[in] volume The volume level to set the effects to (range 0 - 100)
	*/
	void SetAllSoundEffectVolumes(int volume);

	//////////////////////////////////////////////////////////////////////////
	// GENERAL
	//////////////////////////////////////////////////////////////////////////


	void Init(int frequency = 44100, Uint16 format = AUDIO_S16SYS, int channels = 2, int chunksize = 1024);

	/**
	* Stop all audio (music and sfx).
	*/
	void Stop();

	/**
	* Pause all audio (music and sfx).
	*/
	void Pause();

	/**
	* Resume all audio that was playing (music and sfx).
	*/
	void Resume();

	/**
	* Stops all audio and removes all music and effects from the library
	*/
	void Clear();
}