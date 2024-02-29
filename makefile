build:
	gcc -Wall -std=c99 ./src/*.c ./src/*.h -lSDL2 -o game

run:
	./game

clean:
	rm game

default:
	build

