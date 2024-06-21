CC=g++

PROGRAMME_NAME=out

LIBS=-lGL -lGLU -lglfw -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -lassimp -I include/ -I src/ -o bin/out
INCLUDES= -I include/ -I src/

CFLAGS=$(LIBS) $(INCLUDES) -o bin/$(PROGRAMME_NAME)

FILES=src/glad.c src/main.cpp src/shader.h src/mesh.h src/model.h src/entity/entity.h src/entity/light/*.h

DIVIDER="-------------------------- <<[[ COMPILING ]]>> --------------------------"

c:
	@echo $(DIVIDER); $(CC) $(FILES) $(CFLAGS)

r:
	@./bin/out

cr:
	@make c r
