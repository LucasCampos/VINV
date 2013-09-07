CC          = g++
MAKE        = make
SHELL       = /bin/sh

LIBS = -lGL -lopencv_core -lopencv_highgui -lX11
EXECS = vinv
SOURCES = VINV.cpp
CXXFLAGS= -std=c++0x -O3 -Wall 
OBJECTS = VINV.o 

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) $(CXXFLAGS)-o $@  $(LIBS) 

.cpp.o: obj/
	$(CC) -c $(SOURCES) $(CXXFLAGS) $(LIBS) 

install: $(EXECS)
	mv $(EXECS) /usr/bin

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif
run: $(EXECS) 
	./$(EXECS) -c test/transannealingR17_Alfa32.dat 336 1 -b 30 -o test/saida.avi
