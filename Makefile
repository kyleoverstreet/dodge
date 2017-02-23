CFLAGS = -I ./include
##LIB  = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lx11 -lGLU -lGL -pthread -lm #-lXrandr

all: dodge

dodge: dodge.cpp ppm.cpp log.cpp
	g++ $(CFLAGS) dodge.cpp ppm.cpp log.cpp christianC.cpp kyleO.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o dodge

clean:
	rm -f dodge
	rm -f *.o
