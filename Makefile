CC = g++ -std=c++11
exe_file = testGame
$(exe_file): game.o testGame.o
	 $(CC) game.o testGame.o -o $(exe_file)    
pokedex.o: game.cpp
	 $(CC) -c game.cpp
pokemon.o: testGame.cpp
	 $(CC) -c testGame.cpp

clean:
	 rm -f *.out *.o $(exe_file)