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

#define PI 3.14

in vec2 vTexCoords;

uniform sampler2D uDiffuse;
uniform vec2 uResolution;

//alpha threshold for our occlusion map
const float THRESHOLD = 0.75;
const float UPSCALE = 1.0;

void main() 
{
  float distance = 1.0;
  
  for (float y=0.0; y<uResolution.y; y+=1.0) 
  {
    	//rectangular to polar filter
		vec2 norm = vec2(vTexCoords.s, y/uResolution.y) * 2.0 - 1.0;
		float theta = PI*1.5 + norm.x * PI; 
		float r = (1.0 + norm.y) * 0.5;
		
		//coord which we will sample from occlude map
		vec2 coord = vec2(-r * sin(theta), -r * cos(theta))/2.0 + 0.5;
		
		//sample the occlusion map
		vec4 data = texture(uDiffuse, coord);
		
		//the current distance is how far from the top we've come
		float dst = y/uResolution.y / UPSCALE;
		
		//if we've hit an opaque fragment (occluder), then get new distance
		//if the new distance is below the current, then we'll use that for our ray
		float caster = data.a;
		if (caster > THRESHOLD) 
		{
			distance = min(distance, dst);
  		}
  }
   
  gl_FragColor = vec4(vec3(distance), 1.0);
}