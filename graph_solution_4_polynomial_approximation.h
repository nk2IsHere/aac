//
// Created by Nikita Kozlov on 07.11.2023.
//

#include "graph_utils.h"

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_4_POLYNOMIAL_APPROXIMATION_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_4_POLYNOMIAL_APPROXIMATION_H

MultigraphAdjacencyMatrix makeSubmultigraphFromSelection(
    const MultigraphAdjacencyMatrix& multigraph,
    const std::vector<int>& selectedVertices,
    int newVertex
) {
    // Check if newVertex is already in selectedVertices
    bool isNewVertexIncluded = std::find(selectedVertices.begin(), selectedVertices.end(), newVertex) != selectedVertices.end();

    // Determine the size of the new submultigraph
    int submultigraphSize = selectedVertices.size() + (isNewVertexIncluded ? 0 : 1);

    // Initialize the new submultigraph adjacency matrix
    MultigraphAdjacencyMatrix submultigraph(submultigraphSize, std::vector<int>(submultigraphSize, 0));

    // Fill the adjacency matrix
    for (int i = 0; i < submultigraphSize; ++i) {
        for (int j = 0; j < submultigraphSize; ++j) {
            // Map the submultigraph vertices back to the original graph vertices
            int originalVertex1 = i < selectedVertices.size() ? selectedVertices[i] : newVertex;
            int originalVertex2 = j < selectedVertices.size() ? selectedVertices[j] : newVertex;

            // Set the edge weight in the submultigraph
            submultigraph[i][j] = multigraph[originalVertex1][originalVertex2];
        }
    }

    return submultigraph;
}

bool isSelected(
    int vertex1,
    int vertex2,
    const std::vector<int>& selectedVertices1,
    const std::vector<int>& selectedVertices2
) {
    return (find(selectedVertices1.begin(), selectedVertices1.end(), vertex1) != selectedVertices1.end())
        && (find(selectedVertices2.begin(), selectedVertices2.end(), vertex2) != selectedVertices2.end());
}

std::pair<std::vector<int>, std::vector<int>> maximalCommonSubmultigraphPolynomialApproximation(
    MultigraphAdjacencyMatrix multigraph1,
    MultigraphAdjacencyMatrix multigraph2
) {
    // Time complexity: O(V^3 * V^2)
    // Outer while loop: The outer loop continues until no improvements can be made. In the worst case, this can take O(V) iterations.

    std::vector<int> selection1;
    std::vector<int> selection2;

    int minimalCurrentGraphEditDistance = std::numeric_limits<int>::max();
    bool graphEditDistanceHasImproved = true;

    // Loop until no improvement is made
    while (graphEditDistanceHasImproved) {
//        std::cout << "iter\n";

        graphEditDistanceHasImproved = false;
        int maxVertexCount = selection1.size();
        std::pair<int, int> bestVertexPair = {-1, -1};
        int bestVertexPairGraphEditDistance = minimalCurrentGraphEditDistance;

        // Iterate through all pairs of vertices (one from each graph)
        for(int i = 0; i < multigraph1.size(); ++i) {
            for(int j = 0; j < multigraph2.size(); ++j) {
                // Skip if the vertex pair is already selected
                if (isSelected(i, j, selection1, selection2)) continue;

                // Create submultigraphs by including a new pair of vertices
                auto newSubmultigraph1 = makeSubmultigraphFromSelection(multigraph1, selection1, i);
                auto newSubmultigraph2 = makeSubmultigraphFromSelection(multigraph2, selection2, j);

//                std::cout << newSubmultigraph1.size() << " " << newSubmultigraph2.size() << std::endl;

                // Calculate the approximated GED for the new submultigraphs
                int currentVertexPairGraphEditDistance = graphEditDistancePolynomialApproximation(newSubmultigraph1, newSubmultigraph2);

                // Check if the current GED is better and update if necessary
                if (
                    currentVertexPairGraphEditDistance < bestVertexPairGraphEditDistance
                    || (currentVertexPairGraphEditDistance == bestVertexPairGraphEditDistance && newSubmultigraph1.size() > maxVertexCount)
                ) {
                    bestVertexPairGraphEditDistance = currentVertexPairGraphEditDistance;
                    bestVertexPair = {i, j};
                    graphEditDistanceHasImproved = true;
                }
            }
        }

        // Update the selected vertices if an improvement was made
        if (graphEditDistanceHasImproved) {
            selection1.push_back(bestVertexPair.first);
            selection2.push_back(bestVertexPair.second);
            minimalCurrentGraphEditDistance = bestVertexPairGraphEditDistance;
        }
    }

    return {selection1, selection2};
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_4_POLYNOMIAL_APPROXIMATION_H
