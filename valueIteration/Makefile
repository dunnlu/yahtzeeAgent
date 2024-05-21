CC = g++ -std=c++11
exe_file = valit
$(exe_file): game.o valit.o
	 $(CC) game.o valit.o -o $(exe_file)    
game.o: ../game/game.cpp
	 $(CC) -c ../game/game.cpp
valit.o: valit.cpp
	 $(CC) -c valit.cpp

clean:
	 rm -f *.out *.o $(exe_file)