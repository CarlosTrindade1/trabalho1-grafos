/*
    TRABALHO 1 DE GRAFOS
    Estudante: Carlos Eduardo da Silva Trindade
    RGA: 202219040493
*/
#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <sstream>
#include <vector>

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
void dijkstra(Graph graph, int key, int previous[], int distance[], int length[]);
int min(int distance[], int length[], int processing[], int sizeVector);
void update(int minKey, int neighbor, int costNeighbor, int distance[], int previous[], int length[]);
void output(int length[], int distance[], int previous[], int key, int size);

int main(int argc, char **argv) {
    string fileName = argv[1];
    string argumentKey = argv[2];

    int key = stoi(argumentKey);
    
    GraphInput* graphInput = readFile(fileName);

    if (!graphInput) {
        cout << "E" << endl;
        return 1;
    }

    Graph graph = Graph(graphInput);

    int previous[graph.vectorLength];
    int distance[graph.vectorLength];
    int length[graph.vectorLength];

    dijkstra(graph, key, previous, distance, length);

    output(length, distance, previous, key, graph.vectorLength);

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
    bool isFileValid = false;

    txtFile.open(fileName);

    while (!txtFile.eof()) {
        getline(txtFile, line);

        istringstream iss(line);
        vector<string> tokens;
        string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        if (counter == 0 && tokens[0] == "I") {
            vertices = stoi(tokens[1]);
            edges = stoi(tokens[2]);

            graphInput->verticesInput = new VerticeInput[vertices];
            graphInput->edgesInput = new EdgesInput[edges];

            graphInput->vertices = vertices;
            graphInput->edges = edges;

        } else if (counter > 0 && counter <= vertices && tokens[0] == "N") {
            graphInput->verticesInput[counter - 1].key = stoi(tokens[1]);
            graphInput->verticesInput[counter - 1].qtdEntranceEdges = stoi(tokens[2]);
            graphInput->verticesInput[counter - 1].qtdExitEdges = stoi(tokens[3]);

        } else if (counter > vertices && counter - vertices <= edges && tokens[0] == "E") {
            graphInput->edgesInput[counter - vertices - 1].originVertice = stoi(tokens[1]);
            graphInput->edgesInput[counter - vertices - 1].destinyVertice = stoi(tokens[2]);
            graphInput->edgesInput[counter - vertices - 1].cost = stoi(tokens[3]);
        } else if (counter == vertices + edges + 1 && line[0] == 'T') {
            isFileValid = true;
        } else if (!isFileValid) {
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
void dijkstra(Graph graph, int key, int previous[], int distance[], int length[]) {
    int sizeProcessing = graph.vectorLength;
    int processing[sizeProcessing];

    for (int i = 0; i < graph.vectorLength; i++) {
        distance[graph.vector[i].key] = INT_MAX;
        previous[graph.vector[i].key] = -1;
        processing[i] = graph.vector[i].key;
        length[i] = 0;
    }

    distance[key] = 0;

    while (sizeProcessing != 0) {
        int minKey = min(distance, length, processing, graph.vectorLength);

        processing[minKey] = -1;
        sizeProcessing--;

        for (int i = 0; i < graph.vector[minKey].indexEdge; i++) {
            int neighbor = graph.vector[minKey].edges[i].destinyKey;
            int costNeighbor = graph.vector[minKey].edges[i].cost;

            if (distance[minKey] == INT_MAX) continue;

            if (
                processing[neighbor] != -1 &&
                distance[minKey] + costNeighbor < distance[neighbor]
            ) {
                update(minKey, neighbor, costNeighbor, distance, previous, length);
            } else if (
                processing[neighbor] != -1 &&
                distance[minKey] + costNeighbor == distance[neighbor] &&
                length[minKey] + 1 < length[neighbor]
            ) {
                update(minKey, neighbor, costNeighbor, distance, previous, length);
            } else if (
                processing[neighbor] != -1 &&
                distance[minKey] + costNeighbor == distance[neighbor] &&
                length[minKey] + 1 == length[neighbor] &&
                minKey < previous[neighbor]
            ) {
                update(minKey, neighbor, costNeighbor, distance, previous, length);
            }
        }
    }
}

/*
    @function
    @description: Atualiza a distância de um vizinho de um determinado vértice.
    @params: número do vértice, número do vizinho, custo do vizinho e vetores onde estão
    armazenadas essas informações
    @return: void
    @complexity: O(1)
*/
void update(int minKey, int neighbor, int costNeighbor, int distance[], int previous[], int length[]) {
    distance[neighbor] = distance[minKey] + costNeighbor;
    previous[neighbor] = minKey;
    length[neighbor] = length[minKey] + 1;
}

/*
    @function
    @description: Realiza a impressão dos dados no formato passado na descrição do trabalho
    @params: vetores que contém as informações
    @return: void
    @complexity: O(n²)
*/
void output(int length[], int distance[], int previous[], int key, int size) {
    for (int i = 0; i < size; i++) {
        if (length[i] == 0 && i != key) {
            cout << "U " << i << endl;
            continue;
        }

        int path[length[i] + 1];

        path[length[i]] = i;

        int index = i;

        for (int j = length[i] - 1; j >= 0; j--) {
            path[j] = previous[index];
            index = previous[index];
        }

        cout << "P " << i << " " << distance[i] << " " << length[i] << " ";

        for (int j = 0; j < length[i] + 1; j++) {
            cout << path[j] << " ";
        }

        cout << endl;
    }
}

/*
    @function
    @description: Retorna o índice (vértice) da menor distância do vetor
    @params: vetor de inteiros, que representam as distâncias, e tamanho do vetor
    @return: índice (int)
    @complexity: O(n)
*/
int min(int distance[], int length[], int processing[], int sizeVector) {
    int min;

    for (int i = 0; i < sizeVector; i++) {
        if (processing[i] != -1) {
            min = i;
            break;
        }
    }

    for (int i = 1; i < sizeVector; i++) {
        if (distance[i] < distance[min] && processing[i] != -1) min = i;
        else if (distance[i] == distance[min] && length[i] < length[min] && processing[i] != -1) min = i;
        else if (distance[i] == distance[min] && length[i] == length[min] && i < min && processing[i] != -1) min = i;
    }

    return min;
} 