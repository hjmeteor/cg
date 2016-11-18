#include "demo2.h"
#include "Ray.h"
#include "Object.h"

//window resolution (default 640x480)
int windowX = 640;
int windowY = 480;

//Camera matrices
//For raytracer, only need camera transformation (View Matrix)
glm::mat4 viewMatrix; //The transformation matrix for the virtual camera

//An object in the scene
glm::vec3 c(0.0f);
float r = 1.0f;
Material::Material():
ambient(1.0f),//Default to white
diffuse(1.0f),//Default to white
specular(1.0f),//Default to white
specularExponent(10.0f)//Used in lighting equation
{
}

//Constructor
//@transform The transformation matrix for the object
//@material The material properties of the object
Object::Object(const glm::mat4 &transform,const Material &material):
_transform(transform),
_material(material)
{
}

Sphere* sphere = NULL;
Object* object = sphere;


//Perform any cleanup of resources here
void cleanup()
{
	if(object){
		delete object;
	}
}

//Forward declaration of functions, see below for more information
bool CheckIntersection(const Ray &ray, IntersectInfo &info);
float CastRay(Ray &ray, Payload &payload);

//Function for testing for intersection with all the objects in the scene
//If an object is hit then info contains the information on the intersection,
//returns true if an object is hit, false otherwise
bool CheckIntersection(const Ray &ray, IntersectInfo &info)
{
	// float b = 2 * (xd * (xs - xc));
	// float c = ray.origin * ray.origin - 1.1;
	// float disc = (b * b - 4 * c);
	// if(disc < 0)
	// 	return false;
	// else{
	// 	return true;
	// }
	//Test here for intersection of ray with nearest object
	return true;
}

//Recursive ray-casting function
//Called for each pixel and each time a ray is reflected/used for shadow testing
//@ray The ray we are casting
//@payload Information on the current ray i.e. the cumulative color and the number of bounces it has performed
//returns either the time of intersection with an object (the coefficient t in the equation: RayPosition = RayOrigin + t*RayDirection) or zero to indicate no intersection
float CastRay(Ray &ray, Payload &payload)
{
	//Perform early termination here (use number of bounces)
	//Check if the ray intersects something
	IntersectInfo info;
	if(CheckIntersection(ray,info)){
		return 1.0f;
	}
	return 0.0f;
}

/*--- Display Function ---*/
//The main display function.
//This allows you to draw pixels onto the display by using GL_POINTS.
//Drawn every time an update is required.
//Students: This is the main file you'll need to modify or replace.
//The idea with this example function is the following:
//1)Clear the screen so we can draw a new frame
//2)Cast a ray into the scene for each pixel on the screen and use the returned color to render the pixel
//3)Flush the pipeline so that the instructions we gave are performed.
void DemoDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear OpenGL Window

	//The window aspect ratio
	float aspectRatio = (float)windowX/(float)windowY;
	//The field of view of the camera.  This is 90 degrees because our imaginary image plane is 2 units high (-1->1) and 1 unit from the camera position
	float fov = 90.0f;
	//Value for adjusting the pixel position to account for the field of view
	float fovAdjust = tan(fov*0.5f *(M_PI/180.0f));

	//Tell OpenGL to start rendering points
	glBegin(GL_POINTS);
	//Iterate over each pixel in the image
	for(int column = 0; column < windowX; ++column){
		for(int row = 0; row < windowY; ++row){

			//Convert the pixel (Raster space coordinates: (0->ScreenWidth,0->ScreenHeight)) to NDC (Normalised Device Coordinates: (0->1,0->1))
			float pixelNormX = (column+0.5f)/windowX; //Add 0.5f to get centre of pixel
			float pixelNormY = (row+0.5f)/windowY;
			//Convert from NDC, (0->1,0->1), to Screen space (-1->1,-1->1).  These coordinates correspond to those used by OpenGL
			//Note coordinate (-1,1) in screen space corresponds to coordinate (0,0) in raster space i.e. column = 0, row = 0
			float pixelScreenX = 2.0f*pixelNormX - 1.0f;
			float pixelScreenY = 1.0f-2.0f*pixelNormY;

			//Account for Field of View
			float pixelCameraX = pixelScreenX * fovAdjust;
			float pixelCameraY = pixelScreenY * fovAdjust;

			//Account for image aspect ratio
			pixelCameraX *= aspectRatio;

			//Put pixel into camera space (offset by 1 unit along camera facing direction i.e. negative z axis)
			//vec4 so we can multiply with view matrix later
			glm::vec4 pixelCameraSpace(pixelCameraX,pixelCameraY,-1.0f,1.0f);

			glm::vec4 rayOrigin(0.0f,0.0f,0.0f,1.0f);//ray comes from camera origin

			//Transform from camera space to world space
			pixelCameraSpace = viewMatrix * pixelCameraSpace;
			rayOrigin = viewMatrix * rayOrigin;

			//Set up ray in world space
			Ray ray(glm::vec3(rayOrigin), //The origin of the ray we are casting
				glm::normalize(glm::vec3(pixelCameraSpace - rayOrigin))//The direction the ray is travelling in
			);

			//Structure for storing the information we get from casting the ray
			Payload payload;

			//Default color is white
			glm::vec3 color(1.0f);

			//Cast our ray into the scene
			if(CastRay(ray,payload) > 0.0f){// > 0.0f indicates an intersection
				color = ray.direction;
			}
			//Get OpenGL to render the pixel with the color from the ray
			glColor3f(color.x,color.y,color.z);

			glVertex3f(pixelScreenX,pixelScreenY,0.0f);
		}
	}
	glEnd();

	glFlush();// Output everything (write to the screen)
}


//This function is called when a (normal) key is pressed
//x and y give the mouse coordinates when a keyboard key is pressed
void DemoKeyboardHandler(unsigned char key, int x, int y)
{
	if(key == 'm')
	{
	cout << "Mouse location: " << x << " " << y << endl;
	}

	cout << "Key pressed: " << key << endl;

	glutPostRedisplay();

}

//Program entry point.
//argc is a count of the number of arguments (including the filename of the program).
//argv is a pointer to each c-style string.
int main(int argc, char **argv)
{

	atexit(cleanup);
    	cout << "Computer Graphics Assignment 2 Demo Program" << endl;

	//initialise OpenGL
	glutInit(&argc, argv);
	//Define the window size with the size specifed at the top of this file
	glutInitWindowSize(windowX, windowY);

	//Create the window for drawing with the title "SimpleExample"
	glutCreateWindow("CG-CW2");

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Set the function demoDisplay (defined above) as the function that
	//is called when the window must display
	glutDisplayFunc(DemoDisplay);// Callback function
    //similarly for keyboard input
    glutKeyboardFunc(DemoKeyboardHandler);

	//Set up our camera transformation matrices
	viewMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,200.0f));

	//Run the GLUT internal loop
	glutMainLoop();// Display everything and wait
}
