#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../game/game.h"


// void writeVectorToFile(const std::vector<float>& vec, const std::string& filename) {
//     std::ofstream outFile(filename, std::ios::binary);
//     if (!outFile) {
//         std::cerr << "Error opening file for writing: " << filename << std::endl;
//         return;
//     }

//     size_t size = vec.size();
//     outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
//     outFile.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(float));
//     outFile.close();
// }

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


// std::vector<float> readVectorFromFile(const std::string& filename) {
//     std::ifstream inFile(filename, std::ios::binary);
//     if (!inFile) {
//         std::cerr << "Error opening file for reading: " << filename << std::endl;
//         return {};
//     }

//     size_t size;
//     inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
    
//     std::vector<float> vec(size);
//     inFile.read(reinterpret_cast<char*>(vec.data()), size * sizeof(float));
//     inFile.close();
    
//     return vec;
// }

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
std::tuple<std::vector<float>,std::vector<int>> valit(float epsilon, int max_iters) {
    Game game;

    std::vector<std::vector<int>> space = game.stateSpaceHalf();

    std::vector<std::vector<int>> actions;
    
    std::vector<int> policy;
    std::vector<float> value;
    std::vector<float> tempValue;

    std::vector<std::tuple<int,float>> transition;

    int best_action, curr_iters;
    float best_value, temp_value; //, residual, max_residual;

    curr_iters = 0;

    policy.resize(space.size());
    value.resize(space.size());
    tempValue.resize(space.size());
    transition.resize(252);

    for (int v = 0; v < value.size(); v++)
        value[v] = 0;


    while (curr_iters < max_iters) {
        std::cout << "Iteration " << curr_iters << std::endl;
        // max_residual = 0;
        for (int s = 0; s < space.size(); s++) {
            if (s%1000 == 0)
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
            // residual = abs(best_value - value[s]);
            // max_residual = std::max(max_residual,residual);

            // game.printState();
            // std::cout << "Value: " << best_value << std::endl;
            // std::cout << "Action: " << actions[best_action][5] << std::endl;
            // std::cout << std::endl;
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

        // Exit Clause
        // if (max_residual < epsilon)
        //     break;
    }

    // std::cout << max_residual << std::endl;

    return std::make_tuple(value, policy);
}


int main() {
    valit(0.1,20);

    // std::vector<float> value = readVectorFromFile("valitFiles/valit1.txt");

    // for (int i = 0; i < 100; i++)
    //     std::cout << i << " | " << value[i] << std::endl;


    return 0;
}


