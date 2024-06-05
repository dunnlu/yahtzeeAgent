#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../game/game.h"



void writeVectorToFile(const std::vector<float>& vec, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    outFile << vec.size() << "\n";  // Write the size of the vector
    for (const float& val : vec) {
        outFile << val << "\n";  // Write each float value on a new line
    }
    outFile.close();
}

void writeVectorToFileInt(const std::vector<int>& vec, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    outFile << vec.size() << "\n";  // Write the size of the vector
    for (const int& val : vec) {
        outFile << val << "\n";  // Write each float value on a new line
    }
    outFile.close();
}


std::vector<float> readVectorFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return {};
    }

    size_t size;
    inFile >> size;  // Read the size of the vector

    std::vector<float> vec(size);
    for (size_t i = 0; i < size; ++i) {
        inFile >> vec[i];  // Read each float value
    }
    inFile.close();

    return vec;
}


std::vector<int> readVectorFromFileInt(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return {};
    }

    size_t size;
    inFile >> size;  // Read the size of the vector

    std::vector<int> vec(size);
    for (size_t i = 0; i < size; ++i) {
        inFile >> vec[i];  // Read each float value
    }
    inFile.close();

    return vec;
}

// Returns (value, policy)
std::tuple<std::vector<float>,std::vector<int>> valit(int starting_iters, int max_iters) {
    Game game;

    std::vector<std::vector<int>> space = game.stateSpaceHalf();

    std::vector<std::vector<int>> actions;
    
    std::vector<int> policy;
    std::vector<float> value;
    if (starting_iters==-1)
        for (int v = 0; v < value.size(); v++)
            value[v] = 0;
    else {
        std::stringstream ss3;
        ss3 << "valitFiles/valit" << starting_iters << ".txt";
        std::string filename = ss3.str();
        value = readVectorFromFile(filename);
    }

    std::vector<float> tempValue;

    std::vector<std::tuple<int,float>> transition;

    int best_action, curr_iters;
    float best_value, temp_value; 

    curr_iters = starting_iters+1;

    policy.resize(space.size());
    value.resize(space.size());
    tempValue.resize(space.size());
    transition.resize(252);


    while (curr_iters < max_iters) {
        std::cout << "Iteration " << curr_iters << std::endl;
        // max_residual = 0;
        for (int s = 0; s < space.size(); s++) {
            if (s%1000 == 0)
                std::cout << "State " << s << std::endl;
            game.goToState(space[s]);

            if (game.isTerminal()) {
                tempValue[s] = 0;
                policy[s] = -1;
            } else {

                actions = game.possibleActions();
                best_value = 0;
                best_action = 0;
                for (int a = 0; a < actions.size(); a++) {
                    temp_value = game.reward(actions[a][5]);
                    game.transitionHalf(space[s],actions[a],transition);
                    for (int t = 0; t < transition.size(); t++) {
                        temp_value += std::get<1>(transition[t]) * value[std::get<0>(transition[t])];
                    }
                    if (temp_value > best_value) {
                        best_value = temp_value;
                        best_action = a;
                    } 
                }
                tempValue[s] = best_value;
                policy[s] = best_action;
            }
        }
        value = tempValue;

        std::stringstream ss;
        ss << "valitFiles/valit" << curr_iters << ".txt";
        std::string filename = ss.str();
        writeVectorToFile(value,filename);

        std::stringstream ss2;
        ss2 << "valitFiles/polit" << curr_iters << ".txt";
        std::string filename2 = ss2.str();
        writeVectorToFileInt(policy,filename2);

        curr_iters++;
    }

    return std::make_tuple(value, policy);
}


int main() {
    valit(3,4); 


    return 0;
}

 
