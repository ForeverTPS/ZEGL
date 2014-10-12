/*  ____ ____ ____ ____
 * ||Z |||E |||G |||L ||
 * ||__|||__|||__|||__||
 * |/__\|/__\|/__\|/__\|
 *
 * Copyright(c) 2014, Phil Sampson
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

#include "entity.h"

class Light : public Entity
{
public:
	Light() :
		m_lightColor(Vector3f(1.0f, 1.0f, 1.0f)),
		m_lightIntensity(1.0f),
		m_ambientColor(Vector3f(0.2f, 0.2f, 0.2f)),
		m_ambientIntensity(0.2f),
		m_falloff(Vector3f(0.4f, 3.0f, 20.0f)),
		Entity(Vector3f(0.5f, 0.5f, 0.075f)) {}

	Light(const Vector3f& pos, const Vector3f& lightCol, float lightIntensity, const Vector3f& ambientCol, float ambientIntensity, const Vector3f& falloff) :
		m_lightColor(lightCol),
		m_lightIntensity(lightIntensity),
		m_ambientColor(ambientCol),
		m_ambientIntensity(ambientIntensity),
		m_falloff(falloff),
		Entity(pos) {}

	virtual ~Light() {}

	inline const Vector3f& GetLightColor()		const { return m_lightColor; }
	inline float GetLightIntensity()			const { return m_lightIntensity; }
	inline const Vector3f& GetAmbientColor()	const { return m_ambientColor; }
	inline float GetAmbientIntensity()			const { return m_ambientIntensity; }
	inline const Vector3f& GetFalloff()			const { return m_falloff; }
	
	inline void SetLightColor(const Vector3f& lightCol)		{ m_lightColor = lightCol; }
	inline void SetLightIntensity(float lightIntensity)		{ m_lightIntensity = lightIntensity; }
	inline void SetAmbientColor(const Vector3f& ambientCol)	{ m_ambientColor = ambientCol; }
	inline void SetAmbientIntensity(float ambientIntensity)	{ m_ambientIntensity = ambientIntensity; }
	inline void SetFalloff(const Vector3f& falloff)			{ m_falloff = falloff; }

private:
	Vector3f	m_lightColor;
	float		m_lightIntensity;
	Vector3f	m_ambientColor;
	float		m_ambientIntensity;
	Vector3f	m_falloff;
};