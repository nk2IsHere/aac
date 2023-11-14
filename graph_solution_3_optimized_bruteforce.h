
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

#include "graph_utils.h"
#include "graph_solution_3.h"

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_3_OPTIMIZED_BRUTEFORCE_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_3_OPTIMIZED_BRUTEFORCE_H

#include <algorithm> // For std::sort

CliqueAlgorithmResult maximalCliqueBruteforceOptimized(const MultigraphAdjacencyMatrix& multigraph) {
    // For every single selection of selection in size of 2 to |V|
    // 1. check if it is a complete multigraph
    // 2. if yes, return it
    // 3. if no, continue

    // Time complexity: O(2^|V|)

    // Start with the largest possible bitset
    auto multigraphSize = size(multigraph);

    int enumerateFrom = 0;
    int enumerateTo = multigraphSize.numVertices - 1;

    int totalElements = enumerateTo - enumerateFrom + 1;
    int minimalSize = 2; // Minimal size of a complete multigraph
    int currentCompleteGraphSize = 0; // Current size of the complete multigraph

    std::vector<int> largestCompleteMultigraphSelection = {};
    CompleteMultigraph largestCompleteMultigraph = {0, 0};

    // Iterate over all possible bitsets starting from the largest one.
    // Iterating from the largest one is important, because we want to find the largest complete multigraph
    // It makes sense to start from the largest one, because it is more likely to be a complete multigraph
    for (int bitset = (1 << totalElements) - 1; bitset > 0; --bitset) {
        std::vector<int> currentSelection;

        // In this case, a bitset is a selection of vertices
        // I.E.:
        // For a graph with 4 vertices, the bitset 0011 means that the vertices 0 and 1 are selected
        // For a graph with 4 vertices, the bitset 1010 means that the vertices 1 and 3 are selected
        // For a graph with 4 vertices, the bitset 1111 means that all vertices are selected

        // Construct the current selection based on the bitset
        for (int j = 0; j < totalElements; ++j) {
            if (bitset & (1 << j)) {
                currentSelection.push_back(enumerateFrom + j);
            }
        }

        // If selection is less than minimal size, skip it
        if (currentSelection.size() < minimalSize) {
            continue;
        }

        // If selection is less than the current complete graph size, skip it
        if (currentSelection.size() < currentCompleteGraphSize) {
            continue;
        }

        // Check if the current selection is a complete multigraph
        if(!isSetOfVerticesFormCompleteMultigraph(multigraph, currentSelection)) {
            continue;
        }

//        std::cout << "Found complete multigraph: ";
//        for (const auto vertex: currentSelection) {
//            std::cout << vertex << " ";
//        }
//        std::cout << std::endl;

        int alpha = minimalAmountOfConnectionsBetweenVerticesInCompleteMultigraph(multigraph, currentSelection);
        int n = currentSelection.size();

//        std::cout << "This is a " << alpha << "K" << n << " graph" << std::endl;
        if (n > largestCompleteMultigraph.n) {
            largestCompleteMultigraphSelection = currentSelection;
            largestCompleteMultigraph = {alpha, n};
        }

        if (n == largestCompleteMultigraph.n && alpha > largestCompleteMultigraph.alpha) {
            largestCompleteMultigraphSelection = currentSelection;
            largestCompleteMultigraph = {alpha, n};
        }

        currentCompleteGraphSize = n;
    }

    return {
        largestCompleteMultigraph,
        largestCompleteMultigraphSelection
    };
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_3_OPTIMIZED_BRUTEFORCE_H
