#pragma once

//These include basic math and vector support, as well as
//a helper library for OpenGL.
//For the Java developers amongst you, this is slightly similar
//to an import statement in Java.
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

//#include <GL/glut.h>
//The following three include is for running on xcode
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//this allows you to not have to write the std namespace before each
//function from the std library.
//for instance: std::vector becomes vector.
using namespace std; 
