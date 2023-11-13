//
// Created by Nikita Kozlov on 05.11.2023.
//

#include "graph_utils.h"

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_2_POLYNOMIAL_APPROXIMATION_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_2_POLYNOMIAL_APPROXIMATION_H

int graphEditDistancePolynomialApproximation(const MultigraphAdjacencyMatrix& multigraph1, const MultigraphAdjacencyMatrix& multigraph2){
    auto size1 = size(multigraph1);
    auto size2 = size(multigraph2);

    // Time complexity: O(V^2)
    // Space complexity: O(V)

    int maxNumVertices = std::max(size1.numVertices, size2.numVertices);
    int editDistance = 0;

    editDistance += std::abs(size1.numVertices - size2.numVertices);

    std::vector<std::pair<int, int>> degrees1 = degreeSequence(multigraph1);
    std::vector<std::pair<int, int>> degrees2 = degreeSequence(multigraph2);

    for (int i = 0; i < maxNumVertices; ++i) {
        if (i < degrees1.size() && i < degrees2.size()) {
            editDistance += std::abs(degrees1[i].second - degrees2[i].second);
        } else if (i < degrees1.size()) {
            editDistance += degrees1[i].second;
        } else if (i < degrees2.size()) {
            editDistance += degrees2[i].second;
        }
    }

    return editDistance;
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_2_POLYNOMIAL_APPROXIMATION_H
