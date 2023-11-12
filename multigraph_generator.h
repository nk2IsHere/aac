//
// Created by Nikita Kozlov on 12.11.2023.
//

#include <random>

#ifndef AAC_LABORATORIES_MULTIGRAPH_GENERATOR_H
#define AAC_LABORATORIES_MULTIGRAPH_GENERATOR_H

MultigraphAdjacencyMatrix generateMultigraph(int numVertices, int numEdges) {
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());

    // Time complexity: O(V^2)
    // Space complexity: O(V^2)

    MultigraphAdjacencyMatrix multigraph(numVertices, std::vector<int>(numVertices));

    // Generate a random multigraph
    std::uniform_int_distribution<std::mt19937::result_type> randomVertexDistribution(0, numVertices - 1);
    for (int i = 0; i < numEdges; ++i) {
        int vertex1 = randomVertexDistribution(randomNumberGenerator);
        int vertex2 = randomVertexDistribution(randomNumberGenerator);

        multigraph[vertex1][vertex2]++;
        multigraph[vertex2][vertex1]++;
    }

    return multigraph;
}

#endif //AAC_LABORATORIES_MULTIGRAPH_GENERATOR_H
