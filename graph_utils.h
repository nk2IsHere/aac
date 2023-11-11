
#ifndef AAC_LABORATORIES_GRAPH_UTILS_H
#define AAC_LABORATORIES_GRAPH_UTILS_H

typedef std::vector<std::vector<int>> MultigraphAdjacencyMatrix;
typedef std::pair<int, int> MultigraphSize;
typedef std::pair<int, int> CompleteMultigraph;

std::pair<int, MultigraphAdjacencyMatrix> readGraph(std::ifstream& input_file) {
    // Read the number of vertices
    int num_vertices;
    input_file >> num_vertices;
    std::cout << "Number of vertices: " << num_vertices << std::endl;

    std::vector<std::vector<int>> adjacencyMatrix(num_vertices, std::vector<int>(num_vertices));

    // Read the adjacency matrix
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            input_file >> adjacencyMatrix[i][j];
        }
    }

    // Print the adjacency matrix
    std::cout << "Adjacency matrix:" << std::endl;
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return {
            num_vertices,
            adjacencyMatrix
    };
}

// 	A size of a multigraph:
//  it is a pair of (|V| x |E|) where number of vertices takes the advantage in comparison
MultigraphSize size(MultigraphAdjacencyMatrix multigraph) {
    // Time complexity: O(V^2)

    int num_vertices = multigraph.size();
    int num_edges = 0;

    for (int i = 0; i < num_vertices; ++i) {
        for (int j = i; j < num_vertices; ++j) {
            num_edges += multigraph[i][j];
        }
    }

    return {
            num_vertices,
            num_edges
    };
}

// Compare two sizes of multigraphs
// 1. compare number of vertices
// 2. if equal, compare number of edges
// 3. if equal, return 0
int compareSize(MultigraphSize size1, MultigraphSize size2) {
    auto [vertices1, edges1] = size1;
    auto [vertices2, edges2] = size2;

    if (vertices1 > vertices2) {
        return 1;
    }

    if (vertices1 < vertices2) {
        return -1;
    }

    if (edges1 > edges2) {
        return 1;
    }

    if (edges1 < edges2) {
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

#endif //AAC_LABORATORIES_GRAPH_UTILS_H
