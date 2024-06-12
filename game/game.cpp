#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <tuple> 
#include <cmath>
#include <string>
#include "json.hpp"
#include <fstream>

#include "game.h"

using json = nlohmann::json;


#define rewardMode 0
/*
    Reward Modes
    - 0 : Minimalistic
    - 1 : Experimental 
    - 2 : Common Human Strategy
    - 3 : Minimalistic Human Strategy

    The two mentioned in the report are 0 and 2

*/

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

    readJSON();
    
    // Reset the game. 
    reset() ; 


} 


void Game::readJSON() {
    

    const char* file_path = "diceOdds.json";
    FILE* file = fopen(file_path, "rb");

    if (!file) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<char> buffer(file_size);
    fread(buffer.data(), 1, file_size, file);
    fclose(file);

    std::string json_string(buffer.begin(), buffer.end());

    if (json_string.empty()) {
        std::cerr << "File is empty: " << file_path << std::endl;
        exit(1);
    }

    try {
        diceOdds = nlohmann::json::parse(json_string);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        exit(1);
    }
}



/*
    Function to return the list of all 252 dice combinations in non decreasing order


*/

std::vector<std::vector<int>> Game::diceConfigurations() {
    std::vector<std::vector<int>> configurations;
    
    for (int d1 = 1; d1 <= 6; ++d1) {
        for (int d2 = d1; d2 <= 6; ++d2) {
            for (int d3 = d2; d3 <= 6; ++d3) {
                for (int d4 = d3; d4 <= 6; ++d4) {
                    for (int d5 = d4; d5 <= 6; ++d5) {
                        configurations.push_back({d1, d2, d3, d4, d5});
                        // std::cout << d1 << "," << d2 << "," << d3 << "," << d4 << "," << d5 << std::endl;
                    }
                }
            }
        }
    }
    
    return configurations;
}


int Game::dicePosition(std::vector<int> dice) {
    // Looking at positions 0-4
    // Algorithm to decide the position in the dice config array

    //just iterate through the json until you reach the dice config
    std::string roll = stringifyDice(dice);
    int count = 0;
    for (auto it = diceOdds["5"].begin(); it!= diceOdds["5"].end(); ++it) //iterate through the json
        if (it.key()==roll) //if the key (roll) is our roll, return the position
            return count;
        else
            count++; //otherwise continue
    return -1;
}


std::string Game::stringifyDice(std::vector<int> dice) {
    std::string results;
    results.resize(5);
    for (int i = 0; i<5; i++)
        results[i] = '0' + dice[i];
    return results;
}


std::vector<std::vector<int>> Game::scoreCardConfigHalf() {
    std::vector<std::vector<int>> config;
    int a,b,c,d,e,f, g;
    for ( a = 0; a < 2; a++)
        for ( b = 0; b < 2; b++)
            for ( c = 0; c < 2; c++)
                for ( d = 0; d < 2; d++)
                    for ( e = 0; e < 2; e++)
                        for ( f = 0; f < 2; f++)
                            for ( g = 0; g < 2; g++) {
                                config.push_back({a,b,c,d,e,f,g});
                            }
    return config;
}   


int Game::scoreCardPositionHalf(std::vector<int> state) {
    // Looking at positions in the state: 12-18
    int position = 0;
    int place = 1;
    for (int i = 18; i > 11; i--) {
        if (state[i])
            position += place;
        place*=2;
    }
    return position;  


}

// Returns a 96,768 x 13 array of all state space configurations for the lower section only

std::vector<std::vector<int>> Game::stateSpaceHalf() {
   // distribution of spaces for state space i:
        // i % 252 == dice config
        // i/252 % 3 == rolls left
        // i / (252*3) == scorecard config

    std::vector<std::vector<int>> space, diceConfig = diceConfigurations(), scorecardConfig = scoreCardConfigHalf();

    space.resize(3*252*128); // 3 rolls, 252 dice, scorecard configurations

    for (int i = 0; i < space.size(); i++) {
        space[i].resize(19);
            // 0 - 4 is dice
            // 5 is rolls left
            // 6 - 11 is taken upper section
            // 12 - 18 is lower section
        for (int j = 0; j < 5; j++)
            space[i][j] = diceConfig[i%252][j];
        space[i][5] = (i / 252) % 3;
        for (int j = 6; j < 12; j++)
            space[i][j] = 1;
        for (int j = 12; j < 19; j++) 
            space[i][j] = scorecardConfig[(i/(252*3)%128)][j-12];
        
            

        
    }

    return space;
}


// Return the reward sent action (the only relevent information is the action taken in the 5th column of actions, the scorecard action)
// Assume we are in the desired state

int Game::reward(int action5) {
    if (action5 != -1)
        action5 -= 6;

    switch (rewardMode) {
        case 0:
            if ((action5 == -1)) {
                return 0;
            } else {
                std::vector<int> scores = possibleScores();
                return scores[action5]; //since the scores array goes from 0 - 12, subtract 6 and return that
            }
            break;
        case 1:
            if ((action5 == -1)) {
                return -1;
            } else {
                std::vector<int> scores = possibleScores();
                if (scores[action5] == 0)
                    return -10;
                return scores[action5]; //since the scores array goes from 0 - 12, subtract 6 and return that
            }
            break;
        case 2:
            // The following reward
                // rerolling --> always 0
                // with nonzero score
                    // tok  sumOfDice - 1
                    // fok sumOfDice
                    // fh 25
                    // ss 30
                    // ls 40
                    // y 50
                    // chance -1 
                // for scoring 0 in a box:
                    // tok -10
                    // fok -7
                    // fh -10
                    // ss -10
                    // ls  -7
                    // y - 4
            if (action5 == -1) {
                return 0;
            } else {
                std::vector<int> scores = possibleScores();
                if (scores[action5] == 0)
                    switch (action5) {
                        case 6: //three of a kind
                            return -10;
                            break;
                        case 7: //four of a kind
                            return -7;
                            break;
                        case 8: //full house
                            return -10;
                            break;
                        case 9: //small straight
                            return -10;
                            break;
                        case 10: //large straight
                            return -7;
                            break;
                        case 11: // yahtzee
                            return -4;
                            break;
                    }
                else 
                    switch (action5) {
                        case 6: //three of a kind
                            return scores[action5] - 1; // we want to prioritize three of a kind. Chance will also be the last one we score in always
                            break;
                        case 7: //four of a kind
                            return scores[action5];
                            break;
                        case 8: //full house
                            return scores[action5];
                            break;
                        case 9: //small straight
                            return scores[action5];
                            break;
                        case 10: //large straight
                            return scores[action5];
                            break;
                        case 11: // yahtzee
                            return scores[action5];
                            break;
                        case 12: // chance
                            return -1;
                            break;
                    }
            }
            break;
        case 3: 
            // reroll --> 0
            // score 0 --> -1
            // score in the order:
                // four of a kind if avail (- 0)
                // three of a kind (- 1)
                // chance (-2)
            if (action5 == -1)
                return 0;
            else {
                std::vector<int> scores = possibleScores();
                if (scores[action5] == 0)
                    return -1;
                if (action5 == 6) // three of a kind
                    return scores[action5] - 1;
                if (action5 == 12)
                    return scores[action5] - 2;
                return scores[action5];
            }
            break;
    }
    return 0;
}




void Game::goToState(std::vector<int> target) {
    for (int i = 0; i < 5; i++)
        dice[i] = target[i];
    for (int i = 0; i < 19; i++)
        state[i] = target[i];
}


int Game::findState() {
    int scPos=scoreCardPositionHalf(state);
    int dPos = dicePosition(dice);
    int rlPos = state[5];

    return 252*3*scPos + 252*rlPos + dPos;


}


void Game::transitionHalf(std::vector<int> state, std::vector<int>& action, std::vector<std::tuple<int,float>>& transition) {
    // Where to find the state based on properties:
        //252*3 * scorecardPosition(half) + 252*rolls left + Position

    transition.clear();
    int scPos;

    // If the action is to roll again
    
    if (action[5] == -1) {
        // initialize needed vars
        scPos = scoreCardPositionHalf(state);

        // determine how many dice are being rolled
        std::vector<int> kept;
        for (int i = 0; i < 5; i++)
            if (action[i]) 
                kept.push_back(state[i]);
            
        
        // If the decision is to keep all 5 dice
        if (kept.size() == 5) {
            transition.push_back(std::make_tuple(252*3*scPos + 252*(state[5]-1) + dicePosition(kept), 1.0));
            return;
        }


        std::string rolled;
        rolled.resize(1);
        rolled[0] = '5' - kept.size();

        std::vector<int> roll;
        roll.resize(5);
        int count = 0;
        // create all rolls with the remaining dice, add the kept dice to the end, with corresponding odds
        //go through each roll and return the position where that state is found, and the corresponding odds
        for (auto it = diceOdds[rolled].begin(); it!= diceOdds[rolled].end(); ++it) {
            // add the it roll with the kept dice
            for (int i = 0; i < kept.size(); i++)
                roll[i] = kept[i];
            
            for (int i = kept.size(); i < 5; i++)
                roll[i] = it.key()[i-kept.size()] - '0';

            // sort the roll
            std::sort(roll.begin(),roll.end());

            // add the tuple with the enumerated position and the corresponding odds to the transition vector
            transition.push_back(std::make_tuple(252*3*scPos + 252*(state[5]-1) + dicePosition(roll), it.value()));

        }

        //return the transition array
        return;
    }

    else 

    // If the action is to score

    {
        // initialize needed vars

        //
        state[action[5]] = 1;
        scPos = scoreCardPositionHalf(state);
        transition.resize(252);
        int dicePosition = 0;


        // go through all rolls of 5 dice in the diceOdds json
        for (auto it = diceOdds["5"].begin(); it!= diceOdds["5"].end(); ++it) {

            // add the enumerated position and the corresponding odds to the transition vector
            transition[dicePosition] = std::make_tuple(252*3*scPos + 252*2 + dicePosition, it.value()); 

            //next dice position
            dicePosition++;
        }

        //return the transition vector

        return;

    }

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


bool Game::rollsLeft()
{
    return state[5] > 0;
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

std::vector<int> Game::resetHalf() { 

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
    // Set all of the upper section to taken (1) 
    for ( int i = 6 ; i < 12 ; i ++ ) { 
        state[ i ] = 1 ; 
    } 

    // Set all lower section to 0

    for ( int i = 12 ; i < 19 ; i ++ ) { 
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

        // fill dice keep with ones, conceptually we are keeping all 5 dice but in reality it does not matter

        for (int j = 0; j < 5; j++) 
            actions[i][j] = 1;
        // std::cout << "1 1 1 1 1 -- ";
        //next available square

        while (state[next] == 1)
            next++;
        actions[i][5] = next;
        // std::cout << next << std::endl;
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
                // std::cout << temp%2 << " ";
                temp /= 2;
            }
            // std::cout << "-- -1" << std::endl;
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

void Game::printScorecard() const{
    std::cout << "Scorecard: [ ";
    for (int i = 6; i < 19; ++i) {
        std::cout << stringifyCategory(i-6) << " " << state[i] << " | ";
        if (i == 11)
            std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
}


void Game::printState() const {
    std::cout << "State: " << std::endl;
    printDice();
    std::cout << "Rolls Left: " << state[5] << std::endl;
    printScorecard();
    std::cout << std::endl;
}


std::string Game::stringifyCategory(int cat) const {
    std::vector<std::string> cats = {"Ones","Twos","Threes","Fours","Fives","Sixes","Three of A Kind","Four of a Kind","Full House","Small Straight","Large Straight","Yahtzee","Chance"};
    return cats[cat];
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
        sum += dice[i];
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
                (count[2] && count[3] && count[4] && count[5]); 
}

bool Game::isLargeStraight() const {
    return (dice[0] == 1 && dice[1] == 2 && dice[2] == 3 && dice[3] == 4 && dice[4] == 5) ||
            (dice[0] == 2 && dice[1] == 3 && dice[2] == 4 && dice[3] == 5 && dice[4] == 6);
}

bool Game::isYahtzee() const {
    return std::all_of(dice.begin(), dice.end(), [&](int die) { return die == dice[0]; });
}





