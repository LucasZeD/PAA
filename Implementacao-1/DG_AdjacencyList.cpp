/**
 * @file DG_AdjacencyList.cpp
 * @author  Lucas Zegrine Duarte
 * @brief 
 * Atividade de PAA - Projeto e Analise de Algoritmos, PUC-Minas Coracao Eucaristico
 * Professor: Silvo Guimarães
 * 
 * Implementar um grafo direcionado utilizando uma lista de adjacencia
 * Realizar busca em profundidade no grafo rotulando a aresta com:
 *      "de arvore",
 *      "de retorno",
 *      "de avanco",
 *      "de cruzamento"
 * 
 * @version 0.2
 * @date 2024-08-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
using namespace std;
#define FREE_SPACE -1

struct Edge{
    int u;
    int v;
    int edge_class;  //-1 - null; 0 - tree, 1 - return, 2 - forward, 3 - cross

    //fucntion to find and return edge position based on u, v to alter the edge_class

    /* Not used anymore
    void Edge_Print(){
        cout << "( " << u << " -> " << v << " ): ";
        switch (edge_class){
            case 0:
                cout << "Tree edge";
                break;
            case 1:
                cout << "Return edge";
                break;
            case 2:
                cout << "Forward edge";
                break;
            case 3:
                cout << "Cross edge";
                break;
            case -1:
                cout << "Error!. Never classified!";
                break;
        }
        cout << endl;
    }
    */
};


class Graph{
    //graph
    int vertices_number;        //number of vertices in graph
    int **graph_adjacency_list;
    //dfs
    int *vertex_find_time;
    int *vertex_end_time;
    int *vertex_color;          //0 - not visited; 1 - first time visited; 2 - all neighbours visited
    //edge classfication
    Edge *edges;           
    int edges_total;            //max number of edges allowed ((n*n-1)/2) -> complete graph
    int num_edges;              //number of edges added

    public:
        Graph();
        Graph(int size);
        ~Graph();
        void Add_Edge(int u, int v);
        void Graph_Add_Edges_Menu();
        void DFS();
        void DFS_aux();
        //
        void Graph_Print();
        void Graph_Print_Edges();
    private:
        void Vertex_Print(int vertex);
        int Find_Edge(int u, int v);
        void Classify_Edge(int u, int v);
        void Graph_Print_Edge(int edge);
        void Graph_Print(int *vertex);
        void DFS(int vertex, int &time);
        void Vertices_Colors();
        void Graph_Print_Times();
};

/**
 * @brief Construct a new Graph:: Graph object
 * 
 */
Graph::Graph(){
    vertices_number = 8;
    //
    graph_adjacency_list = new int*[vertices_number];
    vertex_color = new int[vertices_number];
    vertex_find_time = new int[vertices_number];
    vertex_end_time = new int[vertices_number];
    //
    for (int i = 0; i < vertices_number; i++){
        graph_adjacency_list[i] = new int[vertices_number];
        vertex_color[i] = 0;                //mark all as not visited
        vertex_find_time[i] = -1;           //null value
        vertex_end_time[i] = -1;             //null value
        for (int j = 0; j < vertices_number; ++j){
            graph_adjacency_list[i][j] = FREE_SPACE;    //mark as free space
        }
    }
    //
    num_edges = 0;
    edges_total = (vertices_number*(vertices_number-1));
    edges = new Edge[edges_total];
    for(int i=0; i < edges_total; i++){
        edges[i].u = -1;
        edges[i].v = -1;
        edges[i].edge_class = -1;
    }

    cout << "-- Creating Graph --" << endl;
    Add_Edge(0,1);
    Add_Edge(0,2);
    Add_Edge(0,7);
    Add_Edge(1,3);
    Add_Edge(2,4);
    Add_Edge(3,5);
    Add_Edge(4,3);
    Add_Edge(4,6);
    Add_Edge(4,7);
    Add_Edge(5,1);

    cout << endl;
}

/**
 * @brief Construct a new Graph:: Graph object
 * 
 * @param size 
 */
Graph::Graph(int size){
    vertices_number = size;
    //
    graph_adjacency_list = new int*[vertices_number];
    vertex_color = new int[vertices_number];
    vertex_find_time = new int[vertices_number];
    vertex_end_time = new int[vertices_number];
    //
    for (int i = 0; i < vertices_number; i++){
        graph_adjacency_list[i] = new int[vertices_number];
        vertex_color[i] = 0;                //mark all as not visited
        vertex_find_time[i] = -1;           //null value
        vertex_end_time[i] = -1;             //null value
        for (int j = 0; j < vertices_number; ++j){
            graph_adjacency_list[i][j] = FREE_SPACE;    //mark as free space
        }
    }
    //
    num_edges = 0;
    edges_total = (vertices_number*(vertices_number-1));
    edges = new Edge[edges_total];
    for(int i=0; i < edges_total; i++){
        edges[i].u = -1;
        edges[i].v = -1;
        edges[i].edge_class = -1;
    }

    cout << "-- Creating Graph --" << endl;
    Graph_Add_Edges_Menu();

    cout << endl;
}
/**
 * @brief Destroy the Graph object
 * 
 */
Graph::~Graph(){
    for(int i = 0; i < vertices_number; ++i){
        delete[] graph_adjacency_list[i];
    }
    delete[] graph_adjacency_list;
    delete[] vertex_find_time;
    delete[] vertex_end_time;
    delete[] vertex_color;
    delete[] edges;
}

void Graph::Add_Edge(int u, int v) {
    // Check for valid vertices
    if (u >= vertices_number || u < 0 || v >= vertices_number || v < 0) {
        cout << "Vertice invalido (" << u << ", " << v << ")." << endl;
        return;
    }

    // Check if the edge already exists
    int i = 0;
    while (i < vertices_number && graph_adjacency_list[u][i] != FREE_SPACE) {
        if (graph_adjacency_list[u][i] == v) {
            cout << "Aresta (" << u << "," << v << ") ja adicionada anteriormente!" << endl;
            return;
        }
        ++i;
    }

    // Add edge
    if (i < vertices_number) {
        graph_adjacency_list[u][i] = v;
        edges[num_edges].u = u;
        edges[num_edges].v = v;
        edges[num_edges].edge_class = -1;
        num_edges++;
        cout << "Add! (" << u << "," << v << ")" << endl;
    } else {
        cout << "Vetor de sucessores do vertice " << u << " cheio!" << endl;
    }
}

void Graph::Graph_Add_Edges_Menu() {
    cout << "Adicione as arestas" << endl;
    cout << "   Exemplo: - adicionar aresta conectando vertice u com o vertice v" << endl;
    cout << "              input: \"'u' 'v'\" " << endl;
    cout << "              input: \"0  3\"     -> adiciona aresta conectando os vertices 0 e 3" << endl;
    cout << "              Se o tamanho do grafo for igual a 4, então haverão as arestas \"0, 1, 2, 3\"" << endl;
    cout << "   Para parar de adicionar arestas digite '-1'" << endl << endl;

    int u, v;
    while (true) {
        cout << "Digite uma aresta (u v): ";
        cin >> u;
        if (u == -1) {
            break;
        }
        cin >> v;
        if (v == -1) {
            break;
        }

        // Use the helper function to add the edge
        Add_Edge(u, v);
    }
}

/**
 * @brief Print graph
 * 
 */
void Graph::Graph_Print(){
    cout << "-- Graph --" << endl;
    for(int i = 0; i < vertices_number; i++){
        Vertex_Print(i);
    }
    cout << endl;
}

/**
 * @brief Print given vertex and all of its successors
 * 
 * @param vertex vertex label [0, n]
 */
void Graph::Vertex_Print(int vertex){
    cout << "[" << vertex << "] -> ";
    int i = 0;
    while(i < vertices_number){
        if(graph_adjacency_list[vertex][i] == FREE_SPACE){
            cout << "//" << endl;
            return;
        }
        cout << graph_adjacency_list[vertex][i] << " -> ";
        i++;
    }
    cout << "." << endl ;
}

/**
 * @brief Prints all edges from graph with direction and classification
 * edge classificatio:
 * -1   - not classified
 * 0    - tree
 * 1    - return
 * 2    - forward
 * 3    - cross
 * 
 */
void Graph::Graph_Print_Edges(){
    cout << "-- Edges --" << endl;
    for(int i = 0; i < num_edges; i++){
        cout << "Aresta[" << i << "]: " << "( " << edges[i].u << " -> " << edges[i].v << " ): ";
        switch (edges[i].edge_class){
            case 0:
                cout << "Aresta de Arvore";
                break;
            case 1:
                cout << "Aresta de Retorno";
                break;
            case 2:
                cout << "Aresta de Avanco";
                break;
            case 3:
                cout << "Aresta de Cruzamento";
                break;
            case -1:
                cout << "Nao classificada!";
                break;
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * @brief Print a given edge
 * 
 * @param edge edge index
 */
void Graph::Graph_Print_Edge(int edge){
    cout << "Aresta[" << edge << "]: " << "( " << edges[edge].u << " -> " << edges[edge].v << " ): ";
    switch (edges[edge].edge_class){
        case 0:
            cout << "Aresta de Arvore";
            break;
        case 1:
            cout << "Aresta de Retorno";
            break;
        case 2:
            cout << "Aresta de Avanco";
            break;
        case 3:
            cout << "Aresta de Cruzamento";
            break;
        case -1:
            cout << "Nao classificada!";
            break;
    }
    cout << endl;
}

/**
 * @brief Depth-First-Search through the graph
 * starts at vertex 0
 * 
 */
void Graph::DFS(){
    cout << "-- DFS --" << endl;
    int time = 0;
    for(int i = 0; i < vertices_number; i++){
        if(vertex_color[i] == 0){   //white
            DFS(i, time);
        }
    }
    Graph_Print_Times();
}

/**
 * @brief Depth-First-Search starting at given vertex
 * Call for edge classification function when running
 * 
 * white vertex = never visited before              (vertex_color[u]) 0 - not visited; 1 - first time visited; 2 - all neighbours visited
 * blue vertex  = first time visited                (vertex_find_time[u])
 * red vertes   = all neighbours have been visited  (vertex_end_time[u])
 * 
 * @param vertex starting vertex
 */
void Graph::DFS(int vertex, int &time){
    int white = 0;
    int blue = 1;
    int red = 2;

    vertex_color[vertex] = blue;
    vertex_find_time[vertex] = ++time;

    for(int i = 0; i < vertices_number && graph_adjacency_list[vertex][i] != FREE_SPACE; ++i){
        int adj_vertex = graph_adjacency_list[vertex][i];

        if(vertex_color[adj_vertex] == white){
            Classify_Edge(vertex, adj_vertex);
            DFS(adj_vertex, time);
        }
        else{
            Classify_Edge(vertex, adj_vertex);
        }

    }
    vertex_color[vertex] = red;
    vertex_end_time[vertex] = ++time;
}

/**
 * @brief Classifies edge class based on edge type
 * 0 -> tree edge
 * 1 -> return edge
 * 2 -> forward edge
 * 3 -> cross edge
 * 
 * edges can be classified based on v vertex color when DFS goes through v for the first time,
 * white    -> tree
 * blue     -> return
 * red      -> if u is visited before v then forward
 *             if v is visited before u the cross
 * 
 * @param u vertex u
 * @param v vertex v
 */
void Graph::Classify_Edge(int u, int v){
    //just to make it easier to comprehend while coding
    int white = 0;
    int blue = 1;
    int red = 2;
    //
    int edge_index = Find_Edge(u, v);
    //
    if(edge_index == -1){
        cout << "Aresta nao foi encontrada: (" << u << ", " << v << ")." << endl;
    }
    //
    if( vertex_color[v] == white ){
        //Tree edge
        edges[edge_index].edge_class = 0;
        
    }
    else if( vertex_color[v] == blue ){
        //Return edge
        edges[edge_index].edge_class = 1;

    }
    else if( vertex_color[v] == red ){
        if(vertex_find_time[u] < vertex_find_time[v]){
            //Forward edge
            edges[edge_index].edge_class = 2;
        }
        else{   // vertex_find_time[u] > vertex_find_time[v]
            //Cross edge
            edges[edge_index].edge_class = 3;
        }
    }
    else{
        cout << "Erro ao classificar a aresta: (" << u << ", " << v << ")." << endl;
    }
    return;
}

/**
 * @brief Shows all vertices colors
 * 
 */
void Graph::Vertices_Colors(){
    for(int i = 0; i < vertices_number; i++){
        cout << "[" << i << "]" << vertex_color[i] << " ";
    }
    cout << endl;
}

/**
 * @brief Return the index of the edge inside array Edge *edges
 * 
 * @param u vertex u
 * @param v vertex v
 * @return int edge_index
 */
int Graph::Find_Edge(int u, int v){
    int i = 0;
    while(i < num_edges){
        if(edges[i].u == u && edges[i].v == v){
            return i;
        }
        i++;
    }
    return -1;
}

/**
 * @brief Print Vertex find and end times after running DFS or BFS search
 * 
 */
void Graph::Graph_Print_Times(){
    cout << "vertice [descoberta, termino]" << endl;
    for(int i=0; i < vertices_number; i++){
        cout << i << " [" << vertex_find_time[i] << ", " << vertex_end_time[i] << "]" << endl;
    }
    cout << endl;
    return;
}

void clear();
void ui_header();
void restart_prompt();

int main(){
    int input, graphSize;
    while(true){
        ui_header();
        cout << "Qual grafo deseja utilizar ? " << endl;
        cout << "1 - Grafo pre-definid. " << endl;
        cout << "2 - Criar grafo manualmente." << endl;
        cout << "0 - Sair." << endl;
        cin >> input;
        switch (input) {
            case 0:
                cout << "Saindo.";
                return 0;
                break;
            case 1:
                ui_header();
                //
                { 
                    Graph g;
                    g.Graph_Print();
                    g.DFS();
                    g.Graph_Print_Edges();
                }
                //
                restart_prompt();
                break;
            case 2:
                ui_header();
                //
                {
                    cout << "Digite o tamanho do grafo: ";
                    cin >> graphSize;
                    cout << endl;
                    Graph g(graphSize);
                    g.Graph_Print();
                    g.DFS();
                    g.Graph_Print_Edges(); 
                }
                //
                restart_prompt();
                break;
            default:
                cout << "Comando nao reconhecido." << endl;
                restart_prompt();
                break;
        }
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

void ui_header(){
    clear();
    cout << endl << "Atividade PAA - Implementacao 01" << endl;
    cout << "   Estrutura: Grago Direcionado - Lista de adjacencia" << endl;
    cout << "   Objetivo : Rolutar arestas de um grafo a partir de suas caracteristicas." << endl << endl;
}

void restart_prompt(){
    cout << "Aperte \"Enter\" para reiniciar: ";
    cin.ignore();
    cin.get();
}