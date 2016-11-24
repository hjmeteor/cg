Implementation:
1.Write Object subclass and intersection tests member function for spheres, planes, and triangles in Object.h and include these objects in the scene.
If you want to add these object to the scene you can create the objects in the putobjects() function in demo2.cpp. Also you can change the every parameter you want.

2.Implement the basic ray tracing algorithm in the CastRay function bysending a ray from the eye through all objects in the scene, up to arecursion depth of at least 3 in demo2.cpp by function CastRay(Ray &ray, Payload &payload).

3.Add direct illumination and shadows by sending rays to point lights in demo2.cpp by function CastRay(Ray &ray, Payload &payload) as well

4.Add specular reflections by sending reflected rays into the scene in demo2.cpp by function CastRay(Ray &ray, Payload &payload) as well


Instructions for compiling and running:
This demonstration program is written in C++. It needs to be compiledinto executable code before running.To compile the demo program, open up a terminal window and navigateto the directory containing the source code. Here you can run the command:
make run
This will compile the default source code and run the demo programwith the teapot mesh as input.