PROGRAM = main
CC = g++
CFLAGS  = -std=c++11 -g
LDLIBS = -lGL -lGLU -lglut -lm

main: main.o brick.o space.o keyboard.o animate.o mouse.o utility.o camera.o loadModel.o car.o collision.o timer.o
	$(CC) $(CFLAGS)  -o main *.o $(LDLIBS)
main.o: main.cpp brick.h 
	$(CC) $(CFLAGS) -c main.cpp $(LDLIBS)
brick.o: brick.cpp brick.h
	$(CC) $(CFLAGS) -c brick.cpp $(LDLIBS)
space.o: space.cpp space.h
	$(CC) $(CFLAGS) -c space.cpp $(LDLIBS)
keyboard.o: keyboard.cpp keyboard.h 
	$(CC) $(CFLAGS) -c keyboard.cpp $(LDLIBS)
mouse.o: mouse.cpp mouse.h 
	$(CC) $(CFLAGS) -c mouse.cpp $(LDLIBS)
animate.o: animate.cpp
	$(CC) $(CFLAGS) -c animate.cpp $(LDLIBS)
utility.o: utility.cpp utility.h
	$(CC) $(CFLAGS) -c utility.cpp $(LDLIBS)
camera.o: camera.cpp camera.h
	$(CC) $(CFLAGS) -c camera.cpp $(LDLIBS)
loadModel.o: loadModel.cpp loadModel.h
	$(CC) $(CFLAGS) -c loadModel.cpp $(LDLIBS)
car.o: car.cpp car.h
	$(CC) $(CFLAGS) -c car.cpp $(LDLIBS)
collision.o: collision.cpp collision.h
	$(CC) $(CFLAGS) -c collision.cpp $(LDLIBS)
timer.o: timer.cpp timer.h
	$(CC) $(CFLAGS) -c timer.cpp $(LDLIBS)

clean:
	@rm -f *.o main
