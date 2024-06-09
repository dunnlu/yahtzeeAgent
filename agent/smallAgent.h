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
    initializes the fullStateSpace --> initializes the fullStateSpace to 0 value


*/
using namespace std;


class SmallAgent {
    private:
        vector<int> stateSpace; // 0 - rolls left, 1-5 - dice, 6-12 - scoresheet
        vector<vector<int>> fullSS //length 13, one array for each feature
        int reward; 
        int actionSpace; //2^k - keep dice k for k e [0,4], store score in scoresheet slot k for k e [5,11]


    public:
        SmallAgent();

        int step(int action); //takes action, stores new statespace, returns reward
        void initializeFSS();
        void reset();

}



#endif