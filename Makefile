all: main

main: main.cpp sansi.cpp sansi.h
	g++ main.cpp sansi.cpp -o main

srcgen: srcgen.cpp
	g++ srcgen.cpp -o srcgen

clean:
	rm -r main srcgen