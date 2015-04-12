/**
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
attribute float rot;
attribute float xSize;
attribute float ySize;
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

	mat4 t1 = mat4(1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, 0,
				   pos.x, pos.y, 0, 1);

	mat4 r = mat4(cos(rot), sin(rot), 0, 0,
				 -sin(rot), cos(rot), 0, 0,
				         0,		   0, 1, 0,
					     0,		   0, 0, 1);

	mat4 t2 = mat4(1, 0, 0, 0,
			  	   0, 1, 0, 0,
				   0, 0, 1, 0,
				   -pos.x, -pos.y, 0, 1);

	mat4 model = t1 * r * t2;
  
	gl_Position = MVP * model * vec4(pos.x + (offset.x * xSize),
									 pos.y + (offset.y * ySize), 
									 0, 
									 1);
}