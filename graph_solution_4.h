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
#include <numeric>

#include "graph_utils.h"
#include "graph_solution_2.h"
#include "graph_solution_3.h"

#ifndef AAC_LABORATORIES_GRAPH_SOLUTION_4_H
#define AAC_LABORATORIES_GRAPH_SOLUTION_4_H

MultigraphAdjacencyMatrix makeSubmultigraphFromSelection(const MultigraphAdjacencyMatrix& multigraph, const std::vector<int>& selection) {
    int numVertices = selection.size();
    MultigraphAdjacencyMatrix submultigraph(numVertices, std::vector<int>(numVertices));

    for (int i = 0; i < numVertices; i++) {
        for (int j = i; j < numVertices; j++) {
            submultigraph[i][j] = multigraph[selection[i]][selection[j]];
            submultigraph[j][i] = multigraph[selection[i]][selection[j]];
        }
    }

    return submultigraph;
}

std::pair<std::vector<int>, std::vector<int>> maximalCommonSubmultigraph(MultigraphAdjacencyMatrix multigraph1, MultigraphAdjacencyMatrix multigraph2) {
    auto size1 = size(multigraph1);
    auto size2 = size(multigraph2);

    int minNumVertices = std::min(size1.numVertices, size2.numVertices);

    // For all possible selections of vertices from 2 to numVertices1
    std::vector<std::vector<int>> vectorSelections1 = enumerateAllPossibleSelectionsFromNtoM(0, size1.numVertices - 1, 2);
    // Group selections by size
    std::vector<std::vector<std::vector<int>>> groupedSelections1(minNumVertices + 1);
    for (auto& selection : vectorSelections1) {
        groupedSelections1[selection.size()].push_back(selection);
    }

    // For all possible selections of vertices from 2 to numVertices2
    std::vector<std::vector<int>> vectorSelections2 = enumerateAllPossibleSelectionsFromNtoM(0, size2.numVertices - 1, 2);
    // Group selections by size
    std::vector<std::vector<std::vector<int>>> groupedSelections2(minNumVertices + 1);
    for (auto& selection : vectorSelections2) {
        groupedSelections2[selection.size()].push_back(selection);
    }

    MultigraphSize currentMaximalCommonSubmultigraphSize = {0, 0};
    std::pair<std::vector<int>, std::vector<int>> currentMaximalCommonSubmultigraph;

    for(int i = minNumVertices; i >= 0; --i) {
        if (currentMaximalCommonSubmultigraphSize.numVertices > i) {
            break;
        }

        std::vector<std::vector<int>> selections1 = groupedSelections1[i];
        std::vector<std::vector<int>> selections2 = groupedSelections2[i];

        for (auto& selection1 : selections1) {
            for (auto& selection2 : selections2) {
                MultigraphAdjacencyMatrix submultigraph1 = makeSubmultigraphFromSelection(multigraph1, selection1);
                MultigraphAdjacencyMatrix submultigraph2 = makeSubmultigraphFromSelection(multigraph2, selection2);

                // If GED is 0, then the submultigraphs are isomorphic
                if (graphEditDistance(submultigraph1, submultigraph2) == 0) {
                   auto submultigraphSize = size(submultigraph1);

                   if (compareSize(submultigraphSize, currentMaximalCommonSubmultigraphSize) == 1) {
                       currentMaximalCommonSubmultigraphSize = submultigraphSize;
                       currentMaximalCommonSubmultigraph = {selection1, selection2};
                   }
                   continue;
                }

                // If GED is not 0 we can still look for the maximal common submultigraph where
                // we take the submultigraph of minimal connections per 2 vertices in both multigraphs
                // ie for degree sequences:
                // 5 4 3 5 2
                // 5 4 4 4 1
                // the result will be:
                // 5 4 3 4 1

//                std::cout << "Selections: " << std::endl;
//                std::cout << "Selection 1: ";
//                for (auto& selection : selection1) {
//                    std::cout << selection << " ";
//                }
//                std::cout << std::endl;
//                std::cout << "Selection 2: ";
//                for (auto& selection : selection2) {
//                    std::cout << selection << " ";
//                }
//                std::cout << std::endl;


//                // out submultigraph1
//                std::cout << "Submultigraph 1: " << std::endl;
//                for (auto& row : submultigraph1) {
//                    for (auto& element : row) {
//                        std::cout << element << " ";
//                    }
//                    std::cout << std::endl;
//                }
//                std::cout << std::endl;
//                // out submultigraph2
//                std::cout << "Submultigraph 2: " << std::endl;
//                for (auto& row : submultigraph2) {
//                    for (auto& element : row) {
//                        std::cout << element << " ";
//                    }
//                    std::cout << std::endl;
//                }
//                std::cout << std::endl;

                auto degreeSequence1 = degreeSequence(submultigraph1);
                auto degreeSequence2 = degreeSequence(submultigraph2);

                std::vector<int> minimalDegreeSequence(degreeSequence1.size());

                for (int j = 0; j < degreeSequence1.size(); ++j) {
                    minimalDegreeSequence[j] = std::min(degreeSequence1[j].second, degreeSequence2[j].second);

//                    std::cout << degreeSequence1[j].second << " " << degreeSequence2[j].second << " " << minimalDegreeSequence[j] << "; ";

                    // If the minimal degree sequence is 0, then the submultigraphs are not isomorphic at all
                    if (minimalDegreeSequence[j] == 0) {
                        minimalDegreeSequence.clear();
                        break;
                    }
                }

//                std::cout << std::endl;

                if (minimalDegreeSequence.empty()) {
                    continue;
                }

                MultigraphSize minimalDegreeSequenceMultigraphSize = {
                    (int)minimalDegreeSequence.size(),
                    std::accumulate(minimalDegreeSequence.begin(), minimalDegreeSequence.end(), 0)
                };

                if (compareSize(minimalDegreeSequenceMultigraphSize, currentMaximalCommonSubmultigraphSize) == 1) {
                    currentMaximalCommonSubmultigraphSize = minimalDegreeSequenceMultigraphSize;
                    currentMaximalCommonSubmultigraph = {selection1, selection2};
                }
            }
        }
    }

    return currentMaximalCommonSubmultigraph;
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_4_H
