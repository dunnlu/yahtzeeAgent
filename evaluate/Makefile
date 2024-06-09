CC = g++ -std=c++11
exe_file = evaluate
$(exe_file): game.o evaluate.o
	 $(CC) game.o evaluate.o -o $(exe_file)    
game.o: ../game/game.cpp
	 $(CC) -c ../game/game.cpp
testGame.o: evaluate.cpp
	 $(CC) -c evaluate.cpp

clean:
	 rm -f *.out *.o $(exe_file)