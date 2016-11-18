#version 130

//Inputs:
//Uniforms
//4 by 4 model transformation matrix
uniform mat4 model;
//4 by 4 view transformation matrix
uniform mat4 view;
//4 by 4 projection transformation matrix
uniform mat4 proj;
//4 by 4 normal vector transformation matrix
uniform mat4 normTrans;
//Homogeneous coordinate of eye/camera
uniform vec4 eyePos;
//Homogeneous coordinate of light source
uniform vec4 lightPos;

//Vertex attributes
//Vertex position
in vec3 position;
//Vertex normal
in vec3 normal;

//Outputs:
//Vector to light source
out vec4 lightVec;
//Vector to eye/camera
out vec4 eyeVec;
//Vector of transformed surface normal
out vec4 normOut;

/* Standard operations work on vectors, e.g.
	light + eye
	light - eye
   Single components can be accessed (light.x, light.y, light.z, light.w)
   C style program flow, e.g. for loops, if statements
   Can define new variables, vec2, vec3, vec4, float
   No recursion
   Example function calls you might find useful:
	normalize(x) - normalise a vector
*/



void main() {
	vec4 v=vec4(position,1.0);
	/*calculate transformed vertex position*/
	v = proj * view  * model * v;
	vec4 v2 = model * v;
	/*calculate transformed normal vector from input vector normal (store in normOut)*/
	vec4 n = vec4(normal, 1.);
	normOut = normTrans *n;
	/*calculate vector from transformed vertex to light source (store in lightVec)*/
	lightVec = normalize(lightPos - v2);
	/*calculate vector from transformed vertex to eye (store in eyeVec)*/
	eyeVec = normalize(eyePos - v2);
	/*output position of vertex on screen */
	gl_Position=v;
		
}

