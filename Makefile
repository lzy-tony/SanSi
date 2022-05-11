all: main srcgen toBin birthday_attack performance_test

main: main.cpp sansi.cpp sansi.h
	g++ main.cpp sansi.cpp -o main -O3

srcgen: srcgen.cpp
	g++ srcgen.cpp -o srcgen -O3

toBin: outToBin.cpp
	g++ outToBin.cpp -o toBin -O3

birthday_attack: birthday_attack.cpp sansi.cpp sansi.h
	g++ birthday_attack.cpp sansi.cpp -o birthday_attack -O3

performance_test: performance_test.cpp sansi.cpp sansi.h
	g++ performance_test.cpp sansi.cpp -o performance_test -O3

clean:
	rm -r main srcgen birthday_attack toBin performance_test