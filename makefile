CC          = g++
MAKE        = make
SHELL       = /bin/sh

LIBS = -lGL -lopencv_core -lopencv_highgui
EXECS = vinv
SOURCES = VINV.cpp
CXXFLAGS= -O3 -Wall -std=c++0x
OBJECTS = VINV.o 

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS) $(LIBS) 

.cpp.o: obj/
	$(CC) -c $(SOURCES) $(CFLAGS) $(LIBS) 

install: $(EXECS)
	mv $(EXECS) /usr/bin

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif
run: $(EXECS) 
	./$(EXECS) -c test/conffinal.dat 5000 0.05 -b 10 -o test/saida.png -l "N=5000" -l "Yukawa" -l "Teste" 
	./$(EXECS) -c test/conffinal.dat 5000 0.05 -b 10 -o test/saida.eps -l "N=5000" -l "Yukawa" -l "Teste" 
