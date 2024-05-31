all: main zombie

main: main.cpp
	g++ -o main main.cpp

zombie: zombie.c
	gcc -o zombie zombie.c
