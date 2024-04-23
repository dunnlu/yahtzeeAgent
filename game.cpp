#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

class Die {
public:
    int roll() {
        return rand() % 6 + 1;
    }
};

class YahtzeeGame {
private:
    std::vector<int> dice;
    std::vector<bool> keep;
    Die die;

public:
    YahtzeeGame() {
        srand(static_cast<unsigned int>(time(0)));
        dice.resize(5);
        keep.resize(5, false);
        rollDice();
    }

    void rollDice() {
        for (int i = 0; i < 5; ++i) {
            if (!keep[i]) {
                dice[i] = die.roll();
            }
        }
        std::sort(dice.begin(), dice.end());
        resetKeep();
    }

    void printDice() const {
        std::cout << "Dice: ";
        for (int i = 0; i < 5; ++i) {
            if (keep[i]) {
                std::cout << "[" << dice[i] << "] ";
            } else {
                std::cout << dice[i] << " ";
            }
        }
        std::cout << std::endl;
    }

    void toggleKeep(int index) {
        keep[index] = !keep[index];
    }

    void resetKeep() {
        for (int i = 0; i<5; ++i)
            if (keep[i])
                keep[i] = false;
    }

    std::vector<int> possibleScores() {
        std::vector<int> scores = {
            ones(),
            twos(),
            threes(),
            fours(),
            fives(),
            sixes(),
            threeOfAKind(),
            fourOfAKind(),
            fullHouse(),
            smallStraight(),
            largeStraight(),
            yahtzee(),
            chance()
        };
        return scores;
    }

    int ones() const {
        return countDice(1);
    }

    int twos() const {
        return countDice(2);
    }

    int threes() const {
        return countDice(3);
    }

    int fours() const {
        return countDice(4);
    }

    int fives() const {
        return countDice(5);
    }

    int sixes() const {
        return countDice(6);
    }

    int threeOfAKind() const {
        return isNOfAKind(3) ? sumDice() : 0;
    }

    int fourOfAKind() const {
        return isNOfAKind(4) ? sumDice() : 0;
    }

    int fullHouse() const {
        if (isFullHouse())
            return 25;
        return 0;
    }

    int smallStraight() const {
        if (isSmallStraight())
            return 30;
        return 0;
    }

    int largeStraight() const {
        if (isLargeStraight())
            return 40;
        return 0;
    }

    int yahtzee() const {
        return isYahtzee() ? 50 : 0;
    }

    int chance() const {
        return sumDice();
    }

private:
    int countDice(int value) const {
        int count = 0;
        for (int die : dice) {
            if (die == value) {
                count += die;
            }
        }
        return count;
    }

    int sumDice() const {
        int sum = 0;
        for (int i = 0; i < 5; ++i) {
            if (keep[i]) {
                sum += dice[i];
            }
        }
        return sum;
    }

    bool isNOfAKind(int n) const {
        for (int i = 1; i <= 6; ++i) {
            int count = std::count(dice.begin(), dice.end(), i);
            if (count >= n) {
                return true;
            }
        }
        return false;
    }

    bool isFullHouse() const {
        bool two = false, three = false;
        for (int i = 1; i <= 6; ++i) { //for all dice, count the number
            int count = std::count(dice.begin(), dice.end(), i);
            if (count == 2) {
                two = true;
            } else if (count == 3) {
                three = true;
            }
        }
        return two && three; //we have to have one thats exactly 2 and one thats exactly 3
    }

    bool isSmallStraight() const {
        std::vector<int> count = {0,0,0,0,0,0}; //keep a running count of all dice
        for (int i = 0; i < 5; i++){
            count[dice[i]-1]++; //store the count at the index one below, so dice '1' is stored at [0]
        }
        return ((count[0] && count[1] && count[2] && count[3])||
                (count[1] && count[2] && count[3] && count[4])) || 
                 count[2] && count[3] && count[4] && count[5]; 
    }

    bool isLargeStraight() const {
        return (dice[0] == 1 && dice[1] == 2 && dice[2] == 3 && dice[3] == 4 && dice[4] == 5) ||
               (dice[0] == 2 && dice[1] == 3 && dice[2] == 4 && dice[3] == 5 && dice[4] == 6);
    }
    
    bool isYahtzee() const {
        return std::all_of(dice.begin(), dice.end(), [&](int die) { return die == dice[0]; });
    }
};