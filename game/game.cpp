#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <tuple> 

#include "game.h"

int Die::roll() {
    return rand() % 6 + 1;
}

Game::Game() { 

    // Set the random seed. 
    srand(static_cast<unsigned int>(time(0))); 
    // Set the size of the state. 
    state.resize(19) ; 
    // Set the size of the dice. 
    dice.resize(5);
    // Set the size of the keep, initialize is to False. 
    keep.resize(5, false); 
    
    // Reset the game. 
    reset() ; 

} 

std::tuple<std::vector<int>,int,bool> Game::step( std::vector<int> action ) { 
    /* 
    Input: 
    1. Action: { roll_dice_1 , roll_dice_2 , roll_dice_3 , roll_dice_4 , roll_dice_5 , store_at_index }. 
    */ 

   // If the decision is not to store. 
   if ( action[ 5 ] == - 1 ) 
   { 
        // If there is no remaining rolls: 
        if ( state[ 5 ] == 0 ) 
        { 
            // Give a negative reward, return the same state. 
            return std::make_tuple( state ,  - 10 , false ) ; 
        } 
        else 
        { 
            // Set the keep. 
            for ( int i = 0 ; i < 5 ; i ++ ) 
            { 
                if ( action[ i ] == 1 ) 
                { 
                    keep[ i ] = true ; 
                } 
                else
                { 
                    keep[ i ] = false ; 
                }
            }
            // Roll the dice. 
            rollDice() ; 
            // Copy the dice values to the state. 
            for ( int i = 0 ; i < 5 ; i ++ ) 
            { 
                state[ i ] = dice[ i ] ; 
            } 
            // Update the remaining rolls. 
            state[ 5 ] -- ; 
            
            // Return the next state, give a 0 reward. 
            return std::make_tuple( state , 0 , false ) ; 
        }
   } 
   // If the decision is to store. 
   else 
   { 
        // Get the potential scores. 
        std::vector<int> potential_scores = possibleScores() ; 
        // Calculate the reward. 
        int current_reward = potential_scores[ action[ 5 ] ] ; 
        // Set the category to taken. 
        state[ 6 + action[ 5 ] ] = 1 ; 
        // Set all of the keep to false. 
        resetKeep() ; 
        // Roll the dice. 
        rollDice() ; 

        // Copy the dice values to the state. 
        for ( int i = 0 ; i < 5 ; i ++ ) 
        { 
            state[ i ] = dice[ i ] ; 
        } 
        // Update the remaining rolls. 
        state[ 5 ] = 2 ; 
        // Find out if it is terminal. 
        bool terminal = isItTerminal() ; 
        
        // Return the next state, give the reward. 
        return std::make_tuple( state , current_reward, terminal ) ; 

   } 
} 

bool Game::isItTerminal() 
{
    bool terminal = true ; 
    for ( int i = 6 ; i < 19 ; i ++ ) 
    { 
        if ( state[ i ] == 0 )
        { 
            terminal = false ; 
        }
    } 

    return terminal ; 
}

std::vector<int> Game::reset() { 

    // Set all of the keep to False. 
    for ( int i = 0 ; i < 5 ; i ++ ) { 
        keep[ i ] = false ; 
    } 
    // Roll the dice. 
    rollDice() ; 
    // Copy the dice values to state. 
    for ( int i = 0 ; i < 5 ; i ++ ) { 
        state[ i ] = dice[ i ] ; 
    } 
    // Set the remaining roll to 2. 
    state[ 5 ] = 2 ; 
    // Set all of the taken to 0. 
    for ( int i = 6 ; i < 19 ; i ++ ) { 
        state[ i ] = 0 ; 
    } 

    return state ; 

} 

void Game::rollDice() {
    for (int i = 0; i < 5; ++i) {
        if (!keep[i]) {
            dice[i] = die.roll();
        }
    }
    std::sort(dice.begin(), dice.end());
    resetKeep();
} 

void Game::printDice() const {
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

void Game::toggleKeep(int index) {
    keep[index] = !keep[index];
}

void Game::resetKeep() {
    for (int i = 0; i<5; ++i)
        if (keep[i])
            keep[i] = false;
}

std::vector<int> Game::possibleScores() const {
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

int Game::ones() const {
    return countDice(1);
}

int Game::twos() const {
    return countDice(2);
}

int Game::threes() const {
    return countDice(3);
}

int Game::fours() const {
    return countDice(4);
}

int Game::fives() const {
    return countDice(5);
}

int Game::sixes() const {
    return countDice(6);
}

int Game::threeOfAKind() const {
    return isNOfAKind(3) ? sumDice() : 0;
}

int Game::fourOfAKind() const {
    return isNOfAKind(4) ? sumDice() : 0;
}

int Game::fullHouse() const {
    if (isFullHouse())
        return 25;
    return 0;
}

int Game::smallStraight() const {
    if (isSmallStraight())
        return 30;
    return 0;
}

int Game::largeStraight() const {
    if (isLargeStraight())
        return 40;
    return 0;
}

int Game::yahtzee() const {
    return isYahtzee() ? 50 : 0;
}

int Game::chance() const {
    return sumDice();
}

int Game::countDice(int value) const {
    int count = 0;
    for (int die : dice) {
        if (die == value) {
            count += die;
        }
    }
    return count;
}

int Game::sumDice() const {
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        if (keep[i]) {
            sum += dice[i];
        }
    }
    return sum;
}

bool Game::isNOfAKind(int n) const {
    for (int i = 1; i <= 6; ++i) {
        int count = std::count(dice.begin(), dice.end(), i);
        if (count >= n) {
            return true;
        }
    }
    return false;
}

bool Game::isFullHouse() const {
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

bool Game::isSmallStraight() const {
    std::vector<int> count = {0,0,0,0,0,0}; //keep a running count of all dice
    for (int i = 0; i < 5; i++){
        count[dice[i]-1]++; //store the count at the index one below, so dice '1' is stored at [0]
    }
    return ((count[0] && count[1] && count[2] && count[3])||
            (count[1] && count[2] && count[3] && count[4])) || 
                count[2] && count[3] && count[4] && count[5]; 
}

bool Game::isLargeStraight() const {
    return (dice[0] == 1 && dice[1] == 2 && dice[2] == 3 && dice[3] == 4 && dice[4] == 5) ||
            (dice[0] == 2 && dice[1] == 3 && dice[2] == 4 && dice[3] == 5 && dice[4] == 6);
}

bool Game::isYahtzee() const {
    return std::all_of(dice.begin(), dice.end(), [&](int die) { return die == dice[0]; });
}





