all: main

main: main.cpp sansi.cpp sansi.h
	g++ main.cpp sansi.cpp -o main