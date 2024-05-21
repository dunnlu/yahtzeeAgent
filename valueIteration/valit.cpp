#include "../game/game.h"

#include <algorithm>
#include <iostream>


// Returns (value, policy)
std::tuple<std::vector<int>,std::vector<int>> valit(float epsilon, int max_iters) {
    Game game;

    std::vector<std::vector<int>> space = game.stateSpaceHalf();

    std::vector<std::vector<int>> actions;
    
    std::vector<int> policy;
    std::vector<int> value;
    std::vector<int> tempValue;

    std::vector<std::tuple<int,float>> transition;

    int best_action, curr_iters;
    float best_value, temp_value, residual, max_residual;

    curr_iters = 0;

    policy.resize(space.size());
    value.resize(space.size());
    tempValue.resize(space.size());
    transition.resize(252);

    for (int v = 0; v < value.size(); v++)
        value[v] = 0;


    while (curr_iters < max_iters) {
        std::cout << "Iteration " << curr_iters << std::endl;
        max_residual = 0;
        for (int s = 0; s < space.size(); s++) {
            std::cout << "State " << s << std::endl;
            game.goToState(space[s]);
            actions = game.possibleActions();
            best_value = 0;
            best_action = 0;
            for (int a = 0; a < actions.size(); a++) {
                // std::cout << "Action " << a << " [5] = " << actions[a][5] << std::endl;
                temp_value = game.reward(actions[a][5]);
                // std::cout << "Got reward: " << temp_value << std::endl;
                game.transitionHalf(space[s],actions[a],transition);
                // std::cout << "Got transition" << std::endl;
                for (int t = 0; t < transition.size(); t++) {
                    // std::cout << "Transition " << t << std::endl;
                    temp_value += std::get<1>(transition[t]) * value[std::get<0>(transition[t])];
                }
                if (temp_value > best_value) {
                    best_value = temp_value;
                    best_action = a;
                } 
            }
            tempValue[s] = best_value;
            policy[s] = best_action;
            residual = abs(best_value - value[s]);
            max_residual = std::max(max_residual,residual);

            game.printState();
            std::cout << "Value: " << best_value << std::endl;
            std::cout << "Action: " << actions[best_action][5] << std::endl;
            std::cout << std::endl;
        }
        value = tempValue;

        // Exit Clause
        if (max_residual < epsilon)
            break;
    }

    std::cout << value[0] << std::endl;

    return std::make_tuple(value, policy);
}


int main() {
    valit(0.1,1);


    return 0;
}



// Current Issue :
    // Something to do with storing policy as a 2d vector
    // Something to do with updating policy