#version 150

#define attribute in

attribute vec2 pos;

uniform mat4 MVP;

void main() 
{ 
	gl_Position = MVP * vec4(pos.x, pos.y, 0, 1); 
}