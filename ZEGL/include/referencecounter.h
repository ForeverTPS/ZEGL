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

namespace ZEGL
{
	/**
	* This class is to be inherited by any class which is intended to
	* use shared resources e.g. textures, shader etc. It performs reference
	* counting in order to allow the resource to know when the allocated
	* memory can be safely released i.e. when it is no longer in use.
	*/
	class ReferenceCounter
	{
	public:
		/**
		* The constructor initializes the reference count to 1
		*/
		ReferenceCounter() :
			m_refCount(1) {}

		virtual ~ReferenceCounter() {}

		/**
		* Increments the reference count by 1
		*/
		inline void AddReference() { m_refCount++; }

		/**
		* Decrements the reference count by 1
		*
		* \return True if the reference count is reduced to 0 otherwise false
		*/
		inline bool RemoveReference()	{ m_refCount--; return m_refCount == 0; }

	protected:
	private:
		ReferenceCounter(ReferenceCounter const&) = delete;
		ReferenceCounter& operator=(ReferenceCounter const&) = delete;

		int m_refCount;		/*!< The number of times the object is referenced */
	};
}
