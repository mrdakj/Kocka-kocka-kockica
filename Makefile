PROGRAM = main
CC = g++
CFLAGS  = -std=c++11 -g -Wall -Wextra
LDLIBS = -lGL -lGLU -lglut -lm

main: main.o brick.o room.o keyboard.o init_timers.o mouse.o utility.o camera.o car.o timer.o image.o textures.o vector3f.o model.o
	$(CC) $(CFLAGS)  -o main *.o $(LDLIBS)
main.o: sources/main.cpp  
	$(CC) $(CFLAGS) -c sources/main.cpp $(LDLIBS)
brick.o: sources/brick.cpp headers/brick.h
	$(CC) $(CFLAGS) -c sources/brick.cpp $(LDLIBS)
room.o: sources/room.cpp  headers/room.h
	$(CC) $(CFLAGS) -c sources/room.cpp $(LDLIBS)
keyboard.o: sources/keyboard.cpp  headers/keyboard.h
	$(CC) $(CFLAGS) -c sources/keyboard.cpp $(LDLIBS)
mouse.o: sources/mouse.cpp  headers/mouse.h
	$(CC) $(CFLAGS) -c sources/mouse.cpp $(LDLIBS)
init_timers.o: sources/init_timers.cpp
	$(CC) $(CFLAGS) -c sources/init_timers.cpp $(LDLIBS)
utility.o: sources/utility.cpp  headers/utility.h
	$(CC) $(CFLAGS) -c sources/utility.cpp $(LDLIBS)
camera.o: sources/camera.cpp headers/camera.h
	$(CC) $(CFLAGS) -c sources/camera.cpp $(LDLIBS)
car.o: sources/car.cpp headers/car.h
	$(CC) $(CFLAGS) -c sources/car.cpp $(LDLIBS)
timer.o: sources/timer.cpp headers/timer.h
	$(CC) $(CFLAGS) -c sources/timer.cpp $(LDLIBS)
image.o: sources/image.cpp headers/image.h
	$(CC) $(CFLAGS) -c sources/image.cpp $(LDLIBS)
textures.o: sources/textures.cpp headers/textures.h
	$(CC) $(CFLAGS) -c sources/textures.cpp $(LDLIBS)
vector3f.o: sources/vector3f.cpp headers/vector3f.h
	$(CC) $(CFLAGS) -c sources/vector3f.cpp $(LDLIBS)
model.o: sources/model.cpp headers/model.h
	$(CC) $(CFLAGS) -c sources/model.cpp $(LDLIBS)

clean:
	@rm -f *.o main
