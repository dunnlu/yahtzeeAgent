#include "smallagent.h"
#include "../game/game.h"

#include <cmath>


using namespace std;



SmallAgent::SmallAgent() {
    Game game;
    stateSpace.resize(13);
    
}



/*
Action Guide:
    +2^0 --> keep die 0
    +2^1 --> keep die 1 
    ...
    +2^5 --> score in spot 0 (in general, 2^5+k)


*/

int SmallAgent::step(int action) {
    //toggle dice
    bool rollDice = false;
    for (int i = 0; i<5; i++) {
        if (action % 2 == 1) 
            game.toggleKeep[i];
            rollDice = true;
        action =/ 2;
    }
    if (rollDice && game.getRollsLeft()>0) {
        game.rollDice();
        
    }




}