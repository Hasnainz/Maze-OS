# -*- MakeFile -*-

#target: dependencies
#	action
all: tetris

tetris: tetris.o
	cc tetris.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o tetris

tetris.o: tetris.c
	cc tetris.c -c

clean:
	rm -f *.o tetris