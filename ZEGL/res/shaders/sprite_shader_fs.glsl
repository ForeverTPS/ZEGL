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

#version 330

in vec2 vTexCoords;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalColor;

uniform vec4 uAmbient;
uniform sampler2D uDiffuse, uNormal;

void main() 
{
    normalColor = texture(uNormal, vTexCoords, 0);
    fragColor = texture(uDiffuse, vTexCoords, 0) * uAmbient;
}