FLAGS = 
LIBS = -lGLEW -lGLU -lglfw -lGL -lX11 -lfreetype -lassimp -lXrandr -lpthread -lXi -ldl

glad = ./include/glad/glad.c
gladh = ./include/glad/glad.h
libh = Lib.hpp
win = ./Window/Window.cpp
winh = ./Window/Window.hpp
game = ./Game/Game.cpp
gameh = ./Game/Game.hpp
worldh = ./World/World.hpp
vaoh = ./Buffers/VAO.hpp
vboh = ./Buffers/VBO.hpp
shader = ./Shader/Shader.cpp
shaderh = ./Shader/Shader.hpp
enth = ./Entity/Entity.hpp
icomh = ./Component/IComponent.hpp
transfh = ./Component/Transform.hpp
isysh = ./System/ISystem.hpp

# camera = ./Camera/Camera.cpp
# camerah = ./Camera/Camera.hpp
# lighth = ./Light/Light.hpp
# dl = ./Light/DirectionalLight.cpp
# dlh = ./Light/DirectionalLight.hpp
# pl = ./Light/PointLight.cpp
# plh = ./Light/PointLight.hpp
# sl = ./Light/SpotLight.cpp
# slh = ./Light/SpotLight.hpp
# imageh = ./include/texture/stb_image.hpp
# util = ./Utils/utils.cpp
# utilh = ./Utils/utils.hpp

all: main.exe


main.exe: main.o window.o game.o shader.o glad.o
	g++ ${FLAGS} -o main.exe main.o window.o game.o shader.o glad.o ${LIBS}

main.o: main.cpp ${gladh} ${libh} ${enth} ${transfh} ${icomh} ${isysh}
	gcc -c ${FLAGS} main.cpp -o main.o

window.o: ${win} ${winh} ${libh} ${gladh}
	gcc -c ${FLAGS} ${win} -o window.o

game.o: ${game} ${gameh} ${libh} ${worldh}
	gcc -c ${FLAGS} ${game} -o game.o

shader.o: ${shader} ${shaderh} ${shapeh} ${gladh}
	gcc -c ${FLAGS} ${shader} -o shader.o

glad.o: ${glad} ${gladh}
	gcc -c ${FLAGS} ${glad} -o glad.o

# These lines are used for opengl.cpp
###---START---###
opengl: opengl.o shader.o glad.o
	g++ ${FLAGS} -o opengl.exe opengl.o shader.o glad.o ${LIBS}

opengl.o: main.cpp ${imageh} ${libh}
	gcc -c ${FLAGS} main.cpp -o opengl.o
###---END---###

.PHONY:	clean

clean:
	-rm -f *.o *.exe

