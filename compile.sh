#!/bin/bash
g++ -std=c++11 -O3 -Wall -shared -fPIC `python3 -m pybind11 --includes` game/game_2.cpp pybind/game_bindings.cpp -o test/game_module`python3-config --extension-suffix` 
