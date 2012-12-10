all: flip

flip: image.o main.o
	g++  image.o main.o -ljpeg -o flip

main.o: main.cpp
	g++ -c -O3 main.cpp

image.o: image.cpp
	g++ -c -O3 image.cpp

clean:
	rm -rf *o flip outputImages/*
