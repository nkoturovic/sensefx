PROGRAM = senseFX
CXX     = g++
CFLAGS  = -g -Wall -std=c99 -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lGLEW

$(PROGRAM): main.o vec.o 
	$(CXX) $(LDFLAGS) -o $(PROGRAM) main.o vec.o $(LDLIBS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
