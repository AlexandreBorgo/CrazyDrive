CC=gcc
CFLAGS=-Wall
LDFLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_gfx
EXEC=program
NAME="Crazy Drive"

all: $(EXEC)

program: Car.o SDL2_Interface.o main.o 
	$(CC) -o $(NAME) Car.o main.o $(LDFLAGS)

Car.o: Car.c
	$(CC) -o Car.o -c Car.c $(CFLAGS)

SDL2_Interface.o:
	$(CC) -o SDL2_Interface.o -c SDL2_Interface.h $(CFLAGS)

main.o: main.c Car.h config.h 
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(NAME)
