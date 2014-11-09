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
in vec2 vPositionUnits;

layout(location=0) out vec4 fragColor;

uniform float uConstant, uLinear, uQuadratic;
uniform float uRadius;

uniform vec3 uLightPos, uLightColor;

uniform sampler2D uNormal;

void main() 
{
    vec3 vp = vec3(gl_FragCoord.x, 600 - gl_FragCoord.y, 0);

    float distance = length(vec3(vp) - uLightPos);
    float dn = (distance / uRadius);

    float intensity = 1 / (uConstant + (dn * uLinear) + (dn * dn * uQuadratic));

    vec3 nm = texture2D(uNormal, vTexCoords, 0).rgb;

    vec3 n = normalize((2 * nm) - 1);
    vec3 l = normalize(uLightPos - vp);
    
    float diffuseStrength = max(0, dot(l, n));

    fragColor.rgb = uLightColor * diffuseStrength;
    fragColor.a = clamp(intensity, 0, 1);
}