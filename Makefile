all: main srcgen toBin birthday_attack

main: main.cpp sansi.cpp sansi.h
	g++ main.cpp sansi.cpp -o main

srcgen: srcgen.cpp
	g++ srcgen.cpp -o srcgen

toBin: outToBin.cpp
	g++ outToBin.cpp -o toBin

birthday_attack: birthday_attack.cpp sansi.cpp sansi.h
	g++ birthday_attack.cpp sansi.cpp -o birthday_attack

clean:
	rm -r main srcgen