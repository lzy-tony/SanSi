all: main srcgen toBin

main: main.cpp sansi.cpp sansi.h
	g++ main.cpp sansi.cpp -o main

srcgen: srcgen.cpp
	g++ srcgen.cpp -o srcgen

toBin: outToBin.cpp
	g++ outToBin.cpp -o toBin

clean:
	rm -r main srcgen