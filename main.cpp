#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <utility>
#include <chrono>
#include <algorithm>
#include <random>

#include "library/clipp.h"
#include "library/termcolor.h"
#include "graph_utils.h"
#include "graph_solution_2.h"
#include "graph_solution_2_polynomial_approximation.h"
#include "graph_solution_3.h"
#include "graph_solution_3_optimized_bruteforce.h"
#include "graph_solution_3_polynomial_approximation.h"
#include "graph_solution_4.h"
#include "graph_solution_4_polynomial_approximation.h"
#include "multigraph_generator.h"

// For Multigraphs:
// 	1. define size (practical, reasonable definition)
//	2. distance function (metric), metric space - in the space of multigraphs define a function that is a metric (satisties axioms)
//	3. maximal clique (brute force algo [exp complexity] and its approximation [polynomial complexity])
//	4. maximal common subgraph (of two multigraphs)

void renderSelectionOnMultigraph(const MultigraphAdjacencyMatrix& multigraph, const std::vector<int>& selection) {
    auto selectionInSet = std::set<int>(selection.begin(), selection.end());

    std::cout << "Vertices: " << std::endl;
    for (int i = 0; i < multigraph.size(); ++i) {
        if (selectionInSet.find(i) != selectionInSet.end()) {
            std::cout << termcolor::on_bright_white << termcolor::color<0, 0, 0> << i << termcolor::reset;
        } else {
            std::cout << i;
        }
        std::cout << " ";
    }
    std::cout << std::endl;

    std::cout << "Adjacency matrix: " << std::endl;
    for (int i = 0; i < multigraph.size(); ++i) {
        for (int j = 0; j < multigraph.size(); ++j) {
            if (
                i != j
                && selectionInSet.find(i) != selectionInSet.end()
                && selectionInSet.find(j) != selectionInSet.end()
            ) {
                std::cout << termcolor::on_bright_white << termcolor::color<0, 0, 0> << multigraph[i][j] << termcolor::reset;
            } /*else if(i == j) {
                std::cout << termcolor::on_color<0, 0, 0> << termcolor::color<0, 0, 0> << multigraph[i][j] << termcolor::reset;
            } */ else {
                std::cout << multigraph[i][j];
            }
            std::cout << " ";

        }
        std::cout << std::endl;
    }
}

enum class AlgorithmToRun {
    GenerateMultigraph,
    MaximalCliqueBruteforce,
    MaximalCliqueBruteforceOptimized,
    MaximalCliquePolynomialApproximation,
    GraphEditDistance,
    GraphEditDistancePolynomialApproximation,
    MaximalCommonSubmultigraph,
    MaximalCommonSubmultigraphPolynomialApproximation,
    MaximalCommonSubmultigraphPolynomialApproximationImprovedSearch
};

struct AlgorithmRunResult {
    long long timeMillis;
};

int main(int argc, char* argv[]) {
    AlgorithmToRun selectedAlgorithmToRun;
    bool shouldPrintTime = false;

    std::string filenamePrefix;
    int graphCount;
    int numVertices;
    int numEdges;

    std::string filename1;
    std::string filename2;

    auto generateMultigraphRunner = [](const std::string& filenamePrefix, int graphCount, int numVertices, int numEdges) -> AlgorithmRunResult {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < graphCount; ++i) {
            auto multigraph = generateMultigraph(numVertices, numEdges);
            writeGraphToFile(filenamePrefix + std::to_string(i) + ".txt", multigraph);
        }
        auto end = std::chrono::high_resolution_clock::now();

        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };
    
    auto generateMultigraphCli = (
        clipp::command("generate-multigraph").set(selectedAlgorithmToRun, AlgorithmToRun::GenerateMultigraph),
        clipp::value("filename prefix", filenamePrefix),
        clipp::value("graph count", graphCount),
        clipp::value("num vertices", numVertices),
        clipp::value("num edges", numEdges)
    );

    auto maximalCliqueBruteforceRunner = [](const std::string& filename) -> AlgorithmRunResult {
        auto readGraphResult = readGraphFromFile(filename);

        auto start = std::chrono::high_resolution_clock::now();
        auto completeMultigraph = maximalCliqueBruteforce(readGraphResult.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Maximal clique: " << termcolor::on_bright_white << termcolor::color<0, 0, 0> << completeMultigraph.alpha << "K" << completeMultigraph.n << termcolor::reset << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };
    
    auto maximalCliqueBruteforceCli = (
        clipp::command("maximal-clique-bruteforce").set(selectedAlgorithmToRun, AlgorithmToRun::MaximalCliqueBruteforce),
        clipp::value("filename 1", filename1)
    );

    auto maximalCliqueBruteforceOptimizedRunner = [](const std::string& filename) -> AlgorithmRunResult {
        auto readGraphResult = readGraphFromFile(filename);

        auto start = std::chrono::high_resolution_clock::now();
        auto completeMultigraph = maximalCliqueBruteforceOptimized(readGraphResult.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Maximal clique: " << termcolor::on_bright_white << termcolor::color<0, 0, 0> << completeMultigraph.alpha << "K" << completeMultigraph.n << termcolor::reset << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto maximalCliqueBruteforceOptimizedCli = (
        clipp::command("maximal-clique-bruteforce-optimized").set(selectedAlgorithmToRun, AlgorithmToRun::MaximalCliqueBruteforceOptimized),
        clipp::value("filename 1", filename1)
    );

    auto maximalCliquePolynomialApproximationRunner = [](const std::string& filename) -> AlgorithmRunResult {
        auto readGraphResult = readGraphFromFile(filename);

        auto start = std::chrono::high_resolution_clock::now();
        auto completeMultigraph = maximalCliquePolynomialApproximation(readGraphResult.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Maximal clique: " << termcolor::on_bright_white << termcolor::color<0, 0, 0> << completeMultigraph.alpha << "K" << completeMultigraph.n << termcolor::reset << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto maximalCliquePolynomialApproximationCli = (
        clipp::command("maximal-clique-polynomial-approximation").set(selectedAlgorithmToRun, AlgorithmToRun::MaximalCliquePolynomialApproximation),
        clipp::value("filename 1", filename1)
    );

    auto graphEditDistanceRunner = [](const std::string& filename1, const std::string& filename2) -> AlgorithmRunResult {
        auto readGraphResult1 = readGraphFromFile(filename1);
        auto readGraphResult2 = readGraphFromFile(filename2);

        auto start = std::chrono::high_resolution_clock::now();
        int graphEditDistanceResult = graphEditDistance(readGraphResult1.multigraph, readGraphResult2.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Graph edit distance: " << termcolor::on_bright_white << termcolor::color<0, 0, 0> << graphEditDistanceResult << termcolor::reset << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto graphEditDistanceCli = (
        clipp::command("graph-edit-distance").set(selectedAlgorithmToRun, AlgorithmToRun::GraphEditDistance),
        clipp::value("filename 1", filename1),
        clipp::value("filename 2", filename2)
    );

    auto graphEditDistancePolynomialApproximationRunner = [](const std::string& filename1, const std::string& filename2) -> AlgorithmRunResult {
        auto readGraphResult1 = readGraphFromFile(filename1);
        auto readGraphResult2 = readGraphFromFile(filename2);

        auto start = std::chrono::high_resolution_clock::now();
        int graphEditDistancePolynomialApproximationResult = graphEditDistancePolynomialApproximation(readGraphResult1.multigraph, readGraphResult2.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Graph edit distance: " << termcolor::on_bright_white << termcolor::color<0, 0, 0> << graphEditDistancePolynomialApproximationResult << termcolor::reset << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto graphEditDistancePolynomialApproximationCli = (
        clipp::command("graph-edit-distance-polynomial-approximation").set(selectedAlgorithmToRun, AlgorithmToRun::GraphEditDistancePolynomialApproximation),
        clipp::value("filename 1", filename1),
        clipp::value("filename 2", filename2)
    );

    auto maximalCommonSubmultigraphRunner = [](const std::string& filename1, const std::string& filename2) -> AlgorithmRunResult {
        auto readGraphResult1 = readGraphFromFile(filename1);
        auto readGraphResult2 = readGraphFromFile(filename2);

        auto start = std::chrono::high_resolution_clock::now();
        auto selections = maximalCommonSubmultigraph(readGraphResult1.multigraph, readGraphResult2.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Maximal common submultigraph: " << std::endl;
        std::cout << "Selection from Graph 1: " << std::endl;
        renderSelectionOnMultigraph(readGraphResult1.multigraph, selections.first);
        std::cout << std::endl;
        std::cout << "Selection from Graph 2: " << std::endl;
        renderSelectionOnMultigraph(readGraphResult2.multigraph, selections.second);
        std::cout << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto maximalCommonSubmultigraphCli = (
        clipp::command("maximal-common-submultigraph").set(selectedAlgorithmToRun, AlgorithmToRun::MaximalCommonSubmultigraph),
        clipp::value("filename 1", filename1),
        clipp::value("filename 2", filename2)
    );

    auto maximalCommonSubmultigraphPolynomialApproximationRunner = [](const std::string& filename1, const std::string& filename2) -> AlgorithmRunResult {
        auto readGraphResult1 = readGraphFromFile(filename1);
        auto readGraphResult2 = readGraphFromFile(filename2);

        auto start = std::chrono::high_resolution_clock::now();
        auto selections = maximalCommonSubmultigraphPolynomialApproximation(readGraphResult1.multigraph, readGraphResult2.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Maximal common submultigraph: " << std::endl;
        std::cout << "Selection from Graph 1: " << std::endl;
        renderSelectionOnMultigraph(readGraphResult1.multigraph, selections.first);
        std::cout << std::endl;
        std::cout << "Selection from Graph 2: " << std::endl;
        renderSelectionOnMultigraph(readGraphResult2.multigraph, selections.second);
        std::cout << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto maximalCommonSubmultigraphPolynomialApproximationCli = (
        clipp::command("maximal-common-submultigraph-polynomial-approximation").set(selectedAlgorithmToRun, AlgorithmToRun::MaximalCommonSubmultigraphPolynomialApproximation),
        clipp::value("filename 1", filename1),
        clipp::value("filename 2", filename2)
    );

    auto maximalCommonSubmultigraphPolynomialApproximationImprovedSearchRunner = [](const std::string& filename1, const std::string& filename2) -> AlgorithmRunResult {
        auto readGraphResult1 = readGraphFromFile(filename1);
        auto readGraphResult2 = readGraphFromFile(filename2);

        auto start = std::chrono::high_resolution_clock::now();
        auto selections = maximalCommonSubmultigraphPolynomialApproximationImprovedSearch(readGraphResult1.multigraph, readGraphResult2.multigraph);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Maximal common submultigraph: " << std::endl;
        std::cout << "Selection from Graph 1: " << std::endl;
        renderSelectionOnMultigraph(readGraphResult1.multigraph, selections.first);
        std::cout << std::endl;
        std::cout << "Selection from Graph 2: " << std::endl;
        renderSelectionOnMultigraph(readGraphResult2.multigraph, selections.second);
        std::cout << std::endl;
        return {
            .timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        };
    };

    auto maximalCommonSubmultigraphPolynomialApproximationImprovedSearchCli = (
        clipp::command("maximal-common-submultigraph-polynomial-approximation-improved-search").set(selectedAlgorithmToRun, AlgorithmToRun::MaximalCommonSubmultigraphPolynomialApproximationImprovedSearch),
        clipp::value("filename 1", filename1),
        clipp::value("filename 2", filename2)
    );

    auto cli = (
        (
            generateMultigraphCli
            | maximalCliqueBruteforceCli
            | maximalCliqueBruteforceOptimizedCli
            | maximalCliquePolynomialApproximationCli
            | graphEditDistanceCli
            | graphEditDistancePolynomialApproximationCli
            | maximalCommonSubmultigraphCli
            | maximalCommonSubmultigraphPolynomialApproximationCli
            | maximalCommonSubmultigraphPolynomialApproximationImprovedSearchCli
        ),
        clipp::option("-t", "--time").set(shouldPrintTime).doc("Print time taken to run the algorithm in milliseconds")
    );

    if(!clipp::parse(argc, argv, cli)) {
        std::cout << clipp::make_man_page(cli, argv[0]);
        return 1;
    }

    AlgorithmRunResult algorithmRunResult = {
        .timeMillis = 0
    };

    switch (selectedAlgorithmToRun) {
        case AlgorithmToRun::GenerateMultigraph:
            algorithmRunResult = generateMultigraphRunner(filenamePrefix, graphCount, numVertices, numEdges);
            break;
        case AlgorithmToRun::MaximalCliqueBruteforce:
            algorithmRunResult = maximalCliqueBruteforceRunner(filename1);
            break;
        case AlgorithmToRun::MaximalCliqueBruteforceOptimized:
            algorithmRunResult = maximalCliqueBruteforceOptimizedRunner(filename1);
            break;
        case AlgorithmToRun::MaximalCliquePolynomialApproximation:
            algorithmRunResult = maximalCliquePolynomialApproximationRunner(filename1);
            break;
        case AlgorithmToRun::GraphEditDistance:
            algorithmRunResult = graphEditDistanceRunner(filename1, filename2);
            break;
        case AlgorithmToRun::GraphEditDistancePolynomialApproximation:
            algorithmRunResult = graphEditDistancePolynomialApproximationRunner(filename1, filename2);
            break;
        case AlgorithmToRun::MaximalCommonSubmultigraph:
            algorithmRunResult = maximalCommonSubmultigraphRunner(filename1, filename2);
            break;
        case AlgorithmToRun::MaximalCommonSubmultigraphPolynomialApproximation:
            algorithmRunResult = maximalCommonSubmultigraphPolynomialApproximationRunner(filename1, filename2);
            break;
        case AlgorithmToRun::MaximalCommonSubmultigraphPolynomialApproximationImprovedSearch:
            algorithmRunResult = maximalCommonSubmultigraphPolynomialApproximationImprovedSearchRunner(filename1, filename2);
            break;
    }

    if (shouldPrintTime) {
        std::cout << "Time taken: " << algorithmRunResult.timeMillis << "ms" << std::endl;
    }

    return 0;
}
