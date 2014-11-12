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

#version 150

in vec3 aVertex;

uniform vec3 uPos;
uniform vec2 uSize;
uniform mat4 uMVP;

out vec2 vTexCoords;

void main() 
{
    vTexCoords = aVertex.xy;
	gl_Position = uMVP * vec4(uSize.x * aVertex.x + uPos.x,
							  uSize.y * aVertex.y + uPos.y, 
							  uPos.z,
							  1);
}