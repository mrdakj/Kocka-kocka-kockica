LDFLAGS = -lGL -lGLU -lglut -lm

main: main.o cuboid.o space.o keyboard.o animate.o mouse.o utility.o
	g++ -o main *.o $(LDFLAGS)
main.o: main.cpp cuboid.h globalVariables.h
	g++ -c main.cpp $(LDFLAGS)
cuboid.o: cuboid.cpp cuboid.h
	g++ -c cuboid.cpp $(LDFLAGS)
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

clean:
	@rm -f *.o