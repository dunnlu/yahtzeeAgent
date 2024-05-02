#ifndef game_h
#define game_h

#include <vector>

class Die {
public:
    int roll();
};
 
class Game {
private:
    std::vector<int> dice;
    std::vector<bool> keep;
    Die die;
    std::vector<int> state ; 

public:
    Game();

    std::tuple<std::vector<int>,int,bool> step( std::vector<int> action ) ; 
    bool isItTerminal() ; 
    std::vector<int> reset() ; 
    void rollDice();
    void printDice() const;
    void toggleKeep(int index);
    void resetKeep();
    std::vector<int> possibleScores() const;

    int ones() const;
    int twos() const;
    int threes() const;
    int fours() const;
    int fives() const;
    int sixes() const;
    int threeOfAKind() const;
    int fourOfAKind() const;
    int fullHouse() const;
    int smallStraight() const;
    int largeStraight() const;
    int yahtzee() const;
    int chance() const;

private:
    int countDice(int value) const;
    int sumDice() const;
    
    bool isNOfAKind(int n) const;
    bool isFullHouse() const;
    bool isSmallStraight() const;
    bool isLargeStraight() const;
    bool isYahtzee() const;
};




#endif