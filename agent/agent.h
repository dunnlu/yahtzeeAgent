#ifndef agent_h
#define agent_h

#include "../game/game.h"

#include <vector>



//Needs state space
//Needs action space
//Needs transition function
//Needs reset
//Needs reward function



//PUBLIC: 
/*
    step (action) --> statespace, reward (vector representing state, with reward as the last slot)
    render ()
    reset() --> void
    initialize fullStateSpace --> fills the fullStateSpace, creates an array of length 

*/
using namespace std;


class Agent {
    private:
        vector<int> stateSpace; // 0 - rolls left, 1-5 - dice, 6-18 - scoresheet, 19 - rollsToBonus, 20 - value
        vector<vector<int>> fullStateSpace //length (possibilities) only used for value iteration
        int reward; 
        int actionSpace; //see smallAgent


    public:
        vector<int> step(vector<int> action);
        void initializeFullStateSpace()
        void reset();

}



#endif