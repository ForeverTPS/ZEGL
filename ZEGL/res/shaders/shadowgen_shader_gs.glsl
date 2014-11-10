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

#version 400 

uniform vec3 uLightPos;
uniform mat3 uMVP;
uniform bool uUnmask;

layout(triangles) in;
layout(triangle_strip, max_vertices=15) out;

//For now we'll just assume framebuffer's 800x600...

//Projects the fragments onto the light radius' circle, and emits
//triangles to cover the occluded area.
//This is drawn on top of a circle-gradient texture framebuffer.
//(alpha: lightStrength->0, rgb: lightColor)

void main() 
{
    vec3 lp = vec3(uLightPos.x, uLightPos.y, 0);

    //FIXME: Something's going wrong with clip coord transform.
    //This should just be uMVP * lp
    vec2 l = (uMVP * lp).xy + vec2(-1, 1);

    vec2 v0 = gl_in[0].gl_Position.xy;
    vec2 v1 = gl_in[1].gl_Position.xy;
    vec2 v2 = gl_in[2].gl_Position.xy;
    vec2 v3 = v0 + (normalize(v0 - l) * vec2(800, 800));
    vec2 v4 = v1 + (normalize(v1 - l) * vec2(800, 800));
    vec2 v5 = v2 + (normalize(v2 - l) * vec2(800, 800));

    gl_Position = vec4(0,0,0,1);

    gl_Position.xy = v0; EmitVertex();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v2; EmitVertex();
    EndPrimitive();
    if (uUnmask) return;

    gl_Position.xy = v0; EmitVertex();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v3; EmitVertex();
    EndPrimitive();

    gl_Position.xy = v3; EmitVertex();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v4; EmitVertex();
    EndPrimitive();

    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v2; EmitVertex();
    gl_Position.xy = v4; EmitVertex();
    EndPrimitive();

    gl_Position.xy = v2; EmitVertex();
    gl_Position.xy = v4; EmitVertex();
    gl_Position.xy = v5; EmitVertex();
    EndPrimitive();
}