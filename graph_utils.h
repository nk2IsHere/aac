
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <utility>
#include <chrono>
#include <algorithm>
#include <random>
#include <limits>

#ifndef AAC_LABORATORIES_GRAPH_UTILS_H
#define AAC_LABORATORIES_GRAPH_UTILS_H

using MultigraphAdjacencyMatrix = std::vector<std::vector<int>>;

struct MultigraphSize {
    int numVertices;
    int numEdges;
};

struct CompleteMultigraph {
    int alpha;
    int n;
};

struct CliqueAlgorithmResult {
    CompleteMultigraph completeMultigraph;
    std::vector<int> selection;
};

struct ReadGraphResult {
    int numVertices;
    MultigraphAdjacencyMatrix multigraph;
};

ReadGraphResult readGraph(std::ifstream& inputFile) {
    // Read the number of vertices
    int numVertices;
    inputFile >> numVertices;
    std::cout << "Number of vertices: " << numVertices << std::endl;

    std::vector<std::vector<int>> adjacencyMatrix(numVertices, std::vector<int>(numVertices));

    // Read the adjacency matrix
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            inputFile >> adjacencyMatrix[i][j];
        }
    }

    // Print the adjacency matrix
    std::cout << "Adjacency matrix:" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return {
        numVertices,
        adjacencyMatrix
    };
}

// 	A size of a multigraph:
//  it is a pair of (|V| x |E|) where number of vertices takes the advantage in comparison
MultigraphSize size(MultigraphAdjacencyMatrix multigraph) {
    // Time complexity: O(V^2)

    int numVertices = multigraph.size();
    int numEdges = 0;

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i; j < numVertices; ++j) {
            numEdges += multigraph[i][j];
        }
    }

    return {
        numVertices,
        numEdges
    };
}

// Compare two sizes of multigraphs
// 1. compare number of vertices
// 2. if equal, compare number of edges
// 3. if equal, return 0
//
// Return 1 if size1 > size2
// Return -1 if size1 < size2
// Return 0 if size1 == size2
int compareSize(MultigraphSize size1, MultigraphSize size2) {

    if (size1.numVertices > size2.numVertices) {
        return 1;
    }

    if (size1.numVertices < size2.numVertices) {
        return -1;
    }

    if (size1.numEdges > size2.numEdges) {
        return 1;
    }

    if (size1.numEdges < size2.numEdges) {
        return -1;
    }

    return 0;
}


std::vector<std::pair<int, int>> degreeSequence(const MultigraphAdjacencyMatrix &multigraph) {
    // Time complexity: O(V^2)
    // Space complexity: O(V)

    std::vector<std::pair<int, int>> degreeSequence(multigraph.size());

    for (int i = 0; i < multigraph.size(); ++i) {
        int degree = 0;
        for (int j = 0; j < multigraph.size(); ++j) {
            degree += multigraph[i][j];
        }
        degreeSequence[i] = { i, degree };
    }

    // Sort the degree sequence
    std::sort(degreeSequence.begin(), degreeSequence.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    return degreeSequence;
}

void writeGraph(std::ofstream& outputFile, const MultigraphAdjacencyMatrix& multigraph) {
    // Write the number of vertices
    int num_vertices = multigraph.size();
    outputFile << num_vertices << std::endl;

    // Write the adjacency matrix
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            outputFile << multigraph[i][j] << " ";
        }
        outputFile << std::endl;
    }
}


std::vector<ReadGraphResult> readGraphFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "Error: cannot open file " << filename << std::endl;
        throw std::runtime_error("Cannot open file");
    }

    int graphCount;
    inputFile >> graphCount;

    std::vector<ReadGraphResult> readGraphResults(graphCount);

    for (int i = 0; i < graphCount; ++i) {
        readGraphResults[i] = readGraph(inputFile);
        // Skip the empty line
        inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return readGraphResults;
}

void writeGraphToFile(const std::string& filename, const MultigraphAdjacencyMatrix& multigraph) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cout << "Error: cannot open file " << filename << std::endl;
        throw std::runtime_error("Cannot open file");
    }

    int graphCount = 1;

    outputFile << graphCount << std::endl;
    writeGraph(outputFile, multigraph);
    outputFile.close();
}

#endif //AAC_LABORATORIES_GRAPH_UTILS_H
