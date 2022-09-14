
all: main.out

main.out: main.o game.o particle.o
	g++ -o $@ $^ -L$(stdOpenGL) -lline -lcircle -lshader_program -lglad -lGL -lglfw -ldl

%.o: %.cpp
	g++ -o $@ $^ -c -I $(stdOpenGL)
