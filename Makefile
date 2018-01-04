PROGRAM = main
CC = g++
CFLAGS  = -std=c++11 -g
LDLIBS = -lGL -lGLU -lglut -lm

main: main.o brick.o space.o keyboard.o animate.o mouse.o utility.o camera.o loadModel.o car.o collision.o timer.o image.o textures.o
	$(CC) $(CFLAGS)  -o main *.o $(LDLIBS)
main.o: main.cpp  
	$(CC) $(CFLAGS) -c main.cpp $(LDLIBS)
brick.o: brick.cpp 
	$(CC) $(CFLAGS) -c brick.cpp $(LDLIBS)
space.o: space.cpp 
	$(CC) $(CFLAGS) -c space.cpp $(LDLIBS)
keyboard.o: keyboard.cpp  
	$(CC) $(CFLAGS) -c keyboard.cpp $(LDLIBS)
mouse.o: mouse.cpp  
	$(CC) $(CFLAGS) -c mouse.cpp $(LDLIBS)
animate.o: animate.cpp
	$(CC) $(CFLAGS) -c animate.cpp $(LDLIBS)
utility.o: utility.cpp 
	$(CC) $(CFLAGS) -c utility.cpp $(LDLIBS)
camera.o: camera.cpp 
	$(CC) $(CFLAGS) -c camera.cpp $(LDLIBS)
loadModel.o: loadModel.cpp 
	$(CC) $(CFLAGS) -c loadModel.cpp $(LDLIBS)
car.o: car.cpp 
	$(CC) $(CFLAGS) -c car.cpp $(LDLIBS)
collision.o: collision.cpp 
	$(CC) $(CFLAGS) -c collision.cpp $(LDLIBS)
timer.o: timer.cpp 
	$(CC) $(CFLAGS) -c timer.cpp $(LDLIBS)
image.o: image.cpp 
	$(CC) $(CFLAGS) -c image.cpp $(LDLIBS)
textures.o: textures.cpp 
	$(CC) $(CFLAGS) -c textures.cpp $(LDLIBS)

clean:
	@rm -f *.o main
