all: dodge

dodge: dodge.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodge.cpp ./libraries/ppm.cpp ./libraries/fonts.h ./libraries/log.cpp ./libraries/libggfonts.a -Wall -o dodge -lX11 -lGL -lGLU -lm

clean:
	rm -f dodge
	rm -f *.o
