#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <tuple> 
#include <cmath>
#include <string>

#include "../game/game.h"
#include "evmc.h"

//returns a vector of tuples of (state, action, reward)
std::vector<std::tuple<std::vector<int>,std::vector<int>,int>> generateEpisode() {
    Game game;
    std::vector<std::tuple<std::vector<int>,std::vector<int>,int>> episode;
    std::vector<int> state;
    std::vector<int> action;

    int reward;
    bool terminal = false;

    state = game.reset();
    while (!terminal) {
        action = game.possibleActions()[rand() % game.possibleActions().size()];
        std::tie(state, reward, terminal) = game.step(action);
        episode.push_back(std::make_tuple(state, action, reward));
    }

    return episode;

}

int main(){
    Game game;

    //[[dice 1, dice 2, dice 3, dice 4, dice 5, rolls left, upper, upper, upper, upper, upper, upper, 3 of a kind, 4 of a kind, full house, sm straight, lg straight, yahtzee, chance], ...]
    std::vector<std::vector<int>> states = game.stateSpaceHalf();


}