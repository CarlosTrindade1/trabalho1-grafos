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
    int key;
    int cost;
};

struct GraphStruct {
    int key;
    int* EdgeStruct;
};

class Graph {
    public:
        Graph(GraphInput* graphInput);

    private:
        GraphStruct* vector;
};

GraphInput* readFile(string fileName);

int main(int argc, char **argv) {
    string fileName = argv[1];
    
    GraphInput* graphInput = readFile(fileName);

    if (!graphInput) {
        cout << "Arquivo inválido!" << endl;
        return 1;
    }

    cout << graphInput->vertices << endl;
    cout << graphInput->edges << endl;

    cout << "VÉRTICES" << endl;
    for (int i = 0; i < graphInput->vertices; i++) {
        cout << graphInput->verticesInput[i].key << " " << graphInput->verticesInput[i].qtdEntranceEdges << " " << graphInput->verticesInput[i].qtdExitEdges << endl;
    }

    cout << "ARESTAS" << endl;
    for (int i = 0; i < graphInput->edges; i++) {
        cout << graphInput->edgesInput[i].originVertice << " " << graphInput->edgesInput[i].destinyVertice << " " << graphInput->edgesInput[i].cost << endl;
    }

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
