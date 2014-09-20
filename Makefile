ui: main.c draw.h draw.c layout.h layout.cpp view.c view.h
	gcc -std=gnu99 main.c -c
	gcc -std=gnu99 draw.c -c
	gcc -std=gnu99 view.c -c
	gcc -std=gnu99 nanovg/*.c -c
	g++ -std=c++11 layout.cpp -fPIC -fno-rtti -c
	g++ view.o nanovg.o draw.o main.o layout.o -o ui -lm -lGL -lglfw -lGLEW -g
