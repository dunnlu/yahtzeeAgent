#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <cmath>   // For std::sqrt

#include "../game/game.h"

#define evaluatePolicies true  // If true, the policies in the chosen valitFolder will be tested

#define numEps 1000             // Number of episodes to test for each iteration in the valitFolder

#define numPol 17                // Number of policies in the valitFolder to test

/*
    Recommended Numbers for Each valitFolder:
        - valitFilesExample         4
        - valitFilesCommonHuman     22
        - valitFilesMinimalistic    17

*/

#define human false             // If true, the user will play the game

#define valitFolder "valitFilesMinimalistic" 
/*
    Options:
        - valitFilesExample 
        - valitFilesCommonHuman
        - valitFilesMinimalistic
*/

#define showPolicy 16            // -1 to show no policy

/*
    Recommended Numbers for Each valitFolder:
        - valitFilesExample         3
        - valitFilesCommonHuman     21
        - valitFilesMinimalistic    16

*/

#define numShownGames 0         // Number of games shown for the chosen policy



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



float testPolicy(Game& game, std::vector<std::vector<int>>& space, std::vector<int>& policy) {
    // initialize vars
    float localSum, sum=0;


    std::vector<int> action;




    std::tuple<std::vector<int>,int,bool> reward;
    
    
    // loop for numEp episodes
    for (int i = 0; i < numEps; i++) {
        // reset game and sum
        game.resetHalf();
        localSum = 0;

        // play until terminal
        while(!game.isTerminal()){
            
            // enumerate state
            int pos = game.findState();

            // decide the action

            action = game.possibleActions()[policy[pos]]; // store the action at the index of possible actions which is stored in the policy for the state we are in

            // fix action
            if (action[5] != -1)
                action[5] -= 6;


            // take a step, according to the policy
            reward = game.step(action);


            // add to sum
            localSum += std::get<1>(reward);

        }

        sum += localSum;
        

    }

    // return the mean and variance
    float mean = sum / numEps;
    return mean;


}


std::vector<int> policyFromFile(int p) {
    std::stringstream ss;
    ss << "../valueIteration/" << valitFolder << "/polit" << p << ".txt";
    std::string filename = ss.str();

    return readVectorFromFileInt(filename);
}


// Step through game to show what each policy would choose, and take an action myself to decide
void policyShowGame(Game& game, std::vector<std::vector<int>>& space, std::vector<int>& policy) {
    // initialize vars
    float sum=0;


    std::vector<int> action;




    std::tuple<std::vector<int>,int,bool> reward;
    
    
    
    // reset game and sum
    game.resetHalf();

    // play until terminal
    while(!game.isTerminal()){
        // std::cout << "Next" << std::endl;
        std::cout << "Score: " << sum << std::endl;
        game.printState();

        // enumerate state
        int pos = game.findState();

        // decide the action

        action = game.possibleActions()[policy[pos]]; // store the action at the index of possible actions which is stored in the policy for the state we are in

        // fix action
        if (action[5] != -1)
            action[5] -= 6;

        std::cout << "ACTION " << std::endl;
        if (action[5] == -1) {
            for (int j = 0; j < 4; j++)
                std::cout << action[j] << " | ";
            std::cout << action[4] << std::endl;
        } else
            std::cout << game.stringifyCategory(action[5]) << std::endl;
        std::cout << std::endl;

        // take a step, according to the policy
        reward = game.step(action);


        // add to sum
        sum += std::get<1>(reward);

    }
    
    std::cout << "Final Score: " << sum << std::endl << std::endl;  


}




// Run through 10 games with a human to see a human's average score (more variance obviously)
float humanTest(Game& game) {
    float localSum, sum=0;


    std::vector<int> action;
    action.resize(6);

    std::vector<std::vector<int>> actions;

     std::string wait;




    std::tuple<std::vector<int>,int,bool> reward;
    
    
    // loop for numEp episodes
    for (int i = 1; i < 11; i++) {
        std::cout << "Game " << i << std::endl;
        // reset game and sum
        game.resetHalf();
        localSum = 0;

        // play until terminal
        while(!game.isTerminal()){
            // std::cout << "Next" << std::endl;
            // game.printState();

            game.printState();

            // decide the action
            std::cout << "Score so far: " << localSum << std::endl;

            std::cout << std::endl;
           



            if (game.rollsLeft()) {
                std::cout  << "Type '11111' to save all 5 dice, or '10101' to save the first, third and fifth, etc. \nYou must save as there are rolls left, but if you would like to save these dice and score just type '11111' until you can score" << std::endl; 
                std::cin >> wait;
                while (wait.length()<5 || wait.length() > 5) 
                    std::cin >> wait;

                for (int j = 0; j < 5; j++)
                    action[j] = wait[j] - '0';
                action[5] = -1;

                std::cout << "You chose: " << std::endl;
                for (int k = 0; k < 5; k++)
                    std::cout << action[k] << " | ";
            }
            else {
                actions = game.possibleActions();
                for (int j = 0; j < actions.size(); j++)
                    if (actions[j][5] != -1)
                        std::cout << actions[j][5] << " " << game.stringifyCategory(actions[j][5]-6) << " | ";
                std::cout << std::endl << "Type the number corresponding to the category you would like to save to" << std::endl;
                std::cin >> wait;
                while(wait.length()>2)
                    std::cin >> wait;
                action[5] = wait[1] - '0' + 10;

                std::cout << "You chose: " << std::endl;
                std::cout << game.stringifyCategory(action[5]-6) << std::endl;

            }

            std::cout << std::endl;
            std::cout << std::endl;


            // fix action
            if (action[5] != -1)
                action[5] -= 6;


            // take a step, according to the policy
            reward = game.step(action);


            // game.printState();


            // add to sum
            localSum += std::get<1>(reward);

        }

        sum += localSum;

        std::cout << "Game Score :" << localSum << std::endl;
        std::cout << "Running Average: " << sum/(i) << std::endl;

        

    }

    // return the mean and variance
    float mean = sum / 10;
    return mean;
}



int main() {
    // Read in the state space
    Game game;


    std::vector<float> means;
    int size = numPol+1;

    means.resize(size);


    // Test on human play
    if (human) {
        size++;
        means.resize(size);
        means[numPol+1] = humanTest(game);
    }
    
    

    std::vector<std::vector<int>> space = game.stateSpaceHalf();



    // Generate a random policy
    std::srand(std::time(0));
    
    std::vector<std::vector<int>> actions;
    std::vector<int> randomPolicy;

    randomPolicy.resize(space.size());

    for (int s = 0; s < space.size(); s++) {
        game.goToState(space[s]);
        actions = game.possibleActions();
        if (!game.isTerminal())
            randomPolicy[s] = std::rand() % actions.size();

    }


    // Test policies

    std::vector<int> policy;
    policy.resize(space.size());

    // Show a runthrough of the chosen policy
    if (showPolicy != -1) {
        policy = policyFromFile(showPolicy);
        for (int i = 0; i < numShownGames; i++) 
            policyShowGame(game,space,policy);
    }




    // Test numEps episodes of testing policy
    if (!evaluatePolicies) // If you dont want to evaluate, just exit
        exit(0);


    for (int i = 0; i < numPol; i++) {
        policy = policyFromFile(i);
        means[i] = testPolicy(game,space,policy);
    }

    // Test numEps episodes of random policy

    means[numPol] = testPolicy(game,space,randomPolicy);




    // Compare and print the average accumulated reward for each

    std::cout << "Random, " << means[numPol] << std::endl;
    if (human)
        std::cout << "Human, " << means[numPol+1] << std::endl;
    for (int i = 0; i < numPol; i++)
        std::cout  << i << ", " << means[i] << std::endl;

    return 0;

}