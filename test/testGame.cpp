#include <iostream>
#include "../game/game.h"

int main(){
    Game game;
    
    std::vector<std::vector<int>> space = game.stateSpaceHalf();
    

    //generate max reward in each state
    std::vector<std::vector<int>> actions;
    
    std::vector<int> policy;
    std::vector<int> value;

    policy.resize(space.size());
    value.resize(space.size());

    for (int i = 0; i < space.size(); i++){
        game.goToState(space[i]);

        

        game.printState();

        std::vector<int> scores = game.possibleScores();
        for (int j = 0; j < 13; j++) { 
            std::cout << game.stringifyCategory(j) << " " << scores[j] << " | ";
        }
        std::cout << std::endl;
    

        actions = game.possibleActions();

    

        int best_reward = 0;
        int best_action;

        for (int j = 0; j < actions.size(); j++) {
            if (game.reward(actions[j][5]) > best_reward) {
                best_reward = game.reward(actions[j][5]);
                best_action = actions[j][5];
            }



        }

        policy[i] = best_action;
        value[i] = best_reward;

        std::cout << "Best Move: " << policy[i] << " --> " << value[i] << std::endl;
        std::cout << std::endl;

    }



        
}
