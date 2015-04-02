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

#define attribute in

attribute vec3 pos;
attribute float size;
attribute vec2 texCoord0;
attribute vec2 texCoord1;
attribute vec2 texCoord2;
attribute vec2 texCoord3;

out vec4 vCol;
out vec2 vTexCoord;

uniform mat4 MVP;

const vec2 quadpos[] = vec2[4] (
	vec2(-0.5,  0.5),
	vec2(0.5,   0.5),
	vec2(-0.5, -0.5),
	vec2(0.5,  -0.5)
);

void main() 
{ 
	vec2 offset = quadpos[gl_VertexID];

	if (gl_VertexID == 0)		vTexCoord = texCoord0;
	else if (gl_VertexID == 1)	vTexCoord = texCoord1;
	else if (gl_VertexID == 2)	vTexCoord = texCoord2;
	else						vTexCoord = texCoord3;
  
	gl_Position = MVP * vec4(pos.x + (offset.x * size),
							 pos.y + (offset.y * size), 
							 0, 
							 1);
}