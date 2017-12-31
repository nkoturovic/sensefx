PROGRAM = senseFX.out
CXX     = g++
CXXFLAGS = -std=c++17
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib -g
LDLIBS  = -lglut -lGLU -lGL -lGLEW -lstdc++fs 
WFLAGS = -Wall -Wextra

$(PROGRAM): main.o ast.o config.o object.o keyboard.o user.o camera.o axis.o
	$(CXX) $(LDFLAGS) -o $(PROGRAM) main.o ast.o config.o object.o keyboard.o user.o camera.o axis.o $(LDLIBS) $(WFLAGS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM)

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
