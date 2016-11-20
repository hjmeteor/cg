#include "demo2.h"
#include "Ray.h"
#include "Object.h"

//What I implement
//The RECURSION_DEPTHE is that a ray from the eye through all objects in the scene, up to
//a recursion depth of at least 3.

//The INFINITY1 is used to initialize the nearest intersection distance, when
//comparing the intersection of multiple objects at the same time which one is
//nearer
#define RECURSION_DEPTHE 3
#define INFINITY1 1e8

//window resolution (default 640x480)
int windowX = 640;
int windowY = 480;

//Camera matrices
//For raytracer, only need camera transformation (View Matrix)
glm::mat4 viewMatrix; //The transformation matrix for the virtual camera

//An object in the scene
Object* object = NULL;

//What I implement
//Define a triangel, a plane and two sphere in the scene
glm::vec3 a(200.0f,50.0f,0.0f);
glm::vec3 b(102.0f,-20.0f,0.0f);
glm::vec3 c(250.0f,-40.0f,0.0f);
Triangle t(a, b, c, glm::vec3(0.9f,0.0f,0.0f));


glm::vec3 p0(0.0f, -400.0f, 0.0f );
glm::vec3 n(0.0f, -1.0f, 0.0f);
Plane p(p0, n, glm::vec3(0.752f,0.752f,0.752f));


glm::vec3 center(-50.0f, 0.0f, 0.0f);
float r = 40.0;
Sphere s(center, r, glm::vec3(0.0f,0.0f,1.0f));

glm::vec3 center2(50.0f, 0.0f, 0.0f);
float r2 = 50.0;
Sphere s2(center2, r2, glm::vec3(0.0f,1.0f,0.0f));

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

//What I implement
bool CheckIntersection(const Ray &ray, IntersectInfo &info)
{
	// Test here for intersection of ray with nearest object
	float tNear = INFINITY1;
	// Used to record the nearest object's information of intersection
	IntersectInfo near_info;
	bool is_intersect = false;
	if(p.Intersect(ray, info) && info.time < tNear && info.time > 0) {
		near_info = info;
		tNear = near_info.time;
		is_intersect = true;
	}
	if(t.Intersect(ray, info) && info.time < tNear && info.time > 0) {
		near_info = info;
		tNear = near_info.time;
		is_intersect = true;
	}
	if(s.Intersect(ray, info) && info.time < tNear && info.time > 0) {
		near_info = info;
		tNear = near_info.time;
		is_intersect = true;
	}
	if(s2.Intersect(ray, info) && info.time < tNear && info.time > 0) {
		near_info = info;
		tNear = near_info.time;
		is_intersect = true;
	}
	if(is_intersect == true){
		info = near_info;
		return true;
	}
	else
		return false;
}

//Recursive ray-casting function
//Called for each pixel and each time a ray is reflected/used for shadow testing
//@ray The ray we are casting
//@payload Information on the current ray i.e. the cumulative color and the number of bounces it has performed
//returns either the time of intersection with an object (the coefficient t in the equation: RayPosition = RayOrigin + t*RayDirection) or zero to indicate no intersection

//What I implement

float CastRay(Ray &ray, Payload &payload)
{
	//Perform early termination here (use number of bounces)
	//Check if the ray intersects something

	IntersectInfo info;
	//Default light position
	glm::vec3 lightPos(-60.0f, 90.0f, 100.0f);
	//	set a value to the light intensity
	float light_inten = 1.0f;
	if(CheckIntersection(ray,info)){
		glm::vec3 obj_color = info.color;
		float ambient = info.material -> ambient.x;
		float a = max(light_inten * ambient, 0.0f);
		payload.color = payload.color + a * obj_color;

		// comput shadow(for each light, but here i use point light)
		Ray shadowRay(glm::vec3(info.hitPoint + 0.001f), 
				glm::normalize(glm::vec3(lightPos - info.hitPoint))
		);
		if(CheckIntersection(shadowRay, info)){
			// hit point is in shadow so just return
			glm::vec3 color(0.0f);
			payload.color = color;
			return 1.0f;
		}
		else{
			glm::vec3 norm = info.normal;
			glm::vec3 lightVec = lightPos - info.hitPoint;
			float diff = info.material -> diffuse.x;
			float spec = info.material -> specular.x;

			float specularExponent = info.material -> specularExponent;
		//  calculate the cos_theta where theta is the angle between the normal vector of the vertex and the direction to the light source
			float cos_theta = glm::dot(glm::normalize(lightPos), norm);
		//  calculate the cos_alpha where alpha is the angle between the reflected incoming light and the direction to the camera
			// float cos_alpha = glm::dot(2 * glm::dot(lightPos, norm) * norm - lightPos, ray.direction);
			// using half-vector h to calculate specular coefficient. This way is faster than normal one.
			glm::vec3 h = glm::normalize(lightPos + ray.direction);
		//  calculate the diffuse reflection, specular reflection and the ambient
			float d = max(light_inten * diff * cos_theta, 0.0f);
			// float sp = fabs(light_inten * spec * pow(cos_alpha, spec_inten));
			// float sp;

			float sp = max(light_inten * spec * pow(max(glm::dot(norm, h),0.0f), specularExponent), 0.0f);
		//  The values of cos_theta and cos_alpha should be set to 0 if they are negative
			glm::vec3 color = obj_color * (d + sp);
			payload.color = payload.color + color;
		}
		if(payload.numBounces < RECURSION_DEPTHE){
			glm::vec3 lightVec = lightPos - info.hitPoint;
			float reflection_index = info.material -> reflection_index;
			if(reflection_index > 0.0f){
				glm::vec3 norm = info.normal;
				glm::vec3 lightVec = lightPos - info.hitPoint;
				Ray reflectionRay(glm::vec3(info.hitPoint), 
						glm::normalize(glm::reflect(lightVec, norm))
				);
				payload.numBounces += 1;
				CastRay(reflectionRay, payload);
				payload.color = payload.color + reflection_index * payload.color;
			}
			float refraction_index = info.material -> refraction_index;
			if(refraction_index > 0.0f){
				glm::vec3 norm = info.normal;
				glm::vec3 lightVec = lightPos - info.hitPoint;
				
				Ray refractionRay(glm::vec3(info.hitPoint), 
						glm::normalize(glm::refract(lightVec, norm, refraction_index))
				);
				payload.numBounces += 1;
				CastRay(refractionRay, payload);
				payload.color = payload.color + refraction_index * payload.color;
			}
		}
	}
	return 1.0f;
}

// float CastRay(Ray &ray, Payload &payload)
// {
// 	//Perform early termination here (use number of bounces)
// 	//Check if the ray intersects something
//
// 	IntersectInfo info;
// 	if(CheckIntersection(ray,info)){
// 		glm::vec3 norm = info.normal;
// 		glm::vec3 lightVec = lightPos - info.hitPoint;
// 		float diff = info.material -> diffuse.x;
// 		float spec = info.material -> specular.x;
// 		float ambient = info.material -> ambient.x;
// 		float specularExponent = info.material -> specularExponent;
// 	//  calculate the cos_theta where theta is the angle between the normal vector of the vertex and the direction to the light source
// 		float cos_theta = glm::dot(glm::normalize(lightPos), norm);
// 	//  calculate the cos_alpha where alpha is the angle between the reflected incoming light and the direction to the camera
// 		// float cos_alpha = glm::dot(2 * glm::dot(lightPos, norm) * norm - lightPos, ray.direction);
// 		// using half-vector h to calculate specular coefficient. This way is faster than normal one.
// 		glm::vec3 h = glm::normalize(lightPos + ray.direction);
// 	//  calculate the diffuse reflection, specular reflection and the ambient
// 		float d = light_inten * diff * cos_theta;
// 		// float sp = fabs(light_inten * spec * pow(cos_alpha, spec_inten));
// 		// float sp;
//
// 		float sp = light_inten * spec * pow(max(glm::dot(norm, h),0.0f), specularExponent);
//
// 		float a = light_inten * ambient;
// 		d = max(d, 0.0f);
// 		s = max(s, 0.0f);
// 		a = max(a, 0.0f);
//
// 	//  The values of cos_theta and cos_alpha should be set to 0 if they are negative
// 		glm::vec3 color((a + d + sp) * 0.0f, (a + d + sp) * 0.0f, (a + d + sp) * 1.0f);
// 		payload.color = payload.color + color;
//
// 		//comput shadow
// 		Ray shadowRay;
// 		shadowRay.direction = lightPos - info.hitPoint;
// 		shadowRay.origin = info.hitPoint;
// 		bool is_shadow = false;
// 		if(CheckIntersection(shadowRay, info)){
// 			// hit point is in shadow so just return
// 			glm::vec3 color(0.0f);
// 			payload.color = color;
// 			return 1.0f;
// 		}
// 	}
//
// 	if(payload.numBounces < RECURSION_DEPTHE){
// 		float refraction_index = info.material -> refraction_index;
// 		glm::vec3 norm = info.normal;
// 		glm::vec3 lightVec = lightPos - info.hitPoint;
// 		Ray reflectionRay;
// 		Ray refractionRay;
// 		reflectionRay.origin = info.hitPoint;
// 		reflectionRay.direction = glm::reflect(lightPos, norm);
// 		refractionRay.origin = info.hitPoint;
// 		refractionRay.direction = glm::refract(lightPos, norm, refraction_index);
// 		float reflectionColor, refractionColor;
// 		CastRay(reflectionRay, payload.numBounces + 1);
// 		reflectionColor = payload.color.x;
// 		CastRay(refractionColor, payload.numBounces + 1);
// 		refractionColor = payload.color.y;
// 		float kr, kt;
// 		fresnel(
//             object->indexOfRefraction,
//             nHit,
//             ray.direction,
//             &Kr,
//             &Kt);
// 		payload.color(reflectionColor * Kr + refractionColor * (1 - Kr));
//
// 	}
// 	else
// 	return 1.0f;
// }

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
			//Payload record the final color after cast ray
			if(CastRay(ray,payload) > 0.0f){// > 0.0f indicates an intersection
				color = payload.color;
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
