#include <iostream>
#include "game.h"

int main(){
    Game game;

    game.printDice();

    game.toggleKeep(1);

    game.rollDice();
    game.printDice();

    game.rollDice();
    game.printDice();

    game.rollDice();
    game.printDice();

    std::vector<int> scores = game.possibleScores();
    for (int i = 0; i < scores.size(); i++)
        std::cout << scores[i] << std::endl;
}
