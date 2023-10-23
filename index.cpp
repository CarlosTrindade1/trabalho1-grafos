/*
    TRABALHO 1 DE GRAFOS
    Estudante: Carlos Eduardo da Silva Trindade
    RGA: 202219040493
*/
#include <iostream>
#include <fstream>
#include <string>

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

    private:
        GraphStruct* vector;
        int vectorLength;
};

GraphInput* readFile(string fileName);

int main(int argc, char **argv) {
    string fileName = argv[1];
    
    GraphInput* graphInput = readFile(fileName);

    Graph graph = Graph(graphInput);

    graph.printGraph();

    // if (!graphInput) {
    //     cout << "Arquivo inválido!" << endl;
    //     return 1;
    // }

    // cout << graphInput->vertices << endl;
    // cout << graphInput->edges << endl;

    // cout << "VÉRTICES" << endl;
    // for (int i = 0; i < graphInput->vertices; i++) {
    //     cout << graphInput->verticesInput[i].key << " " << graphInput->verticesInput[i].qtdEntranceEdges << " " << graphInput->verticesInput[i].qtdExitEdges << endl;
    // }

    // cout << "ARESTAS" << endl;
    // for (int i = 0; i < graphInput->edges; i++) {
    //     cout << graphInput->edgesInput[i].originVertice << " " << graphInput->edgesInput[i].destinyVertice << " " << graphInput->edgesInput[i].cost << endl;
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
    @complexity: 
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
    @description:
    @params:
    @complexity: 
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