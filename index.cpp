/*
    TRABALHO 1 DE GRAFOS
    Estudante: Carlos Eduardo da Silva Trindade
    RGA: 202219040493
*/
#include <iostream>
#include <fstream>
#include <string>
#include <climits>

/*
    @function
    @description: 
    @params:
    @return:
    @complexity: 
*/

using namespace std;

struct VerticeInput {
    int key;
    int qtdEntranceEdges;
    int qtdExitEdges;
};

struct EdgesInput {
    int originVertice;
    int destinyVertice;
    int cost;
};

struct GraphInput {
    int vertices;
    int edges;
    VerticeInput* verticesInput;
    EdgesInput* edgesInput;
};

struct EdgeStruct {
    int destinyKey;
    int cost;
};

struct GraphStruct {
    int key;
    int indexEdge = 0;
    EdgeStruct* edges;
};

class Graph {
    public:
        Graph(GraphInput* graphInput);
        void printGraph();
        int vectorLength;
        GraphStruct* vector;
};

GraphInput* readFile(string fileName);
void dijkstra(Graph graph, int key, int previous[], int distance[]);
int min(int distance[], int processing[], int sizeVector);
void printArray(int array[], int size);

int main(int argc, char **argv) {
    string fileName = argv[1];
    
    GraphInput* graphInput = readFile(fileName);

    if (!graphInput) {
        cout << "Arquivo inválido!" << endl;
        return 1;
    }

    Graph graph = Graph(graphInput);

    //graph.printGraph();

    int previous[graph.vectorLength];
    int distance[graph.vectorLength];

    dijkstra(graph, 8, previous, distance);

    // for (int i = 0; i < graph.vectorLength; i++) {
    //     cout << previous[i] << " ";
    // }

    // cout << endl;

    // for (int i = 0; i < graph.vectorLength; i++) {
    //     cout << distance[i] << " ";
    // }

    return 0;
}

/*
    @function
    @description: Abre o arquivo de entrada e realiza uma série de validações. Caso o arquivo
    esteja correto, realiza a construção do grafo de input a partir das informações extraídas.
    @params: string fileName -> caminho do arquivo de entrada
    @complexity: O(n), n o número de linhas do arquivo
*/
GraphInput* readFile(string fileName) {
    ifstream txtFile;
    GraphInput* graphInput = new GraphInput;
    string line;
    int counter = 0;
    int vertices = 0;
    int edges = 0;

    txtFile.open(fileName);

    while (!txtFile.eof()) {
        getline(txtFile, line);

        if (counter == 0 && line[0] == 'I') {
            vertices = stoi(line.substr(2, 2));
            edges = stoi(line.substr(5, 2));

            graphInput->verticesInput = new VerticeInput[vertices];
            graphInput->edgesInput = new EdgesInput[edges];

            graphInput->vertices = vertices;
            graphInput->edges = edges;

        } else if (counter > 0 && counter <= vertices && line[0] == 'N') {
            graphInput->verticesInput[counter - 1].key = stoi(line.substr(2, 1));
            graphInput->verticesInput[counter - 1].qtdEntranceEdges = stoi(line.substr(4, 1));
            graphInput->verticesInput[counter - 1].qtdExitEdges = stoi(line.substr(6, 1));

        } else if (counter > vertices && counter - vertices <= edges && line[0] == 'E') {
            graphInput->edgesInput[counter - vertices - 1].originVertice = stoi(line.substr(2, 1));
            graphInput->edgesInput[counter - vertices - 1].destinyVertice = stoi(line.substr(4, 1));
            graphInput->edgesInput[counter - vertices - 1].cost = stoi(line.substr(6, 1));
        } else if (counter == vertices + edges + 1 && line[0] == 'T') {
            cout << "Arquivo válido!" << endl;
        } else {
            return nullptr;
        }

        counter++;
    }

    txtFile.close();

    return graphInput;
}

// Métodos da classe Graph

/*
    @function
    @description: Constrói o grafo na estrutura lista de adjacências a partir das informações
    de um grafo de entrada
    @params: GraphInput* graphInput -> ponteiro para uma estruta do tipo 'graphInput'
    @complexity: O(V * E)
*/
Graph::Graph(GraphInput* graphInput) {
    // O(V): V = Quantidade de vertices

    vector = new GraphStruct[graphInput->vertices];
    vectorLength = graphInput->vertices;

    for (int i = 0; i < graphInput->vertices; i++) {
        vector[i].key = graphInput->verticesInput[i].key;
        vector[i].edges = new EdgeStruct[graphInput->verticesInput[i].qtdExitEdges];
    }

    for (int i = 0; i < graphInput->edges; i++) {
        int originVertice = graphInput->edgesInput[i].originVertice;

        for (int j = 0; j < graphInput->vertices; j++) {
            if (originVertice == vector[j].key) {
                vector[j].edges[vector[j].indexEdge].destinyKey = graphInput->edgesInput[i].destinyVertice;
                vector[j].edges[vector[j].indexEdge].cost = graphInput->edgesInput[i].cost;
                vector[j].indexEdge++;
            }
        }
    }
}

/*
    @function
    @description: função que realiza a impressão dos vértices e arestas do grafo
    @params: void
    @complexity: O(V + A)
*/
void Graph::printGraph() {
    for (int i = 0; i < vectorLength; i++) {
        cout << vector[i].key;

        cout << " -> ";

        for (int j = 0; j < vector[i].indexEdge; j++) {
            cout << vector[i].edges[j].destinyKey << " " << vector[i].edges[j].cost << "  ";
        }

        cout << endl;
    }
}

// Fim dos métodos da classe Graph

/*
    @function
    @description: Constrói o grafo na estrutura lista de adjacências a partir das informações
    de um grafo de entrada
    @params: GraphInput* graphInput -> ponteiro para uma estruta do tipo 'graphInput'
    @return: void
    @complexity: O(V * E)
*/
void dijkstra(Graph graph, int key, int previous[], int distance[]) {
    int sizeProcessing = graph.vectorLength;
    int processing[sizeProcessing];

    for (int i = 0; i < graph.vectorLength; i++) {
        distance[graph.vector[i].key] = INT_MAX;
        previous[graph.vector[i].key] = -1;
        processing[i] = graph.vector[i].key;
    }

    distance[key] = 0;

    while (sizeProcessing != 0) {
        int minKey = min(distance, processing, graph.vectorLength); // u

        // cout << minKey << endl;

        processing[minKey] = -1;
        sizeProcessing--;

        for (int i = 0; i < graph.vector[minKey].indexEdge; i++) {
            int neighbor = graph.vector[minKey].edges[i].destinyKey; // v
            int costNeighbor = graph.vector[minKey].edges[i].cost;

            if (
                processing[neighbor] != -1 &&
                distance[minKey] + costNeighbor < distance[neighbor]
            ) {
                distance[neighbor] = distance[minKey] + costNeighbor;
                previous[neighbor] = minKey;
            }
        }
        
        printArray(distance, graph.vectorLength);
    }

}

void printArray(int array[], int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }

    cout << endl;
}

/*
    @function
    @description: Retorna o índice (vértice) da menor distância do vetor
    @params: vetor de inteiros, que representam as distâncias, e tamanho do vetor
    @return: índice (int)
    @complexity: O(n)
*/
int min(int distance[], int processing[], int sizeVector) {
    int min = 0;

    for (int i = 1; i < sizeVector; i++)
        if (distance[i] < distance[min] && processing[i] != -1) min = i;

    return min;
} 