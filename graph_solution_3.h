
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

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_3_UNOPTIMIZED_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_3_UNOPTIMIZED_H


// 3. Maximal clique:
// a. Submultigraph which is complete
// b. Complete multigraph is defined by LKv where Kv is a complete graph of v vertices, and L is smallest number of connections between each vertex
bool isSetOfVerticesFormCompleteMultigraph(const MultigraphAdjacencyMatrix& multigraph, const std::vector<int>& vertex_indices) {
    for (int i = 0; i < vertex_indices.size(); i++) {
        for (int j = i + 1; j < vertex_indices.size(); j++) {
            if (multigraph[vertex_indices[i]][vertex_indices[j]] == 0) {
                return false;
            }
        }
    }

    return true;
}

int minimalAmountOfConnectionsBetweenVerticesInCompleteMultigraph(const MultigraphAdjacencyMatrix& multigraph, const std::vector<int>& vertex_indices) {
    int min_connections = INT_MAX;

    for (int i = 0; i < vertex_indices.size(); i++) {
        for (int j = i + 1; j < vertex_indices.size(); j++) {
            if (multigraph[vertex_indices[i]][vertex_indices[j]] < min_connections) {
                min_connections = multigraph[vertex_indices[i]][vertex_indices[j]];
            }
        }
    }

    return min_connections;
}

void enumerateAllPossibleSelectionsFromNtoMHelper(int start, int m, int minimalSize, std::vector<int>& current, std::vector<std::vector<int>>& result) {
    if (current.size() >= minimalSize) {
        result.push_back(current);
    }
    for (int i = start; i <= m; ++i) {
        current.push_back(i);
        enumerateAllPossibleSelectionsFromNtoMHelper(i + 1, m, minimalSize, current, result); // Recursively add more elements to the current combination
        current.pop_back(); // Backtrack and remove the last element added
    }
}

std::vector<std::vector<int>> enumerateAllPossibleSelectionsFromNtoM(int n, int m, int minimalSize) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    enumerateAllPossibleSelectionsFromNtoMHelper(n, m, minimalSize, current, result);
    return result;
}

CompleteMultigraph maximalCliqueBruteforce(const MultigraphAdjacencyMatrix& multigraph) {
    // For every single selection of vertices in size of 2 to |V|
    // 1. check if it is a complete multigraph
    // 2. if yes, store it for later
    // 3. if no, continue
    // 4. return the largest complete multigraph

    // Time complexity: O(2^|V| * |V|^2)

    auto multigraphSize = size(multigraph);

    // Alpha, n
    CompleteMultigraph largestCompleteMultigraph = {0, 0};
    for(const auto& selection: enumerateAllPossibleSelectionsFromNtoM(0, multigraphSize.numVertices - 1, 2)) {
        if(isSetOfVerticesFormCompleteMultigraph(multigraph, selection)) {
//            std::cout << "Found complete multigraph: ";
//            for (const auto vertex: selection) {
//                std::cout << vertex << " ";
//            }
//            std::cout << std::endl;

            int alpha = minimalAmountOfConnectionsBetweenVerticesInCompleteMultigraph(multigraph, selection);
            int n = selection.size();

//            std::cout << "This is a " << alpha << "K" << n << " graph" << std::endl;
            if (n > largestCompleteMultigraph.n) {
                largestCompleteMultigraph = {alpha, n};
            }

            if (n == largestCompleteMultigraph.n && alpha > largestCompleteMultigraph.alpha) {
                largestCompleteMultigraph = {alpha, n};
            }
        }
    }

    return largestCompleteMultigraph;
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_3_UNOPTIMIZED_H
