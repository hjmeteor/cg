#version 130

//You should not need to modify this file!
//Simply takes a 2d coordinate and generates pos in the range 0 to 1 to index into our texture for the image of the model on the screen

in vec2 position;

out vec2 pos;

void main() {
	vec4 v=vec4(position,0.0,1.0);
	pos=0.5*position+vec2(0.5,0.5);
	gl_Position=v;
}
