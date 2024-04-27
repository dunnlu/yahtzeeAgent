#include <pybind11/pybind11.h>
#include "game.h"  // Ensure this includes the definition of your Game class

namespace py = pybind11;

PYBIND11_MODULE(game_module, m) {  // The name here, 'game_module', is what you'll import in Python
    py::class_<Game>(m, "Game")
        .def(py::init<>())  // Binding the constructor
        .def("rollDice", &Game::rollDice)  // Binding the rollDice method
        .def("printDice", &Game::printDice)  // Binding the printDice method
        // Add other methods as necessary
        ;
}
