LDFLAGS = -lGL -lGLU -lglut -lm -g

main: main.o brick.o space.o keyboard.o animate.o mouse.o utility.o camera.o loadModel.o car.o
	g++  -o main *.o $(LDFLAGS)
main.o: main.cpp brick.h globalVariables.h
	g++ -c main.cpp $(LDFLAGS)
brick.o: brick.cpp brick.h
	g++ -c brick.cpp $(LDFLAGS)
space.o: space.cpp space.h
	g++ -c space.cpp $(LDFLAGS)
keyboard.o: keyboard.cpp keyboard.h globalVariables.h
	g++ -c keyboard.cpp $(LDFLAGS)
mouse.o: mouse.cpp mouse.h globalVariables.h
	g++ -c mouse.cpp $(LDFLAGS)
animate.o: animate.cpp animate.h
	g++ -c animate.cpp $(LDFLAGS)
utility.o: utility.cpp utility.h
	g++ -c utility.cpp $(LDFLAGS)
camera.o: camera.cpp camera.h
	g++ -c camera.cpp $(LDFLAGS)
loadModel.o: loadModel.cpp loadModel.h
	g++ -c loadModel.cpp $(LDFLAGS)
car.o: car.cpp car.h
	g++ -c car.cpp $(LDFLAGS)

clean:
	@rm -f *.o main
