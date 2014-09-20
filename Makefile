ui: main.c draw.h draw.c layout.h layout.cpp view.cpp view.h
	gcc -std=gnu99 -g main.c -c
	gcc -std=gnu99 -g draw.c -c
	g++ -std=c++11 -g view.cpp -c
	gcc -std=gnu99 -g nanovg/*.c -c
	g++ -std=c++11 -g layout.cpp -fPIC -fno-rtti -c
	g++ view.o nanovg.o draw.o main.o layout.o -o ui -lm -lGL -lglfw -lGLEW -g

clean:
	rm -f *.o
	rm -f *~
	rm -f ui
