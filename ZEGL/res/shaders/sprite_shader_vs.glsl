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

in vec3 aPos;
in float aSize;
in vec2 aTexCoord0;
in vec2 aTexCoord1;
in vec2 aTexCoord2;
in vec2 aTexCoord3;

out vec2 vTexCoord;
out vec2 vPositionUnits;

uniform vec2 uCameraPos;
uniform mat3 uMVP;

const vec2 quadpos[] = vec2[4] (
  vec2(-0.5,  0.5),
  vec2( 0.5,  0.5),
  vec2(-0.5, -0.5),
  vec2( 0.5, -0.5)
);

void main() 
{ 
	vec2 offset = quadpos[gl_VertexID];

	if (gl_VertexID == 0)		vTexCoord = aTexCoord0;
	else if (gl_VertexID == 1)	vTexCoord = aTexCoord1;
	else if (gl_VertexID == 2)	vTexCoord = aTexCoord2;
	else						vTexCoord = aTexCoord3;

	vec3 pxPos = vec3(aPos.x + (offset.x * aSize),
					  aPos.y + (offset.y * aSize), 
					  aPos.z);

	vPositionUnits = pxPos.xy;
  
	gl_Position = vec4((uMVP * pxPos).xy, aPos.z, 1);
}