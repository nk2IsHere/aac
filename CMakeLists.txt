cmake_minimum_required(VERSION 3.0)
project(aac_laboratories)

set(CMAKE_CXX_STANDARD 20)

add_executable(aac_laboratories main.cpp
        graph_utils.h
        graph_solution_3.h
        graph_solution_3_optimized_bruteforce.h
        graph_solution_2.h
        graph_solution_4.h
        graph_solution_2_polynomial_approximation.h
        graph_solution_3_polynomial_approximation.h
        graph_solution_4_polynomial_approximation.h
        multigraph_generator.h
        library/clipp.h)
