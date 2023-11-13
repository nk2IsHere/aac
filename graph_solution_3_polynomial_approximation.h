//
// Created by Nikita Kozlov on 07.11.2023.
//

#include "graph_utils.h"

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_3_POLYNOMIAL_APPROXIMATION_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_3_POLYNOMIAL_APPROXIMATION_H

CompleteMultigraph maximalCliqueForSingleVertexGreedy(const MultigraphAdjacencyMatrix& multigraph, int vertex) {
    // Time complexity: O(V^2)

    int n = multigraph.size();

    // Track which vertices are in the clique
    std::vector<bool> inClique(n, false);

    // Store the vertices of the clique
    std::vector<int> clique;

    // Initially, add the given vertex to the clique
    inClique[vertex] = true;
    clique.push_back(vertex);

    // Greedily add vertices to the clique
    for (int i = 0; i < n; ++i) {
        if (i != vertex) {
            bool canAdd = true;

            // Check if the vertex i can be added to the clique
            for (int v : clique) {
                if (multigraph[i][v] == 0) {
                    // If there is no edge, it can't be added
                    canAdd = false;
                    break;
                }
            }

            // If the vertex can be added, add it to the clique
            if (canAdd) {
                inClique[i] = true;
                clique.push_back(i);
            }
        }
    }

    int alpha = minimalAmountOfConnectionsBetweenVerticesInCompleteMultigraph(multigraph, clique);
    return {
        .alpha = alpha,
        .n = (int)clique.size()
    };
}


CompleteMultigraph maximalCliquePolynomialApproximation(const MultigraphAdjacencyMatrix& multigraph) {
    // Time complexity: O(V^3)

    auto multigraphSize = size(multigraph);

    // Find the largest clique for each vertex
    // and return the largest one

    CompleteMultigraph largestCompleteMultigraph = {0, 0};
    for (int i = 0; i < multigraphSize.numVertices; i++) {
        CompleteMultigraph currentCompleteMultigraph = maximalCliqueForSingleVertexGreedy(multigraph, i);
        if (currentCompleteMultigraph.n > largestCompleteMultigraph.n) {
            largestCompleteMultigraph = currentCompleteMultigraph;
        }
        if(currentCompleteMultigraph.n == largestCompleteMultigraph.n && currentCompleteMultigraph.alpha > largestCompleteMultigraph.alpha) {
            largestCompleteMultigraph = currentCompleteMultigraph;
        }
    }

    return largestCompleteMultigraph;
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_3_POLYNOMIAL_APPROXIMATION_H
