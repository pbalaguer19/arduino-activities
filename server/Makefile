pacman:
	g++ pacman.cpp -o pacman -lglut -lGLU -lGL -lm -ljpeg

pacmanMac:
	g++ pacman.cpp -o pacman -framework OpenGL -framework GLUT -framework Cocoa -lm -std=c++0x -Wno-deprecated-declarations -ljpeg

clear:
	rm pacman

all:
	make clear; make
