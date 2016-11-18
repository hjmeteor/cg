#include "demo1.h"
#include "geom.h"
#include "texture.h"
#include "glinit.h"

using namespace std;

//window resolution (default 640x480)
float windowX = 640.0f;
float windowY = 480.0f;

//The mesh global variable
TriangleMesh geom;
//OpenGL handles for object data
GLuint objectVertexBuffer;
GLuint objectNormBuffer;
GLuint objectUVbuffer;
GLuint objectVertexArray;
GLuint objectElementBuffer;
GLuint objectShaderProgram;
GLuint texture;
//Framebuffer to render object into
GLuint frameBuffer;
GLuint renderTarget;
GLuint depthStencil;
//OpenGL handles for postprocessing
GLuint screenVertexBuffer;
GLuint screenVertexArray;
GLuint screenElementBuffer;
GLuint screenShaderProgram;

//rotation angle global variables
float x_angle = 0.0f;
float y_angle = 0.0f;

//vertex and triangle data for a rectangle covering the whole screen
GLfloat screenVertex[] =
{ -1.0, -1.0,
  1.0, -1.0,
  -1.0, 1.0,
  1.0, 1.0 };

GLuint screenTri[] =
{ 0, 2, 1,
  1, 2, 3 };

void cleanup()
{
	//do any cleanup here
}

//The main display function.
void DemoDisplay()
{
	glDisable(GL_STENCIL_TEST);
	//setup for rendering
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer); //render to texture
	glUseProgram(objectShaderProgram);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//set the vertex attributes to vertex position and normal vector
	glBindVertexArray(objectVertexArray);
	GLuint pos = glGetAttribLocation(objectShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, objectVertexBuffer);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	GLuint norm = glGetAttribLocation(objectShaderProgram, "normal");
	glBindBuffer(GL_ARRAY_BUFFER, objectNormBuffer);
	glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectElementBuffer);
	glActiveTexture(GL_TEXTURE0);

	//texture...
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(objectShaderProgram, "tex"), 0);

	//camera
	glm::vec3 eye(0, 0, -5);
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 20.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	// Transformations to perform on model
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
	glm::mat4 translate_z = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
	glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), x_angle, glm::vec3(1, 0, 0));
	glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), y_angle, glm::vec3(0, 1, 0));
	glm::mat4 Model = rotate_x * rotate_y * translate_z * scale;

	//construct data to send to vertex shader
	glm::mat4 MN = glm::transpose(glm::inverse(Model));
	glm::vec4 lightPos(5, 5, -5, 1);
	glm::vec4 eyePos(eye, 1);

	//find locations of shader program variables
	GLuint SM = glGetUniformLocation(objectShaderProgram, "model");
	GLuint SMN = glGetUniformLocation(objectShaderProgram, "normTrans");
	GLuint SMV = glGetUniformLocation(objectShaderProgram, "view");
	GLuint SMVP = glGetUniformLocation(objectShaderProgram, "proj");
	GLuint SEyePos = glGetUniformLocation(objectShaderProgram, "eyePos");
	GLuint SLightPos = glGetUniformLocation(objectShaderProgram, "lightPos");

	//transfer data to shader program
	glUniformMatrix4fv(SM, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(SMN, 1, GL_FALSE, &MN[0][0]);
	glUniformMatrix4fv(SMV, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(SMVP, 1, GL_FALSE, &Projection[0][0]);
	glUniform4fv(SLightPos, 1, &lightPos[0]);
	glUniform4fv(SEyePos, 1, &eyePos[0]);

	//draw the elements (triangles)
	glDrawElements(GL_TRIANGLES, 3 * geom.TriangleCount(), GL_UNSIGNED_INT, (void*)0);

	//render postprocesseing
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //render to screen
	glUseProgram(screenShaderProgram);
	//no depth testing necessary
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//gives us access to two textures, the colour and the depth (z-buffer) information
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTarget);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthStencil);
	//bind the colour and depth textures to locations in the shader program
	glUniform1i(glGetUniformLocation(screenShaderProgram, "tex"), 0);
	glUniform1i(glGetUniformLocation(screenShaderProgram, "depth"), 1);

	//set the vertex attributes to vertex position and normal vector
	glBindVertexArray(screenVertexArray);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, screenVertexBuffer);
	GLuint posS = glGetAttribLocation(screenShaderProgram, "position");
	glVertexAttribPointer(posS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenElementBuffer);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)0);

	//rotate object
	x_angle = x_angle + 0.01;
	y_angle = y_angle + 0.0052;
}

//Program entry point.
//argc is a count of the number of arguments (including the filename of the program).
//argv is a pointer to each c-style string.
int main(int argc, char **argv)
{

	atexit(cleanup);
	cout << "Computer Graphics Assignment 1 Demo Program" << endl;

	if (argc >  1)
	{
		geom.LoadFile(argv[1]);
	}
	else
	{
		cerr << "Usage:" << endl;
		cerr << argv[0] << " <filename> " << endl;
		exit(1);
	}
	geom.CalculateNormals();

	//initialise OpenGL
	GLFWwindow* window = initialiseGL();

	//create framebuffers
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	renderTarget = setupTextureGL(NULL, 800, 600, GL_RGBA);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget, 0);
	//depth/stencil buffer
	glGenTextures(1, &depthStencil);
	glBindTexture(GL_TEXTURE_2D, depthStencil);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencil, 0);

	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%d\n", err);
	}
	//load and compile the shaders
	GLuint objectVertexShader = loadVertexShaderGL("vertex.glsl");
	GLuint objectFragmentShader = loadFragmentShaderGL("fragment.glsl");
	objectShaderProgram = makeShaderProgramGL(objectVertexShader, objectFragmentShader);

	GLuint screenVertexShader = loadVertexShaderGL("postvertex.glsl");
	GLuint screenFragmentShader = loadFragmentShaderGL("postfragment.glsl");
	screenShaderProgram = makeShaderProgramGL(screenVertexShader, screenFragmentShader);

	//transfer the geometry to the GPU
	glGenVertexArrays(1, &objectVertexArray);
	glBindVertexArray(objectVertexArray);
	//create buffers we will use
	glGenBuffers(1, &objectVertexBuffer);
	glGenBuffers(1, &objectElementBuffer);
	glGenBuffers(1, &objectNormBuffer);
	//get geometry vertices, normals and edge lists
	vector <glm::vec3> vert = geom.Vertices();
	vector <glm::vec3> norm = geom.Norms();
	//vector <glm::vec2> uv=geom.UVs();
	vector <Triangle> tri = geom.Triangles();
	//transfer to GPU
	//data for each vertex
	glBindBuffer(GL_ARRAY_BUFFER, objectVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vert.size(), &vert[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, objectNormBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * norm.size(), &norm[0], GL_STATIC_DRAW);
	//element array (triangles)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.size() * 3 * sizeof(unsigned int), &tri[0], GL_STATIC_DRAW);

	//setup for postprocessing
	glGenVertexArrays(1, &screenVertexArray);
	glBindVertexArray(screenVertexArray);
	//create buffers we will use
	glGenBuffers(1, &screenVertexBuffer);
	glGenBuffers(1, &screenElementBuffer);
	//data for each vertex
	glBindBuffer(GL_ARRAY_BUFFER, screenVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 4, &screenVertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(GLuint), &screenTri[0], GL_STATIC_DRAW);

	//load the texture
	readTexture();
	texture = setupTextureGL(tex, 256, 256, GL_RGB);


	//main loop
	while (!glfwWindowShouldClose(window))
	{
		DemoDisplay();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
