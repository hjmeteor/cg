CC=g++
LIBS=-lGL -lGLU -lglfw3 -l:libGLEW.so.1.9 -lXinerama -lXcursor -lXi -lX11 -lXxf86vm -lXrandr
#INCLUDE=-I/usr/pkg/include
#LIBS = -framework OpenGL -framework CoreVideo -framework IOKit -framework GLUT -framework Cocoa -lglfw3 -lGLEW -L/usr/local/lib -L /usr/pkg/lib
#LIBS = -framework OpenGL -framework CoreVideo -framework IOKit -framework Cocoa -lglfw3 -lGLEW -L/usr/local/lib -L /usr/pkg/lib

all:
	$(CC) -w -o demo1 demo1.cpp $(LIBS) $(INCLUDE)

run: all
	./demo1 models/bunny.obj

clean: 
	rm -f demo1 *.o
