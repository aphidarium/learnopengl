CC=g++
CFLAGS=-lGL -lGLU -lglfw -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -lassimp -I include/ -o bin/out
FILES=src/glad.c src/main.cpp src/shader.h src/mesh.h src/model.h
DIVIDER="-------------------------- <<[[ COMPILING ]]>> --------------------------"

c:
	@echo $(DIVIDER); $(CC) $(FILES) $(CFLAGS)

r:
	@./bin/out
