#version 130

//Input position (coordinates within texture)
in vec2 pos;

//Texture maps for the image drawn on the screen, and the Z-buffer
uniform sampler2D tex;
uniform sampler2D depth;

//Final output colour
out vec4 outColour;

void main() {
	//Modify this code to read from the texture and add extra effects!
	//Read the colour from the texture and output it directly to the screen
	vec4 col=texture(tex,vec2(pos.x,pos.y));
	outColour=col;
}
