cmake_minimum_required(VERSION 3.15)
project(Prova_1234_)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS thread REQUIRED)
link_libraries(Boost::thread)
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
        rd.cpp
        rd.hpp
        Solution.cpp
        Solution.hpp
        Utility.cpp
        Utility.hpp
        TimeController.cpp
        TimeController.hpp)
