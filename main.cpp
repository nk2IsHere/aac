#include <iostream>
#include <string>
#include <fstream>

#include "graph_utils.h"
#include "graph_solution_2.h"
#include "graph_solution_2_polynomial_approximation.h"
#include "graph_solution_3.h"
#include "graph_solution_3_optimized_bruteforce.h"
#include "graph_solution_3_polynomial_approximation.h"
#include "graph_solution_4.h"
#include "graph_solution_4_polynomial_approximation.h"
#include "multigraph_generator.h"

const std::string INPUT_FILE_NAME_1 = "mgraph1.txt";
const std::string INPUT_FILE_NAME_2 = "mgraph2.txt";
const std::string INPUT_FILE_NAME_3 = "mgraph3.txt";

// For Multigraphs:
// 	1. define size (practical, reasonable definition)
//	2. distance function (metric), metric space - in the space of multigraphs define a function that is a metric (satisties axioms)
//	3. maximal clique (brute force algo [exp complexity] and its approximation [polynomial complexity])
//	4. maximal common subgraph (of two multigraphs)

std::pair<int, MultigraphAdjacencyMatrix> readGraphFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "Error: cannot open file " << filename << std::endl;
        throw std::runtime_error("Cannot open file");
    }

    auto [numberOfVertices, multigraph] = readGraph(inputFile);
    inputFile.close();

    return {numberOfVertices, multigraph};
}

void writeGraphToFile(const std::string& filename, const MultigraphAdjacencyMatrix& multigraph) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cout << "Error: cannot open file " << filename << std::endl;
        throw std::runtime_error("Cannot open file");
    }

    writeGraph(outputFile, multigraph);
    outputFile.close();
}

int main() {
    auto [numberOfVertices1, multigraph1] = readGraphFromFile(INPUT_FILE_NAME_1);
    auto [numberOfVertices2, multigraph2] = readGraphFromFile(INPUT_FILE_NAME_2);
    auto [numberOfVertices3, multigraph3] = readGraphFromFile(INPUT_FILE_NAME_3);

    auto [num_vertices, num_edges] = size(multigraph1);
    std::cout << "Size: " << num_vertices << " " << num_edges << std::endl;

    std::cout << "Running maximal clique bruteforce approach: " << std::endl;
    auto [alphaBruteforce, nBruteforce] = maximalCliqueBruteforce(multigraph1);
    std::cout << "Maximal clique bruteforce: " << alphaBruteforce << "K" << nBruteforce << std::endl;
    std::cout << std::endl;

    std::cout << "Running maximal clique optimized bruteforce approach: " << std::endl;
    auto [alphaBruteforceOptimized, nBruteforceOptimized] = maximalCliqueBruteforceOptimized(multigraph1);
    std::cout << "Maximal clique bruteforce  optimized: " << alphaBruteforceOptimized << "K" << nBruteforceOptimized << std::endl;
    std::cout << std::endl;

    std::cout << "Running maximal clique polynomial approximation: " << std::endl;
    auto [alphaPolynomialApproximation, nPolynomialApproximation] = maximalCliquePolynomialApproximation(multigraph1);
    std::cout << "Maximal clique polynomial approximation: " << alphaPolynomialApproximation << "K" << nPolynomialApproximation << std::endl;
    std::cout << std::endl;

    std::cout << "Running graph edit distance: " << std::endl;
    int graphEditDistanceResult = graphEditDistance(multigraph1, multigraph2);
    std::cout << "Graph edit distance: " << graphEditDistanceResult << std::endl;
    std::cout << std::endl;

    std::cout << "Running graph edit distance polynomial approximation: " << std::endl;
    int graphEditDistancePolynomialApproximationResult = graphEditDistancePolynomialApproximation(multigraph1, multigraph2);
    std::cout << "Graph edit distance polynomial approximation: " << graphEditDistancePolynomialApproximationResult << std::endl;

    std::cout << "Running maximal common submultigraph: " << std::endl;
    auto [selection1, selection2] = maximalCommonSubmultigraph(multigraph2, multigraph3);
    std::cout << "Maximal common submultigraph: " << std::endl;
    std::cout << "Selection 1: ";
    for (const auto vertex: selection1) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::cout << "Selection 2: ";
    for (const auto vertex: selection2) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Running maximal common submultigraph polynomial approximation: " << std::endl;
    auto [selection1PolynomialApproximation, selection2PolynomialApproximation] = maximalCommonSubmultigraphPolynomialApproximation(multigraph2, multigraph3);
    std::cout << "Maximal common submultigraph polynomial approximation: " << std::endl;
    std::cout << "Selection 1: ";
    for (const auto vertex: selection1PolynomialApproximation) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::cout << "Selection 2: ";
    for (const auto vertex: selection2PolynomialApproximation) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    return 0;
}
