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
uniform float uSoftShadows;
uniform vec4 uColor;

//sample from the distance map
float sample(vec2 coord, float r) 
{
  return step(r, texture(uDiffuse, coord).r);
}

void main() 
{
    //rectangular to polar
	vec2 norm = vTexCoords.st * 2.0 - 1.0;
	float theta = -atan(norm.y, norm.x);
	float r = length(norm);	
	float coord = (theta + PI) / (2.0*PI);
	
	//the tex coord to sample our 1D lookup texture	
	//always 0.0 on y axis
	vec2 tc = vec2(coord, 0.0);
	
	//the center tex coord, which gives us hard shadows
	float center = sample(vec2(tc.x, tc.y), r);        
	
	//we multiply the blur amount by our distance from center
	//this leads to more blurriness as the shadow "fades away"
	float blur = (1.0/uResolution.x) * smoothstep(0.0, 1.0, r); 
	
	//now we use a simple gaussian blur
	float sum = 0.0;
	
	sum += sample(vec2(tc.x - 4.0*blur, tc.y), r) * 0.05;
	sum += sample(vec2(tc.x - 3.0*blur, tc.y), r) * 0.09;
	sum += sample(vec2(tc.x - 2.0*blur, tc.y), r) * 0.12;
	sum += sample(vec2(tc.x - 1.0*blur, tc.y), r) * 0.15;
	
	sum += center * 0.16;
	
	sum += sample(vec2(tc.x + 1.0*blur, tc.y), r) * 0.15;
	sum += sample(vec2(tc.x + 2.0*blur, tc.y), r) * 0.12;
	sum += sample(vec2(tc.x + 3.0*blur, tc.y), r) * 0.09;
	sum += sample(vec2(tc.x + 4.0*blur, tc.y), r) * 0.05;
	
	//1.0 -> in light, 0.0 -> in shadow
 	float lit = mix(center, sum, uSoftShadows);
 	
 	//multiply the summed amount by our distance, which gives us a radial falloff
 	//then multiply by vertex (light) color  
 	gl_FragColor = uColor * vec4(vec3(1.0), lit * smoothstep(1.0, 0.0, r));
}