CC = g++ -std=c++11
exe_file = testGame
$(exe_file): game.o testGame.o
	 $(CC) game.o testGame.o -o $(exe_file)    
game.o: ../game/game.cpp
	 $(CC) -c ../game/game.cpp
testGame.o: ../test/testGame.cpp
	 $(CC) -c ../test/testGame.cpp

clean:
	 rm -f *.out *.o $(exe_file)