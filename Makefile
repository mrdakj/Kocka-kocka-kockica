PROGRAM = main
CC = g++
CFLAGS  = -std=c++11 -g -Wall -Wextra
LDLIBS = -lGL -lGLU -lglut -lm

main: main.o brick.o room.o keyboard.o init_timers.o mouse.o utility.o camera.o loadModel.o car.o collision.o timer.o image.o textures.o vector3f.o
	$(CC) $(CFLAGS)  -o main *.o $(LDLIBS)
main.o: main.cpp  
	$(CC) $(CFLAGS) -c main.cpp $(LDLIBS)
brick.o: brick.cpp headers/brick.h
	$(CC) $(CFLAGS) -c brick.cpp $(LDLIBS)
room.o: room.cpp  headers/room.h
	$(CC) $(CFLAGS) -c room.cpp $(LDLIBS)
keyboard.o: keyboard.cpp  headers/keyboard.h
	$(CC) $(CFLAGS) -c keyboard.cpp $(LDLIBS)
mouse.o: mouse.cpp  headers/mouse.h
	$(CC) $(CFLAGS) -c mouse.cpp $(LDLIBS)
init_timers.o: init_timers.cpp
	$(CC) $(CFLAGS) -c init_timers.cpp $(LDLIBS)
utility.o: utility.cpp  headers/utility.h
	$(CC) $(CFLAGS) -c utility.cpp $(LDLIBS)
camera.o: camera.cpp headers/camera.h
	$(CC) $(CFLAGS) -c camera.cpp $(LDLIBS)
loadModel.o: loadModel.cpp headers/loadModel.h
	$(CC) $(CFLAGS) -c loadModel.cpp $(LDLIBS)
car.o: car.cpp headers/car.h
	$(CC) $(CFLAGS) -c car.cpp $(LDLIBS)
collision.o: collision.cpp headers/collision.h
	$(CC) $(CFLAGS) -c collision.cpp $(LDLIBS)
timer.o: timer.cpp headers/timer.h
	$(CC) $(CFLAGS) -c timer.cpp $(LDLIBS)
image.o: image.cpp headers/image.h
	$(CC) $(CFLAGS) -c image.cpp $(LDLIBS)
textures.o: textures.cpp headers/textures.h
	$(CC) $(CFLAGS) -c textures.cpp $(LDLIBS)
vector3f.o: vector3f.cpp headers/vector3f.h
	$(CC) $(CFLAGS) -c vector3f.cpp $(LDLIBS)

clean:
	@rm -f *.o main
