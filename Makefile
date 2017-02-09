all:
	g++ -std=c++14 -O2 -Wall -Wextra -pedantic \
	animation.cpp app.cpp ball.cpp brick.cpp explosion.cpp font.cpp \
	game.cpp level.cpp main.cpp orthocamera.cpp paddle.cpp particle.cpp \
	postprocessor.cpp serialport.cpp shaderprogram.cpp spriterenderer.cpp \
	texture.cpp utility.cpp vao.cpp viewport.cpp window.cpp \
	-I. -I/usr/include/freetype2 \
	-lsfml-window -lsfml-system -lGLEW -lGL -lboost_system -lfreetype \
	-o arkanoid
