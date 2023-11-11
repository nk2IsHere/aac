//
// Created by Nikita Kozlov on 05.11.2023.
//

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
        }
    }

    return submultigraph;
}

std::pair<std::vector<int>, std::vector<int>> maximalCommonSubmultigraph(MultigraphAdjacencyMatrix multigraph1, MultigraphAdjacencyMatrix multigraph2) {
    auto [numVertices1, numEdges1] = size(multigraph1);
    auto [numVertices2, numEdges2] = size(multigraph2);

    int minNumVertices = std::min(numVertices1, numVertices2);

    // For all possible selections of vertices from 2 to numVertices1
    std::vector<std::vector<int>> vectorSelections1 = enumerateAllPossibleSelectionsFromNtoM(0, numVertices1 - 1, 2);
    // Group selections by size
    std::vector<std::vector<std::vector<int>>> groupedSelections1(minNumVertices + 1);
    for (auto& selection : vectorSelections1) {
        groupedSelections1[selection.size()].push_back(selection);
    }

    // For all possible selections of vertices from 2 to numVertices2
    std::vector<std::vector<int>> vectorSelections2 = enumerateAllPossibleSelectionsFromNtoM(0, numVertices2 - 1, 2);
    // Group selections by size
    std::vector<std::vector<std::vector<int>>> groupedSelections2(minNumVertices + 1);
    for (auto& selection : vectorSelections2) {
        groupedSelections2[selection.size()].push_back(selection);
    }

    MultigraphSize currentMaximalCommonSubmultigraphSize = {0, 0};
    std::pair<std::vector<int>, std::vector<int>> currentMaximalCommonSubmultigraph;

    for(int i = minNumVertices - 1; i >= 0; --i) {
        if (currentMaximalCommonSubmultigraphSize.first > i) {
            break;
        }

        std::vector<std::vector<int>> selections1 = groupedSelections1[i];
        std::vector<std::vector<int>> selections2 = groupedSelections2[i];

        for (auto& selection1 : selections1) {
            for (auto& selection2 : selections2) {
                MultigraphAdjacencyMatrix submultigraph1 = makeSubmultigraphFromSelection(multigraph1, selection1);
                MultigraphAdjacencyMatrix submultigraph2 = makeSubmultigraphFromSelection(multigraph2, selection2);

                if (graphEditDistance(submultigraph1, submultigraph2) == 0) {
                    // Compare by vertices
                    if(selection1.size() > currentMaximalCommonSubmultigraphSize.first) {
                        currentMaximalCommonSubmultigraphSize = {selection1.size(), selection2.size()};
                        currentMaximalCommonSubmultigraph = {selection1, selection2};
                    }

                    // Compare by edges
                    if(selection1.size() == currentMaximalCommonSubmultigraphSize.first && selection2.size() > currentMaximalCommonSubmultigraphSize.second) {
                        currentMaximalCommonSubmultigraphSize = {selection1.size(), selection2.size()};
                        currentMaximalCommonSubmultigraph = {selection1, selection2};
                    }
                }
            }
        }
    }

    return currentMaximalCommonSubmultigraph;
}

#endif //AAC_LABORATORIES_GRAPH_SOLUTION_4_H
