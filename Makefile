LDFLAGS = -lGL -lGLU -lglut -lm

main: main.o cuboid.o space.o keyboard.o animate.o
	g++ -o main main.o cuboid.o space.o keyboard.o animate.o $(LDFLAGS)
main.o: main.cpp cuboid.h globalVariables.h
	g++ -c main.cpp $(LDFLAGS)
cuboid.o: cuboid.cpp cuboid.h
	g++ -c cuboid.cpp $(LDFLAGS)
space.o: space.cpp space.h
	g++ -c space.cpp $(LDFLAGS)
keyboard.o: keyboard.cpp keyboard.h globalVariables.h
	g++ -c keyboard.cpp $(LDFLAGS)
animate.o: animate.cpp animate.h
	g++ -c animate.cpp $(LDFLAGS)

clean:
	@rm -f *.o
