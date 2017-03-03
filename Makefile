
CFLAGS = -Wall -lX11 -lGL -lGLU -lm
LIB = ./libraries/ppm.cpp ./libraries/fonts.h ./libraries/log.cpp ./libraries/libggfonts.a 
all: dodge dodgetest

dodge: dodge.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodge.cpp -include jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	$(LIB) -o dodge $(CFLAGS) /usr/lib/x86_64-linux-gnu/libopenal.so \ /usr/lib/x86_linux-gnu/libalut.so

dodgetest: dodgetest.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
	g++ dodgetest.cpp -include jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp 
	$(LIB) -o dodgetest $(CFLAGS) /usr/lib/x86_64-linux-gnu/libopenal.so  \ /usr/lib/x86_linux-gnu/libalut.so

clean:
	rm -f dodge
	rm -f dodgetest
	rm -f *.o
