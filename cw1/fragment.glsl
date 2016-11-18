#version 130

//Vector to light source
in vec4 lightVec;
//Vector to eye
in vec4 eyeVec;
//Transformed normal vector
in vec4 normOut;

//A texture you might want to use (optional)
uniform sampler2D tex;

//Output colour
out vec4 outColour;

/* Standard operations work on vectors, e.g.
	light + eye
	light - eye
   Single components can be accessed (light.x, light.y, light.z, light.w)
   C style program flow, e.g. for loops, if statements
   Can define new variables, vec2, vec3, vec4, float
   No recursion
   Example function calls you might find useful:
	max(x,y) - maximum of x and y
	dot(u,v) - dot product of u and v
	normalize(x) - normalise a vector
	pow(a,b) - raise a to power b
	texture(t,p) - read texture t at coordinates p (vec2 between 0 and 1)
	mix(a,b,c) - linear interpolation between a and b, by c. (You do not need to use this to interpolate vertex attributes, OpenGL will take care of interpolation between vertices before calling the fragment shader)
   outColour is a vec4 containing the RGBA colour as floating point values between 0 and 1. outColour.r, outColour.g, outColour.b and outColour.a can be used to access the components of a vec4 (as well as .x .y .z .w)
*/

void main() {	
	//Modify this code to calculate Phong illumination based on the inputs
	//
	//set values to the diffuse, specular and ambient reflectivity
	float diff=0.6;
	float spec=1.0;
	float ambient=0.6;
//	set a value to the light intensity
	float light_inten = 1;
//	set a value to the specular intensity
	float spec_inten = 80;
//  calculate the cos_theta where theta is the angle between the normal vector of the vertex and the direction to the light source
	float cos_theta = dot(lightVec, normOut);
//  calculate the cos_alpha where alpha is the angle between the reflected incoming light and the direction to the camera
	float cos_alpha = dot((2 * normOut * dot(lightVec,normOut) - lightVec), eyeVec);
//  calculate the diffuse reflection, specular reflection and the ambient
	float d = light_inten * diff * cos_theta;
	float s = light_inten * spec * pow(cos_alpha, spec_inten);
	float a = light_inten * ambient;
//  The values of cos_theta and cos_alpha should be set to 0 if they are negative
	d = max(d, 0);
	s = max(s, 0);
	a = max(a, 0);
//  output the color
	outColour = vec4(s + d + a, s + d + a, s + d + a, 1.0);
//  rendered into a texture
	outColour = texture(tex,vec2(normOut.x,normOut.y));
}

