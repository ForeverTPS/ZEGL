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

#include <glm/glm.hpp>

namespace ZEGL
{
	class Window;

	/**
	* The camera is used for moving the display around the in game world. It uses
	* position, rotation and zoom to create an orthographic transformation matrix
	* which in turn is used for rendering.
	*/
	__declspec(align(16)) class Camera
	{
	public:
		/**
		* Constructor.
		*
		* The constructor uses the Window which is passed in to calculate
		* a transformation matrix.
		*
		* \param[in] window Pointer to a Window (defaults to nullptr)
		*
		* \warning If no Window is passed a default orthographic
		* projection is used. This is to prevent a crash but should not be used
		* for general purpose i.e. make sure you pass a Window in order to get
		* correct rendering
		*
		* \see [Window]
		*/
		Camera(const Window* window = nullptr);

		/**
		* Get the current transformation matrix.
		*
		* If the last position, rotation or zoom value has changed the Camera
		* uses the Window which is passed in to re-calculate a transformation matrix.
		*
		* \param[in] window Pointer to a Window (defaults to nullptr)
		*
		* \return glm::mat4 Orthographic transformation matrix
		*
		* \warning If no Window is passed a default orthographic
		* projection is used. This is to prevent a crash but should not be used
		* for general purpose i.e. make sure you pass a Window in order to get
		* correct rendering.
		*
		* \see [Window]
		*/
		const glm::mat4& GetTransform(const Window* window);

		/**
		* Get the current position of the camera.
		*
		* \return glm::vec3 representing the camera position
		*/
		inline glm::vec3 GetPos() const { return m_pos; }

		/**
		* Set the position of the camera using individual values.
		*
		* \param[in] x New X position component value
		* \param[in] y New Y position component value
		* \param[in] z New Z position component value
		*/
		inline void SetPos(float x, float y, float z = 0.0f) { m_pos.x = x; m_pos.y = y; m_pos.z = z; }

		/**
		* Set the position of the camera using an existing glm::vec3.
		*
		* \param[in] pos New position value
		*/
		inline void SetPos(glm::vec3 pos) { m_pos = pos; }

		/**
		* Get the current rotation of the camera.
		*
		* \return Camera rotation in radians
		*/
		inline float GetRot() const { return m_rot; }

		/**
		* Set the camera rotation.
		*
		* \param[in] rot Angle of rotation in radians
		*/
		inline void SetRot(float rot) { m_rot = rot; }

		/**
		* Get the current zoom factor of the camera.
		*
		* \return Zoom factor for the camera
		*/
		inline float GetZoom() const { return m_zoom; }

		/**
		* Set the camera zoom factor.
		*
		* \param[in] zoom Level of zoom factor (scaling) to apply
		*/
		inline void SetZoom(float zoom) { m_zoom = zoom; }

		void* operator new(size_t i)	{ return _mm_malloc(i, 16); }
		void  operator delete(void* p)	{ _mm_free(p); }

	protected:
	private:
		/**
		* Stores and updates the orthographic transformation.
		*
		* In addition to storing the orthographic transformation matrix, this struct 
		* stores the last known position, rotation and zoom values. This is to enable
		* the recreation of the transformation matrix to only be required if one of those
		* values is changed
		*/
		struct Transformation
		{
			glm::mat4	m_matrix;		/*!< The orthographic transformation matrix */
			glm::vec3	m_lastPos;		/*!< The last recorded position */
			float		m_lastRot;		/*!< The last recorded rotation (in radians) */
			float		m_lastZoom;		/*!< The last recorded zoom factor */

			/**
			* Updates the last recorded position, rotation and zoom values
			*
			* \param[in] pos The camera position
			* \param[in] rot The camera rotation (in radians)
			* \param[in] zoom The camera zoom factor
			*/
			void Update(const glm::vec3& pos, float rot, float zoom)
			{
				m_lastPos = pos; m_lastRot = rot; m_lastZoom = zoom;
			}
		};

		/**
		* Recalculate the transformation matrix.
		*
		* If the last position, rotation or zoom value has changed the Window which 
		* is passed in used to re-calculate a transformation matrix.
		*
		* \param[in] window Pointer to a Window (defaults to nullptr)
		*
		* \return Description
		*
		* \see [Window]
		*
		* \warning If no Window is passed a default orthographic
		* projection is used. This is to prevent a crash but should not be used
		* for general purpose i.e. make sure you pass a Window in order to get
		* correct rendering.
		*/
		void RecreateTransform(const Window* window);

		glm::vec3		m_pos;			/*!< The camera position */
		float			m_rot;			/*!< The camera rotation (in radians) */
		float			m_zoom;			/*!< The camera zoom factor */
		Transformation	m_transform;	/*!< The Transformation produced by the camera. */
	};
}