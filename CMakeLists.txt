cmake_minimum_required(VERSION 3.15)
project(Prova_1234_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(kempegroup)
include_directories(LocalSearchGroup)
include_directories(tabugroup)

add_executable(Prova_1234_
    kempegroup/Kempe.cpp
    kempegroup/Kempe.hpp
    LocalSearchGroup/LocalSearch.cpp
    LocalSearchGroup/LocalSearch.hpp
    tabugroup/Tabu.cpp
    tabugroup/Tabu.hpp
    ColorShift.cpp
    ColorShift.hpp
    graphw.hpp
    InitialSolver.cpp
    InitialSolver.hpp
    main.cpp
    OptimalSolver.cpp
    OptimalSolver.hpp
    rd.cpp
    rd.hpp
    Solution.cpp
    Solution.hpp
    trolley.cpp
    trolley.h
    Utility.cpp
    Utility.hpp)
