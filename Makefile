ui: main.c draw.h draw.c layout.h layout.cpp view.cpp view.h factory.cpp widget.h
	gcc -std=gnu99 -O2 -g main.c -c
	gcc -std=gnu99 -O2 -g draw.c -c
	g++ -std=c++11 -O2 -g view.cpp -c
	gcc -std=gnu99 -O2 -g nanovg/*.c -c
	g++ -std=c++11 -O2 -g layout.cpp -fPIC -fno-rtti -c
	g++ -std=c++11 -O2 -g factory.cpp -fPIC -fno-rtti -c
	g++ view.o nanovg.o draw.o main.o layout.o factory.o -o ui -lm -lGL -lglfw -lGLEW -lyaml-cpp -g

clean:
	rm -f *.o
	rm -f *~
	rm -f ui
