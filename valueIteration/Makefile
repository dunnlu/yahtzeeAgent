CC = g++ -std=c++11
CFLAGS = -c -lm -fopenmp -g
LDFLAGS = -fopenmp -g

exe_file = valit
exe_file2 = valitParallel

all: $(exe_file) $(exe_file2)

$(exe_file): game.o valit.o
	$(CC) game.o valit.o -o $(exe_file) $(LDFLAGS)   

$(exe_file2): game.o valitParallel.o
	$(CC) game.o valitParallel.o -o $(exe_file2) $(LDFLAGS)

game.o: ../game/game.cpp ../game/game.h
	$(CC) $(CFLAGS) ../game/game.cpp

valit.o: valit.cpp
	$(CC) $(CFLAGS) valit.cpp

valitParallel.o: valitParallel.cpp
	$(CC) $(CFLAGS) valitParallel.cpp

.PHONY: clean
clean:
	rm -f *.out *.o $(exe_file) $(exe_file2)
