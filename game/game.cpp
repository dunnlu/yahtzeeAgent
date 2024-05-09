#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <tuple> 
#include <cmath>

#include "game.h"

int Die::roll() {
    return rand() % 6 + 1;
}

Game::Game() { 



    // Set the random seed. 
    srand(static_cast<unsigned int>(time(0))); 
    // Set the size of the state. 
        // 0 - 4 is dice
        // 5 is rolls left
        // 6 - 11 is upper section
        // 12 - 18 is lower section
    state.resize(19);
    // Set the size of the dice. 
    dice.resize(5);
    // Set the size of the keep, initialize is to False. 
    keep.resize(5, false); 
    
    // Reset the game. 
    reset() ; 

} 



/*
    Function to return the list of all 252 dice combinations in non decreasing order


*/

std::vector<vector<int>> Game::diceConfigurations() {
    std::vector<std::vector<int>> configurations;
    
    for (int d1 = 1; d1 <= 6; ++d1) {
        for (int d2 = d1; d2 <= 6; ++d2) {
            for (int d3 = d2; d3 <= 6; ++d3) {
                for (int d4 = d3; d4 <= 6; ++d4) {
                    for (int d5 = d4; d5 <= 6; ++d5) {
                        configurations.push_back({d1, d2, d3, d4, d5});
                    }
                }
            }
        }
    }
    
    return configurations;
}


std::vector<vector<int>> Game::scoreCardConfigHalf {
    std::vector<int> config;
    config.resize(128);
    int count = 0;
    for (int a = 0; a < 2; a++)
        for (int b = 0; b < 2; b++)
            for (int b = 0; b < 2; b++)
                for (int c = 0; c < 2; c++)
                    for (int d = 0; d < 2; d++)
                        for (int e = 0; e < 2; e++)
                            for (int f = 0; f < 2; f++) {
                                config[count].push_back({a,b,c,d,e,f});
                                count++;
                            }
    return config;
}                   

// Returns a 96,768 x 13 array of all state space configurations for the lower section only

std::vector<vector<int>> Game::stateSpaceHalf() {
    // if full = true --> generate full state space
    // if full = false --> generate lower section only state space (indices of the scoreboard kept the same)

    // distribution of spaces for state space i:
        // i % 252 == dice config
        // i/252 % 3 == rolls left
        // i / (252*3) == scorecard config

    std::vector<vector<int>> space, diceConfig = diceConfigurations(), scorecardConfig = scorecardConfigurations();
    space.resize(3*252*128); // 3 rolls, 252 dice, scorecard configurations

    for (int i = 0; i < space.size(); i++) {
        space[i].resize(13);
            // 0 - 4 is dice
            // 5 is rolls left
            // 6 - 12 is lower section
        for (int j = 0; j < 5; j++)
            space[i][j] = diceConfig[i%252][j]
        space[5] = (i / 252) % 3;
        for (int j = 6; j < 13; j++)
            space[i][j] = scorecardConfig[(i/(252*3)%128)][j]
        
    }


    return space;
}

// Return the reward sent action (the only relevent information is the action taken in the 5th column of actions, the scorecard action)
// Assume we are in the desired state

int Game::reward(int action5) {
    
    // if the action is not to score or we are in a terminal state
    
    if ((action5 == -1) || isTerminal()) {
        return 0;
    } else {
        std::vector<int> scores = possibleScores()
        return scores[action5];
    }
}


void Game::goToState(std::vector<int> target) {
    for (int i = 0; i < 19; i++)
        state[i] = target[i]
}


std::tuple<std::vector<int>,int,bool> Game::step( std::vector<int> action ) { 
    /* 
    Input: 
    1. Action: { keep_dice_1 , keep_dice_2 , keep_dice_3 , keep_dice_4 , keep_dice_5 , store_at_index }. 
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
        bool terminal = isTerminal() ; 
        
        // Return the next state, give the reward. 
        return std::make_tuple( state , current_reward, terminal ) ; 

   } 
} 

bool Game::isTerminal() 
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
    resetKeep();
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

// Returns a 2d array containing all possible actions in this state
std::vector<std::vector<int>> Game::possibleActions() {
    std::vector<std::vector<int>> actions;
    int temp;
    int squares = countAvailableSquares() ; 


    // add scoring actions

    actions.resize(squares);
    int next = 6;
    for (int i = 0; i < squares; i++) {

        // Action: { keep_dice_1 , keep_dice_2 , keep_dice_3 , keep_dice_4 , keep_dice_5 , store_at_index }

        actions[i].resize(6);

        // fill dice keep with zeroes

        for (int j = 0; j < 5; j++)
            actions[i][j] = 0;
        
        //next available square

        while (state[next] == 1)
            next++;
        actions[i][5] = next;
        next++;
    }


    // add dice keep actions, only if there are rolls left

    if (state[5] > 0){

        //resize once, add all possible dice keeps

        actions.resize(squares + 32); 

        // add all actions where you decide to keep rolling

        for (int i = 0; i < 32; i++) {

            // 0th dice keep state is the 'squares'th state, for example if we had 5 available squares, keeping no dice would be possibleActions[5]

            actions[squares + i].resize(6); 
            temp = i;

            // fill 0-4 with the binary representation of the index, example, 10 would be 01010 and 1 would be 00001

            for (int j = 0; j<5; j++) {
                actions[squares + i][j] = temp % 2;
                temp /= 2;
            }
            actions[squares + i][5] = -1;
        }
    }


    return actions;

}

//states: 6 - 11 is upper section
int Game::countAvailableSquaresUpper() {
    int count = 0;
    for (int i = 6; i < 12; i++)
        if (state[i] == 0)
            count++;
    return count;
}

//states: 12-18 is lower section
int Game::countAvailableSquaresLower() {
    int count = 0;
    for (int i = 12; i < 19; i++)
        if (state[i] == 0)
            count++;
    return count;
}

//states: 6-18 is scoresheet
int Game::countAvailableSquares() {
    int count = 0;
    for (int i = 6; i < 19; i++)
        if (state[i] == 0)
            count++;
    return count;
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





