# CS 499 - Yahtzee. 

# Instructions for compiling and running the project. 

### Tabular Q-Learning. 

I have developed, and tested the code using the flip servers, so it definitely works there and instructions are for compiling and running the project on flip. 
I have used the college of engineering interactive desktops, to train it, because I can log off from them, and the process is not killed, therefore the training continues. I did this because I had a lot of trouble getting the python-c++ bindings work submitting the trainings as a job to the DGX machines. 

1. Create a virtual environment. 
2. Activate it. 
3. Install pybind. 
4. Compile the bindings. (g++ -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` game/game_2.cpp pybind/game_bindings.cpp -o test/game_module`python3-config --extension-suffix`) 
5. Change directory to test directory. 
6. Start the training. (python tabular_q_learning.py) 

1. Actually, if I just clone it from GitHub, it works, it does not require to compile it again. It might be because the bindings are already in the repository, however for dgx-2, (interactive desktop) this never works. I think I have pybind installed on flip, that is probably why, I guess also the python version matters. 
2. Actually, it also works on dgx-2, I think one of the compiled bindings are for flip, and the other is for dgx-2. 

1. tabular_q_learning.py. (train, test etc.)
2. lat_just_lower.py. (Load and Test just lower, load the q_table, complete 1 episode, print the episode, print the score of the agent.)
3. lat_not_just_lower. (Load and Test not just lower, load the q_table, complete 1 episode, print the episode, print the score of the agent.) 

