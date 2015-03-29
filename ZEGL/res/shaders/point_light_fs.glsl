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

in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D u_diffuse;
uniform sampler2D u_normals;

uniform vec2 L_Resolution;
uniform vec3 L_LightPos;
uniform vec4 L_LightColor;
uniform vec4 L_AmbientColor;
uniform vec3 L_Falloff;

void main() 
{
	//RGBA of our diffuse color
	vec4 DiffuseColor = texture(u_diffuse, vTexCoord);

	//RGB of our normal map
	vec3 NormalMap = texture(u_normals, vTexCoord).rgb;
	
	//The delta position of light
	vec3 LightDir = vec3(L_LightPos.xy - (gl_FragCoord.xy / L_Resolution.xy), L_LightPos.z);
	
	//Correct for aspect ratio
	LightDir.x *= L_Resolution.x / L_Resolution.y;
	
	//Determine distance (used for attenuation) BEFORE we normalize our LightDir
	float D = length(LightDir);
	
	//Normalize vectors 
	vec3 N = normalize(NormalMap * 2.0 - 1.0);
	vec3 L = normalize(LightDir);
	
	//Pre-multiply light color with intensity
	//Then perform N dot L to determine diffuse term
	vec3 Diffuse = (L_LightColor.rgb * L_LightColor.a) * max(dot(N, L), 0.0);
	
	//Pre-multiply ambient color with intensity
	vec3 Ambient = L_AmbientColor.rgb * L_AmbientColor.a;
	
	//Calculate attenuation
	float Attenuation = 1.0 / ( L_Falloff.x + (L_Falloff.y*D) + (L_Falloff.z*D*D) );
	
	//The calculation which brings it all together
	vec3 Intensity = Ambient + Diffuse * Attenuation;
	vec3 FinalColor = DiffuseColor.rgb * Intensity;
	
	fragColor = vec4(FinalColor, DiffuseColor.a);
}