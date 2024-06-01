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







