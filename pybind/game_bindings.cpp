#include <pybind11/pybind11.h>
#include "../game/game_2.h"  // Ensure this includes the definition of your Game class 
#include <pybind11/stl.h>  // Include this to handle std::vector

namespace py = pybind11;

PYBIND11_MODULE(game_module, m) {  // The name here, 'game_module', is what you'll import in Python
    py::class_<Game>(m, "Game")
        .def(py::init<>())  // Binding the constructor
        // .def("rollDice", &Game::rollDice)  // Binding the rollDice method
        .def("printDice", &Game::printDice)  // Binding the printDice method
        // Add other methods as necessary 
        .def("step", &Game::step)  // Binding the printDice method
        .def("isTerminal", &Game::isTerminal)  // Binding the printDice method
        .def("reset", &Game::reset)  // Binding the printDice method 
        .def("resetHalf", &Game::resetHalf)  // Binding the printDice method
        .def("goToState", &Game::goToState) // binding hte
        ;
}
