### Evaluate
This function is intended to specifically evaluate the results from value iteration. If you have not run value iteration, this will only test a random policy and a human player (you), if the setting 'human' is set to true. 

The function simply runs through n episodes and takes the average score from each policy which is tested. Output is in csv format. 

The human playthrough is admittedly fragile, and some specific wrong human input will lead to errors which may not be completely apparrent in the gameplay itself.

### Execution
To compile:

    make

To run

    ./evaluate

Must have diceOdds.json, evaluate.o, and game.o as well as the execution file present in order to run.

### SETTINGS: 
(at the top of the evaluate.cpp file, requires re-make-ing to apply) 

evaluatePolicies --> If true, the policies in the chosen valitFolder will be tested
numEps --> Number of episodes for random and tested policies NOT human
numPol --> Number of policies tested. If you are testing up to 'polit21.txt' then you would input '22' 
human --> True if you are human testing. False if you are just testing random and the tested policies. Always 10 episodes. 
valitFolder --> The folder in ../valueIteration to be referenced by the function
showPolicy --> An int, the iteration within the selected folder to show (-1 to show none)
numShownGames --> An int, the number of games of the selected policy to be shown
