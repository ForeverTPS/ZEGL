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

#pragma once

#include "sprite.h"
#include <SDL.h>

namespace ZEGL
{
	/**
	* The animated sprite is an extension of the sprite class allowing multiple
	* frames to be loaded in a texture atlas and then used to created an animation.
	*
	* \see [Sprite]
	*/
	class AnimatedSprite : public Sprite
	{
	public:
		/**
		* Constructor using a TextureAtlas.
		*
		* Used for an entity which has a texture containing multiple areas
		* that can be used for rendering different things.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] normalMap Corresponding normal map for the texture
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] frameNames List of each animation frame names (TextureRegion)
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] xScale Initial x scale of the entity
		* \param[in] yScale Initial y scale of the entity
		*
		* \see [Texture][TextureAtlas][TextureRegion]
		*/
		AnimatedSprite(const Texture& texture,
					   const Texture& normalMap,
					   const TextureAtlas& textureAtlas,
					   const std::vector<std::string>& frameNames,
					   const glm::vec3& pos = glm::vec3(0.0f),
					   float rot = 0.0f,
					   float xScale = -1.0f,
					   float yScale = -1.0f);

		/**
		* Constructor using a TextureAtlas but default normal map.
		*
		* Used for an entity which has a texture containing multiple areas
		* that can be used for rendering different things. This uses a default
		* normal map texture.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] frameNames List of each animation frame names (TextureRegion)
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] xScale Initial x scale of the entity
		* \param[in] yScale Initial y scale of the entity
		*
		* \see [Texture][TextureAtlas][TextureRegion]
		*/
		AnimatedSprite(const Texture& texture,
					   const TextureAtlas& textureAtlas,
					   const std::vector<std::string>& frameNames,
					   const glm::vec3& pos = glm::vec3(0.0f),
					   float rot = 0.0f,
					   float xScale = -1.0f,
					   float yScale = -1.0f);

		AnimatedSprite(AnimatedSprite const&);
		AnimatedSprite& operator=(AnimatedSprite const&) = delete;

		/**
		* Calculate the texture coordinates in the texture by using the
		* frame names provided at creation. This is to facilitate animation.
		*
		* \see [TextureRegion]
		*/
		void CalcTextureCoords();

		/**
		* Set the current animation frame.
		*
		* The animation frame is tracked and when changed, the base class (Entity)
		* has the texture coordinates updated from the locally stored ones.
		*
		* \param[in] frame Set the current animation frame
		*
		* \see [Entity]
		*/
		void SetFrame(unsigned int frame);

		/**
		* Reset the animation back to the beginning and optionally restart
		* the animation.
		*
		* \param[in] restart Whether to immediately start animating again
		*/
		void Reset(bool restart = false);

		/**
		* Start the animation at a given frame.
		*
		* \param[in] frame The animation frame to start at
		*/
		void Start(unsigned int frame = 0);

		/**
		* Pause the animation at the current frame.
		*/
		inline int	Pause() { m_isAnimating = false; }
		
		/**
		* Resume the animation from the last frame.
		*/
		inline void Resume() { m_isAnimating = true; }
		
		/**
		* Stop the animation and reset.
		*/
		inline void Stop() { Reset(); }

		/**
		* Set if the animation should loop or just stop when it reaches
		* the final frame.
		*
		* \param[in] loop Whether the animation should loop
		*/
		inline void SetLooping(bool loop) { m_loop = loop; }
		
		/**
		* Set the speed at which the sprite animates
		*
		* \param[in] speed Animation speed
		*
		* \warning The lower the value passed in the faster the animation
		*/
		inline void SetSpeed(Uint32 speed) { m_speed = speed; }

		/**
		* Essentially an update function which decides when to move on to
		* the next frame if not done so manually.
		*/
		void Animate();

	protected:
	private:
		struct QuadTexCoords
		{
			glm::vec2	topLeft;
			glm::vec2	topRight;
			glm::vec2	bottomLeft;
			glm::vec2	bottomRight;
		};

		unsigned int									m_currentFrame;
		std::unordered_map<std::string, unsigned int>	m_frameNames;
		std::vector<QuadTexCoords>						m_textureCoordinates;

		bool											m_isAnimating;
		bool											m_loop;

		Uint32											m_lastAnimated;
		Uint32											m_speed;
	};
}