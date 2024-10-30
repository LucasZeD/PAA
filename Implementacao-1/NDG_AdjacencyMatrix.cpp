/**
 * @file NDG_AdjacencyMatrix.cpp
 * @author Lucas Zegrine Duarte
 * @brief 
 * Implementar um grafo nao-direcionado utilizando matriz de adjacencia
 * Realizar busca em largura no grafo encontrando a excentricidade de um determinado vertice
 * 
 * @version 0.3
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
using namespace std;

/**
 * @brief Non Directed Graph
 * Data Structure -> Adjacency Matrix
 * 
 */
class ND_Graph{
    int numVertices;
    int **Vertex_Matrix;

    public:
        ND_Graph(int size);
        ND_Graph();
        ~ND_Graph();
        void Graph_input();
        void Graph_print();
        void Graph_find_eccentricity();
        int BFS_distance_search(int v_start);

    private:
        void add_edge(int u, int v);
        
};

/**
 * @brief Construct a new nd graph::nd graph object
 * 
 */
ND_Graph::ND_Graph(){
    numVertices = 10;
    Vertex_Matrix = new int*[numVertices];
    //start null graph matrix
    for(int i = 0; i < numVertices; i++){
        Vertex_Matrix[i] = new int[numVertices];
        for(int j = 0; j < numVertices; ++j){
            Vertex_Matrix[i][j] = 0;
        }
    }
    add_edge(0, 8);
    add_edge(1, 3);
    add_edge(1, 7);
    add_edge(1, 4);
    add_edge(2, 4);
    add_edge(2, 3);
    add_edge(2, 5);
    add_edge(3, 8);
    add_edge(3, 6);
    add_edge(3, 4);
    add_edge(3, 5);
    add_edge(5, 8);
    add_edge(6, 7);
    add_edge(8, 9);
}

/**
 * @brief Construct a new nd graph::nd graph object
 * 
 * @param size Graph size
 */
ND_Graph::ND_Graph(int size){
    numVertices = size;
    //row mem aloc
    Vertex_Matrix = new int*[numVertices];
    //creating null graph for give size
    for(int i = 0; i < numVertices; i++){
        //column mem aloc
        Vertex_Matrix[i] = new int[numVertices];
        for(int j = 0; j < numVertices; ++j){
            Vertex_Matrix[i][j] = 0;
        }
    }
}

/**
 * @brief Destroy the nd graph::nd graph object
 * 
 */
ND_Graph::~ND_Graph(){
    for(int i = 0; i < numVertices; ++i){
        //clear allocated memory
        delete[] Vertex_Matrix[i];
    }
    delete[] Vertex_Matrix;
}

/**
 * @brief Allows for manual input of graph vertices
 * 
 */
void ND_Graph::Graph_input(){
    cout << "Adicione as arestas" << endl;
    cout << "   Exemplo: - adcionar aresta conectando vertice u com o vertice v" << endl;
    cout << "              input: \"'u' 'v'\" " << endl;
    cout << "              input: \"0  3\"     -> adiciona aresta conectando os vertices 0 e 3" << endl;
    cout << "              Se o tamanho do grafo for igual a 4, então haverão as arestas \"0, 1, 2, 3\"" << endl;
    cout << "   Para parar de adicionar arestas digite '-1'" << endl << endl;
    int u, v;
    while(true){
        cout << "Digite uma aresta (u v): ";
        cin >> u;
        if(u == -1){
            break;
        }
        cin >> v;
        if(v == -1){
            break;
        }
        if(u >= numVertices || v >= numVertices || u < 0 || v < 0 ){
            cout << "Aresta \"" << u << " <-> " << v << "\" fora do limite" << endl;
        }
        else{
            Vertex_Matrix[u][v] = 1;
            Vertex_Matrix[v][u] = 1;
        }
    }
    cout << "Grafo Criado!" << endl << endl;
}

/**
 * @brief Print resulting graph matrix on screen
 * 
 */
void ND_Graph::Graph_print(){
    cout << "Matriz de adjacencia: " << endl;
    cout << "   ";
    for(int i = 0; i < numVertices; i++){
        cout << i << " ";
    }
    cout << endl << endl;
    for(int i = 0; i < numVertices; i++){
        cout << i << "  ";
        for(int j = 0; j < numVertices; j++){
            cout << Vertex_Matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * @brief Finds maximum distance between a vertex and all other vertices
 * 
 */
void ND_Graph::Graph_find_eccentricity(){
    int vertex = 0;
    int resp = 0;
    cout << "Digite o vertice qual deseja saber a excentricidade: (digite \"-1\", para finalizar.) ";
    while(true){
        cin >> vertex;
        if(vertex == -1){
            cout << endl;
            return;
        }
        if(vertex >= numVertices || vertex < 0){
            cout << "Aresta \"" << vertex << "\" fora do limite" << endl;
            break;
        }
        else{
            resp = BFS_distance_search(vertex);
            cout << "A excentricidade do vertice " << vertex << " eh: " << resp << endl << endl;
        }
        cout << "Digite o vertice qual deseja saber a excentricidade: ";
    }
}

/**
 * @brief perform a BFS starting from given vertex and return the maximum distance 
 * 
 * @param v_start 
 * @return int 
 */
int ND_Graph::BFS_distance_search(int v_start){
    cout << "__ BSF search __" << endl;

    int *queue = new int[numVertices];          //queue of vertices to be visited
    int queue_rear = 0;                         //index of queue rear
    int queue_front = 0;                        //index of queue front
    bool *visited = new bool[numVertices];      //list of visited vertices
    int *distance = new int[numVertices];       //list of distances between initial vertex and all other vertices
    int max_distance = 0;

    for(int i = 0; i < numVertices; i++){       
        visited[i] = false;                     //set all vertices as not visited
        distance[i] = -1;                       //set all distances as null
    }

    visited[v_start] = true;                    //visit first v_start and add it to list
    queue[queue_rear++] = v_start;              //queue push - updates the rear index and adds vertex to the previous rear index value, therefore the rear value will represent the next last open value of the queue
    distance[v_start] = 0;                      //set first distance as 0

    while(queue_front != queue_rear){           //if queue front index and read index is the same, then all values have been read
        int v = queue[queue_front++];           //queue pop - retrieves value at the front of the queue and updates the "queue_front" index value, therefore popping the last value out of the queue (FiFo)
        cout << " -> " << v << " " << " - distance: " << distance[v] << endl;

        for(int i = 0; i < numVertices; ++i){   //find neighbours
            if(Vertex_Matrix[v][i] == 1 && !visited[i]){    //check if not visited before
                visited[i] = true;                          //marks now as visited
                queue[queue_rear++] = i;                    //queue push
                distance[i] = distance[v] + 1;              //adds distance to vertex i (u), to be 1 more than distance of vertex v, as BFS increments in "layers"
            }
        }
    }

    for(int i = 0; i < numVertices; ++i){
        if(distance[i] > max_distance){
            max_distance = distance[i];
        }
    }

    //delete allocated memory
    delete[] visited;
    delete[] queue;
    delete[] distance;

    return max_distance;
}

/**
 * @brief add edge to graph
 * 
 * @param u vertex
 * @param v vertex
 */
void ND_Graph::add_edge(int u, int v){
    if(u < numVertices && v < numVertices && u >= 0 && v >= 0){
        Vertex_Matrix[u][v] = 1;
        Vertex_Matrix[v][u] = 1;
        cout << "Aresta \"" << u << " <-> " << v << "\" adicionada." << endl;
    }
    else{
        cout << "Aresta \"" << u << " <-> " << v << "\" fora do limite!" << endl;
    }
}

/**
 * @brief clear console screen
 * (not best)
 * 
 */
void clear() {
    #if defined _WIN32
        system("cls");
        //clrscr(); // including header file : conio.h
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");
        //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
    #elif defined (__APPLE__)
        system("clear");
    #endif
}

/**
 * @brief Main
 * 
 * @return int 
 */
int main(){
    int input, graphSize;
    while(true){
        clear();
        cout << endl << "Atividade PAA - Implementacao 01" << endl;
        cout << "   Estrutura: Grafo Nao Direcionado - Matriz de adjacencia" << endl;
        cout << "   Objetivo : Encontrar excentricidade por meio de BFS" << endl << endl;
        cout << "Qual grafo deseja utilizar? " << endl;
        cout << "1 = Grafo pre-definido" << endl;
        cout << "2 = Montar grafo manualmente" << endl;
        cout << "0 = Fim" << endl;
        cin  >> input;
        if (input == 1){
            clear();
            cout << "Atividade PAA - Implementacao 01 - Grafo pre-definido" << endl;
            cout << "   Estrutura: Grafo Nao Direcionado - Matriz de adjacencia" << endl;
            cout << "   Objetivo : Encontrar excentricidade por meio de BFS" << endl << endl;
            ND_Graph g_p;
            cout << endl;
            g_p.Graph_print();
            //find eccentric value for given user vertex input
            g_p.Graph_find_eccentricity();
            //
            cout << "Aperte \"Enter\" para reiniciar: ";
            cin.ignore();
            cin.get();
        }
        else if (input == 2){
            clear();
            cout << "Atividade PAA - Implementacao 01 - Grafo manual" << endl;
            cout << "   Estrutura: Grafo Nao Direcionado - Matriz de adjacencia" << endl;
            cout << "   Objetivo : Encontrar excentricidade por meio de BFS" << endl << endl;
            cout << "Digite o tamanho do grafo: ";
            cin >> graphSize;
            cout << endl;
            ND_Graph g_c(graphSize);
            //call for user input to create graph
            g_c.Graph_input();
            //print graph
            g_c.Graph_print();
            //find eccentric value for given user vertex input
            g_c.Graph_find_eccentricity();
            //
            cout << "Aperte \"Enter\" para reiniciar: ";
            cin.ignore();
            cin.get();
        }
        else if (input == 0){
            return 0;
        }
        else{
            cout << "Comando nao reconhecido." << endl;
            //
            cout << "Aperte \"Enter\" para reiniciar: ";
            cin.ignore();
            cin.get();
        }
    }
    return 0;
}