CFLAGS = -lrt -lX11 -lGL -lGLU -pthread -lm - Wall -Wextra
LIB = ./lib/libggfonts.a /usr/lib/x86_64-linux-gnu/libopenal.so /usr/lib/x86_64-linux-gnu/libalut.so 
SOURCES = src/dodge.cpp src/ppm.cpp src/log.cpp src/jacobW.cpp src/christianC.cpp src/kyleO.cpp src/koryD.cpp src/youngO.cpp
all: dodge #dodgetest

dodge: $(SOURCES)
	g++ $(SOURCES) $(LIB) $(FLAGS) -o dodge 

#dodgetest: dodgetest.cpp jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp
#	g++ dodgetest.cpp -include jacobW.cpp christianC.cpp kyleO.cpp koryD.cpp youngO.cpp 
#	$(LIB) -o dodgetest $(CFLAGS) /usr/lib/x86_64-linux-gnu/libopenal.so  \ /usr/lib/x86_linux-gnu/libalut.so
#
clean:
	rm -f dodge
#	rm -f dodgetest
	rm -f *.o
