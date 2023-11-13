//
// Created by Nikita Kozlov on 05.11.2023.
//

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

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_2_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_2_H

int graphEditDistance(const MultigraphAdjacencyMatrix& multigraph1, const MultigraphAdjacencyMatrix& multigraph2) {
    // Time complexity: O(V^2 * V!)

    auto size1 = size(multigraph1);
    auto size2 = size(multigraph2);

    int maxNumVertices = std::max(size1.numVertices, size2.numVertices);
    int editDistance = 0;

    // If the number of vertices is different, then the edit distance is the difference between the number of vertices
    editDistance += std::abs(size1.numVertices - size2.numVertices);

    // For all permutations from 0 to maxNumVertices - 1
    // Find the permutation that minimizes the graph edit distance
    int minEdgesEditDistance = std::numeric_limits<int>::max();

    std::vector<int> permutation(maxNumVertices);
    for (int i = 0; i < maxNumVertices; i++) {
        permutation[i] = i;
    }

    do {
        int edgesEditDistance = 0;
        for (int i = 0; i < maxNumVertices; i++) {
            for (int j = i; j < maxNumVertices; j++) {
                if (i == j) {
                    continue;
                }

                // Assume 1st graph is the one being edited into the 2nd graph. We permute the 1st graph mappings.
                int firstGraphI = permutation[i];
                int firstGraphJ = permutation[j];

                // If the vertex is not present in the 1st graph, then its degree is 0.
                int vertex1Degree = 0;
                if (firstGraphI < size1.numVertices && firstGraphJ < size1.numVertices) {
                    vertex1Degree = multigraph1[firstGraphI][firstGraphJ];
                }

                int secondGraphI = i;
                int secondGraphJ = j;

                int vertex2Degree = 0;
                if (secondGraphI < size2.numVertices && secondGraphJ < size2.numVertices) {
                    vertex2Degree = multigraph2[secondGraphI][secondGraphJ];
                }

//                std::cout << "Vertex 1 degree: " << vertex1Degree << " Vertex 2 degree: " << vertex2Degree << std::endl;

                edgesEditDistance += std::abs(vertex1Degree - vertex2Degree);
            }
        }

//        std::cout << "Permutation: ";
//        for (int i = 0; i < maxNumVertices; i++) {
//            std::cout << permutation[i] << " ";
//        }
//        std::cout << "Edges edit distance: " << edgesEditDistance << std::endl;
//        std::cout << "Min edges edit distance: " << minEdgesEditDistance << std::endl;
//        std::cout << std::endl;

        if (edgesEditDistance < minEdgesEditDistance) {
            minEdgesEditDistance = edgesEditDistance;
        }
    } while (std::next_permutation(permutation.begin(), permutation.end()));

    editDistance += minEdgesEditDistance;
    return editDistance;
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_2_H
