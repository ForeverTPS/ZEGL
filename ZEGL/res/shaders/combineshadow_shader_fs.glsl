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

#version 150 compatibility

in vec2 vTexCoords;

out vec4 fragColor;

uniform sampler2D uIntensity;
uniform sampler2D uMask;

void main() 
{
    //FIXME: Ambient component should be set as a uniform.
    
    fragColor = ((1 - texture(uMask, vTexCoords, 0).r) * texture2D(uIntensity, vTexCoords, 0));
}