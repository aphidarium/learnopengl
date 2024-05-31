CC=g++
CFLAGS=-lGL -lGLU -lglfw -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -I include/ -o bin/out

all:
	$(CC) src/glad.c src/main.cpp $(CFLAGS)

run:
	./bin/out
