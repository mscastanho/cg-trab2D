all: compila clean

compila:
	g++ -c tinyxml2.cpp
	g++ -c rect.cpp -lGL -lGLU -lglut
	g++ -c circle.cpp -lGL -lGLU -lglut -lm
	g++ -c car.cpp -lGL -lGLU -lglut
	g++ -c input.cpp tinyxml2.o
	g++ main.cpp -o trabalhocg tinyxml2.o rect.o circle.o car.o input.o -lGL -lGLU -lglut

clean:
	@rm -f *.o
