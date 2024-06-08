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
I have used the college of engineering interactive desktops, to train it, because I can log off from them, and the process is not killed, therefore the training continues. I did this because I had a lot of trouble getting the python-c++ bindings work submitting the trainings as a job to the DGX machines. The instructions will assume the use of flip. 

To compile + run:
Navigate to the root directory of this project.
Install dependencies. 

    pip install pybind

Compile the python bindings. 

    g++ -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` game/game_2.cpp pybind/game_bindings.cpp -o test/game_module`python3-config --extension-suffix`

Navigate to test folder. 

    cd test/

Running the tabular_q_learning.py will start the training, and will print out the eval results for each batch, with the time relative to the start of the training. 

    python tabular_q_learning.py 

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

