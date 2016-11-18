#ifndef _glinit_H
#define _glinit_H

//create window with glfw
GLFWwindow* initialiseGL()
{
	//initialise OpenGL/glfw
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL); // Windowed
	glfwMakeContextCurrent(window);

	//initialise glew (gets pointers to functions)
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		printf("Error: %s\n", glewGetErrorString(err));
	}

	//weird trick to make it run on OS X
	while (glGetError() != GL_NO_ERROR) {}

	return window;
}

//load shaders from files
GLuint loadVertexShaderGL(char *file)
{
	char slog[2048];
	int llen;
	GLuint shaderVert;
	GLint vsSize;
	GLchar *vsBuff;
	FILE *vsf;

	//read vertex shader code from file
	vsf = fopen(file, "r");
	fseek(vsf, 0, SEEK_END);
	vsSize = (int) ftell(vsf);
	fseek(vsf, 0, SEEK_SET);
	vsBuff = (GLchar*)malloc(vsSize);
	fread(vsBuff, vsSize, sizeof(char), vsf);
	fclose(vsf);

	//compile the vertex shader code
	shaderVert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderVert, 1, &vsBuff, &vsSize);
	glCompileShader(shaderVert);

	//print any errors
	glGetShaderInfoLog(shaderVert, 2048, &llen, slog);
	printf("Vertex shader compilation log: %s\n", slog);

	//free buffer for shader code
	free(vsBuff);

	//return handle of vertex shader
	return shaderVert;
}

GLuint loadFragmentShaderGL(char *file)
{
	char slog[2048];
	int llen;
	GLuint shaderFrag;
	GLint fsSize;
	GLchar *fsBuff;
	FILE *fsf;
	
	//read fragment shader code from file
	fsf = fopen(file, "r");
	fseek(fsf, 0, SEEK_END);
	fsSize = (int) ftell(fsf);
	fseek(fsf, 0, SEEK_SET);
	fsBuff = (GLchar*)malloc(fsSize);
	fread(fsBuff, fsSize, sizeof(char), fsf);
	fclose(fsf);

	//compile the fragment shader code
	shaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderFrag, 1, &fsBuff, &fsSize);
	glCompileShader(shaderFrag);

	//print any errors
	glGetShaderInfoLog(shaderFrag, 2048, &llen, slog);
	printf("Fragment shader compilation log: %s\n", slog);

	//free buffer for shader code
	free(fsBuff);

	//return handle of fragment shader
	return shaderFrag;
}

//link shaders into shader program
GLuint makeShaderProgramGL(GLuint shaderVert, GLuint shaderFrag)
{
	//create a shader program
	GLuint shader = glCreateProgram();

	//attach the vertex and fragment shaders
	glAttachShader(shader, shaderVert);
	glAttachShader(shader, shaderFrag);

	//bind the output colour
	glBindFragDataLocation(shader, 0, "outColour");

	//link the programs
	glLinkProgram(shader);

	return shader;
}

GLuint setupTextureGL(unsigned char *texdata, GLsizei w, GLsizei h, GLint internal)
{
	glActiveTexture(GL_TEXTURE0);
	
	//generate a texture and upload data
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);

	//setup filtering of texture (bilinear interpolation, see anti-aliasing lecture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//if index into texture is outside of 0-1, just return the colour from the closest pixel at the edge
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;
}

#endif //_glinit_H
