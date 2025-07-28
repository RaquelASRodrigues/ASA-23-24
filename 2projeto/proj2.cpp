/* ASA projeto 2 2023/2024
 * Grupo: al039
 * Alunos: Sofia Lopes (106198), Raquel Rodrigues (106322)
 * 
 * Programa que determina qual o maior número de saltos que uma dada doença pode
 * fazer numa população, tendo em conta as interações entre os indíviduos.
 */

#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <stack>

enum Color {WHITE, GREY, BLACK};

void DFS_Visit(std::vector<std::vector<int>>& graph, int startVertex, 
    std::vector<Color>& colours, std::stack<int>& finishedVisits, bool firstTraversal, 
    std::vector<int>& componentsId, int componentId, std::vector<int>& jumps) {
        
    std::stack<int> verticesStack;
    std::vector<int> predecessors(graph.size(), 0); // second DFS
    int lastVertex = 0;
    
    verticesStack.push(startVertex); // push the first vertex
    if (!firstTraversal)
        componentsId[startVertex] = componentId;

    while (!verticesStack.empty()) {
        int currentVertex = verticesStack.top();
        if (colours[currentVertex] == WHITE) {
            std::vector<int> vertexAdjacents = graph[currentVertex];

            if (!firstTraversal) {
                    // adjacent vertex from the same scc
                if (vertexAdjacents.size() != 0 && componentsId[predecessors[currentVertex]] == componentsId[currentVertex])
                    jumps[currentVertex] = jumps[predecessors[currentVertex]];
            }
            
            for (int adjacent: vertexAdjacents) {
                if (colours[adjacent] == WHITE) {
                    verticesStack.push(adjacent);
                    if (!firstTraversal) {  // adjacent vertex from the same scc
                        componentsId[adjacent] = componentId;
                        predecessors[adjacent] = currentVertex;
                    }                          
                }
                else if (!firstTraversal) {
                    if (componentsId[currentVertex] != componentsId[adjacent])  // adjacent vertex from another scc
                        jumps[currentVertex] = std::max(jumps[currentVertex], jumps[adjacent] + 1);
                }
            }
            colours[currentVertex] = GREY; // adjacents have been pushed into the stack
            
        }
        else if (colours[currentVertex] == GREY) {
            colours[currentVertex] = BLACK; // mark this vertex as finished visiting
            if (firstTraversal)
                finishedVisits.push(currentVertex);  
            else {
                if (componentsId[currentVertex] == componentsId[lastVertex])
                    jumps[currentVertex] = std::max(jumps[currentVertex],jumps[lastVertex]);
                lastVertex = currentVertex; 
            }          
            verticesStack.pop();
        }
        else { // colour BLACK
            verticesStack.pop();
        }
    }
}

std::stack<int> firstDFS(std::vector<std::vector<int>>& graph) {
    int numVertices = graph.size();
    std::vector<Color> colours(numVertices, WHITE);
    std::stack<int> finishedVisits;
    std::vector<int> componentIds;  // not utilized in this first DFS
    std::vector<int> jumps(numVertices, 0); // not utilized

    for (int i = 1; i < numVertices; i++) {
        if (colours[i] == WHITE)
            DFS_Visit(graph, i, colours, finishedVisits, true, componentIds, 0, jumps);
    }

    return finishedVisits; // vertices in topological order
}

std::vector<int> secondDFS(std::vector<std::vector<int>>& transposedGraph, std::stack<int>& finishedVisits) {
    int numVertices = transposedGraph.size();
    std::vector<Color> colours(numVertices, WHITE);
    std::vector<int> componentIds(numVertices, 0);
    int componentId = 0;
    std::vector<int> jumps(numVertices, 0);

    while (!finishedVisits.empty()) {   // visit vertices by order of finishing time
        int currentVertex = finishedVisits.top();
        finishedVisits.pop();
        if (colours[currentVertex] == WHITE) {  // new strongly connected component found
            componentId++;
            DFS_Visit(transposedGraph, currentVertex, colours, finishedVisits, false, componentIds, componentId, jumps);
        }            
    }
    return jumps;
}

std::vector<std::vector<int>> transposeGraph(std::vector<std::vector<int>>& graph) {
    std::vector<std::vector<int>> transposedGraph(graph.size(), std::vector<int>());
    int numVertices = graph.size();

    for (int vertex = 1; vertex < numVertices; vertex++) {
        std::vector<int> vertexAdjacents = graph[vertex];
        for (int adjacent: vertexAdjacents) {
            transposedGraph[adjacent].push_back(vertex);    // inverted edge
        }        
    }
    return transposedGraph;
}

int maximumGraphJump(std::vector<std::vector<int>>& graph) {

    std::stack<int> finishedVisits = firstDFS(graph);
    std::vector<std::vector<int>> transposedGraph = transposeGraph(graph);

    std::vector<int> jumps = secondDFS(transposedGraph, finishedVisits);

    int max = 0;
    for (int j: jumps) {
        if (j > max)
            max = j;
    }
    return max;
}

int main() {
    int vertices, edges; // vertices - number of individuals
                         // edges - number of connections

    if (scanf("%d %d", &vertices, &edges) != 2) return 1;

    // represents the graph and its adjacent vertices
    std::vector<std::vector<int>> graph(vertices + 1, std::vector<int>());

    int vertex, adjacent;
    for (int i = 0; i < edges; i++) {
        if (scanf("%d %d", &vertex, &adjacent) != 2 ) return 1;
        graph[vertex].push_back(adjacent);
    }

    std::cout << maximumGraphJump(graph) << std::endl;

    return 0;
}