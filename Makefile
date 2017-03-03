all: dodge dodgetest

dodge: dodge.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodge.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp /
	/libraries/ppm.cpp ./libraries/fonts.h ./libraries/log.cpp ./libraries/libggfonts.a -Wall -o dodge -lX11 -lGL -lGLU -lm
	/usr/lib/x86_64-linux-gnu/libopenal.so \ /usr/lib/x86_linux-gnu/libalut.so

dodgetest: dodgetest.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodgetest.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp /
	./libraries/ppm.cpp ./libraries/fonts.h ./libraries/log.cpp ./libraries/libggfonts.a -Wall -o dodgetest -lX11 -lGL -lGLU -lm
	/usr/lib/x86_64-linux-gnu/libopenal.so  \ /usr/lib/x86_linux-gnu/libalut.so

clean:
	rm -f dodge
	rm -f dodgetest
	rm -f *.o
