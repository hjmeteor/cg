CC=g++
LIBS= -lGLU -lGL -lglut

all:
	g++ -o demo2 *.cpp $(LIBS)

run: all
	./demo2 teapot.obj

clean: 
	rm -f demo2 *.o *~ core
