CC=g++
CFLAGS=-lGL -lGLU -lglfw -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -I include/ -o bin/out
FILES=src/glad.c src/main.cpp src/shader.h

c:
	$(CC) $(FILES) $(CFLAGS)

r:
	./bin/out
