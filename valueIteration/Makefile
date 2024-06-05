CC = g++ -std=c++11
CFLAGS = -c 
 
exe_file = valit

all: $(exe_file) 

$(exe_file): game.o valit.o
	$(CC) game.o valit.o -o $(exe_file) 

game.o: ../game/game.cpp ../game/game.h
	$(CC) $(CFLAGS) ../game/game.cpp

valit.o: valit.cpp
	$(CC) $(CFLAGS) valit.cpp

.PHONY: clean
clean:
	rm -f *.out *.o $(exe_file)
