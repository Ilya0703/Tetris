#include "/Users/mac/tetris/pybind11/pybind11.h"
#include "/Users/mac/tetris/library.cpp"
#include "/Users/mac/tetris/pybind11/stl.h"


PYBIND11_MODULE(library, m) {
    pybind11::class_<tetris>(m, "tetris")
        .def(pybind11::init<>())
        .def(pybind11::init<std::vector<std::vector<int>>, std::string>())
        .def("update_matrix", &tetris::update_matrix)
        .def("get_matrix",&tetris::get_matrix)
        .def("get_flag_finish_game",&tetris::get_flag_finish_game)
        .def("preprocess_action_str",&tetris::preprocess_action_str);
}
