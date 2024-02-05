FLAGS = 
LIBS = -lglfw -lGLU -lGL -lX11 -lfreetype -lassimp -lXrandr -lpthread -lXi -ldl

glad = ./include/glad/glad.c
gladh = ./include/glad/glad.h
camera = ./Camera/Camera.cpp
camerah = ./Camera/Camera.hpp
lighth = ./Light/Light.hpp
dl = ./Light/DirectionalLight.cpp
dlh = ./Light/DirectionalLight.hpp
pl = ./Light/PointLight.cpp
plh = ./Light/PointLight.hpp
sl = ./Light/SpotLight.cpp
slh = ./Light/SpotLight.hpp
game = ./Game/Game.cpp
gameh = ./Game/Game.cpp
win = ./Window/Window.cpp
winh = ./Window/Window.hpp
object = ./Entity/Object.cpp
objecth = ./Entity/Object.hpp
enth = ./Entity/Entity.hpp
icomh = ./Component/IComponent.hpp
compth = ./Component/Transform.hpp
shader = ./Shader/Shader.cpp
shaderh = ./Shader/Shader.hpp
libh = Lib.hpp
imageh = ./include/texture/stb_image.hpp
util = ./Utils/utils.cpp
utilh = ./Utils/utils.hpp

all: main.exe

# These lines are used for opengl.cpp
###---START---###
opengl: opengl.o shader.o glad.o
	g++ ${FLAGS} -o opengl.exe opengl.o shader.o glad.o ${LIBS}

opengl.o: main.cpp ${imageh} ${libh}
	gcc -c ${FLAGS} main.cpp -o opengl.o
###---END---###

main.exe: main.o window.o object.o glad.o
	g++ ${FLAGS} -o main.exe main.o window.o object.o glad.o ${LIBS}

main.o: main.cpp ${gladh} ${libh} ${enth} ${compth} ${icomh}
	gcc -c ${FLAGS} main.cpp -o main.o

window.o: ${win} ${winh} ${libh} ${gladh}
	gcc -c ${FLAGS} ${win} -o window.o

object.o: ${object} ${objecth} ${libh} ${enth}
	gcc -c ${FLAGS} ${object} -o object.o

glad.o: ${glad} ${gladh}
	gcc -c ${FLAGS} ${glad} -o glad.o

.PHONY:	clean

clean:
	-rm -f *.o *.exe

