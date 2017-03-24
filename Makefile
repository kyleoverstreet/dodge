FLAGS = -lrt -lX11 -lGL -lGLU -pthread -lm -Wall -Wextra
LIB = ./lib/libggfonts.a /usr/lib/x86_64-linux-gnu/libopenal.so /usr/lib/x86_64-linux-gnu/libalut.so 
SOURCES = src/dodge.cpp src/ppm.cpp src/log.cpp jacobW.cpp src/christianC.cpp src/kyleO.cpp src/koryD.cpp src/youngO.cpp

all: dodge

dodge: $(SOURCES)
	g++ -std=c++11 $(SOURCES) $(LIB) $(FLAGS) -o dodge 

clean:
	rm -f dodge *.html x.x *.txt 
