# CS 499 - Yahtzee. 

# Instructions for compiling and running the project. 

### Value Iteration

Instructions included within the folders 'valueIteration' and 'evaluate', in their respective README.md files. 

Simply navigate to valueIteration or evaluate and follow instructions

    cd valueIteration

OR

    cd evaluate


### Tabular Q-Learning. 

I have developed, and tested the code using the flip servers, so it definitely works there and instructions are for compiling and running the project on flip. 
I have used the college of engineering interactive desktops, to train it, because I can log off from them, and the process is not killed, therefore the training continues. I did this because I had a lot of trouble getting the python-c++ bindings work submitting the trainings as a job to the DGX machines. 

From the HPC:

Create a python virtual environment. 

    pip install virtualenv
    pip install virtualenvwrapper-win
    python3 -m venv qlearningEnv

Activate it. 

    source qlearningEnv/bin/activate

Install pybind. 

    pip install pybind

Compile the bindings. 

    g++ -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` game/game_2.cpp pybind/game_bindings.cpp -o test/game_module`python3-config --extension-suffix`

Change directory to test directory. 

    cd test/

Start the training. 

    python tabular_q_learning.py


Actually, if I just clone it from GitHub, it works, it does not require to compile it again. It might be because the bindings are already in the repository, however for dgx-2, (interactive desktop) this never works. I think I have pybind installed on flip, that is probably why, I guess also the python version matters. 

Actually, it also works on dgx-2, I think one of the compiled bindings are for flip, and the other is for dgx-2. 

1. tabular_q_learning.py. (train, test etc.)
2. lat_just_lower.py. (Load and Test just lower, load the q_table, complete 1 episode, print the episode, print the score of the agent.)
3. lat_not_just_lower. (Load and Test not just lower, load the q_table, complete 1 episode, print the episode, print the score of the agent.) 


### Every Visit Monte Carlo.

The code was developed and tested on OSU HPC eecs partition and dgx2 partition, so it definetly works there. Compiling and running the project on eecs partition and dgx2 partition.

To compile + run:
Navigate to the root directory of this project.
Install dependencies

    pip install pybind

Compile the python bindings 

    g++ -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` game/game.cpp pybind/game_bindings.cpp -o test/game_module`python3-config --extension-suffix`

Navigate to test folder

    cd test/

evmc.py is used to specify the hyperparameters and train (learned Q function will be saved to a .pkl file)

    python evmc.py

evmc_eval.ipynb is used to load the saved Q function and use it to evalue the performance, create example runs, etc.

