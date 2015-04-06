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

#include "referencecounter.h"
#include <string>
#include <unordered_map>

namespace ZEGL
{
	/**
	* Simple struct which uses four floating point values to define a region in
	* a texture
	*
	= \see [Texture]
	*/
	struct TextureRegion
	{
		float x;	/*!< X coordinate of the region in the texture */
		float y;	/*!< Y coordinate of the region in the texture */
		float w;	/*!< Width of the region in the texture */
		float h;	/*!< Height of the region in the texture */
	};

	/**
	* The texture atlas is used to provide functionality to have more than
	* one texture stored in a single file. The atlas defines one or more TextureRegion 
	* values in the large texture file. These are stored in a map by name.
	*
	* TextureAtlas data is stored in XML files. An example of one is included in the
	* engine called default_atlas.xml. It contains a single region which would be the
	* full size of a 512x512 texture.
	*/
	class TextureAtlas
	{
	public:
		/**
		* Load a texture atlas with the given file name.
		*
		* \param[in] fileName The name of the atlas to load
		*/
		TextureAtlas(const std::string& fileName = "./res/textures/default_atlas.xml");
		TextureAtlas(TextureAtlas const&);
		~TextureAtlas();

		/**
		* Get a named region from the atlas.
		*
		* \param[in] regionName Name of the region to find
		*
		* \return The requested texture region
		*
		* \see [TextureRegion]
		*/
		TextureRegion GetRegion(const std::string& regionName) const;

	protected:
	private:
		class TextureAtlasData : public ReferenceCounter
		{
		public:
			TextureAtlasData(const std::string& fileName);

			inline const std::unordered_map<std::string, TextureRegion>& GetRegions() const { return m_textureRegions; }

		protected:
		private:
			TextureAtlasData(TextureAtlasData const&) = delete;
			TextureAtlasData& operator=(TextureAtlasData const&) = delete;

			void ParseTextureAtlas(const std::string& fileName);

			std::unordered_map<std::string, TextureRegion> m_textureRegions;
		};

		TextureAtlas& operator=(TextureAtlas const&) = delete;

		static std::unordered_map<std::string, TextureAtlasData*> s_resourceMap;

		TextureAtlasData*	m_textureAtlasData;
		std::string			m_fileName;
	};
}