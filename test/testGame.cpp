#include <iostream>
#include "../game/game.h"

int main(){
    Game game;
    
    std::cout << "created game" << std::endl;
    
    std::vector<std::vector<int>> space = game.stateSpaceHalf();

    std::cout << "created space" << std::endl;
    

    //generate max reward in each state
    std::vector<std::vector<int>> actions;

    std::cout << "created actions" << std::endl;
    

    for (int i = 0; i < space.size(); i++){
        game.goToState(space[i]);

        std::cout << "went to state " << i << std::endl;
    

        actions = game.possibleActions();

        std::cout << "copied actions" << std::endl;
    



        for (int j = 0; j < actions.size(); j++) {
            std::cout << i << ", " << j << ": " << game.reward(actions[j][5]) << std::endl;


        }
    }
        
}
