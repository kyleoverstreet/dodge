all: dodge dodgetest

dodge: dodge.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodge.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp /
	/libraries/ppm.cpp ./libraries/fonts.h ./libraries/log.cpp ./libraries/libggfonts.a -Wall -o dodge -lX11 -lGL -lGLU -lm

dodgetest: dodgetest.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodgetest.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp /
	./libraries/ppm.cpp ./libraries/fonts.h ./libraries/log.cpp ./libraries/libggfonts.a -Wall -o dodgetest -lX11 -lGL -lGLU -lm

clean:
	rm -f dodge
	rm -f dodgetest
	rm -f *.o
