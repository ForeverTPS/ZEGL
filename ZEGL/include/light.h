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

#ifndef LIGHT_H
#define LIGHT_H

class Light
{
public:
	Light() :
		m_pos(glm::vec3(0.0f)),
		m_color(glm::vec3(1.0f)),
		m_radius(100.0f),
		m_falloff(glm::vec3(0.4f, 3.0f, 20.0f)) {}

	Light(const glm::vec3& pos, const glm::vec3& color, float radius, const glm::vec3& falloff) :
		m_pos(pos),
		m_color(color),
		m_radius(radius),
		m_falloff(falloff) {}

	virtual ~Light() {}

	float GetIntensityAt(const glm::vec2& other) const
	{
		glm::vec2 mp(m_pos.x, m_pos.y);
		float distance = glm::distance(mp, other);
		float nd = glm::min(distance / m_radius, 1.0f);

		return 1.0f / (m_falloff.x + (nd * m_falloff.y) + (nd * nd * m_falloff.z));
	}

	inline const glm::vec3	GetPos()		const { return m_pos; }
	inline const glm::vec3& GetColor()		const { return m_color; }
	inline const float GetRadius()			const { return m_radius; }
	inline const glm::vec3& GetFalloff()	const { return m_falloff; }
	
	inline void SetPos(const glm::vec3& pos)			{ m_pos = pos; }
	inline void SetLightColor(const glm::vec3& color)	{ m_color = color; }
	inline void SetRadius(float radius)					{ m_radius = radius; }
	inline void SetFalloff(const glm::vec3& falloff)	{ m_falloff = falloff; }

private:
	glm::vec3	m_pos;
	glm::vec3	m_color;
	float		m_radius;
	glm::vec3	m_falloff;		// constant | linear | quadratic
};

#endif