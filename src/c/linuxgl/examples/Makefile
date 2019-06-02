CC=gcc
DEPS=../zero.h
LIBS=-lglfw -lGL -lGLEW

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rgb: rgb.o
	$(CC) -orgb rgb.o $(LIBS)
	rm *.o
