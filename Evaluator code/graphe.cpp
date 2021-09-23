#include "graphe.h"
//
// EXERCISE 25
//

//Utilities
edge countEdges(graph& G, edges& GE)
{
    //GE.size()/2 alone isn't enough to coun't the edges,as loops can only be once in GE
    edge count = GE.size(); 
    for (vertex v = 0; v < G.size(); v++)
        for (index i = 0; i < G[v].size(); i++)
            if (v == G[v][i])
                count++;
    return count / 2;
}

void addEdge(graph& G, edges& GE, vertex v, vertex u)
{
    index vi = G[v].size();
    index ui = G[u].size();
    G[v].push_back(u);

    if (u != v)
        G[u].push_back(v);

    edge en = countEdges(G, GE);
    GE.insert({ {v, vi}, en });
    GE.insert({ {u, ui}, en });
}

// 
//  Complete graph with vn vertices to adjacencies lists (graph)
//

void graphe_complete(graph& Kn, edges& KnE, index n)
{
    vertex vn = n;
    Kn = graph(vn);
    KnE = edges();
    for (vertex v = 0; v < vn; v++)
        for (vertex u = v + 1; u < vn; u++) 
            addEdge(Kn, KnE, v, u);


    //return Kn;
}

//
// EXERCISE 8 -----------------------
//

//Complete bipartite graph
void graphe_bipartite_complete(graph& Kn1n2, edges& Kn1n2E, index n1, index n2)
{
    vertex vn = n1 + n2;
    Kn1n2 = graph(vn);
    Kn1n2E = edges();
    for (vertex v = 0; v < n1; v++)
        for (vertex u = n1; u < vn; u++)
            addEdge(Kn1n2, Kn1n2E, v, u);
}

//Cycle graph
void graphe_cycle(graph& Cn, edges& CnE, index n)
{
    vertex vn = n;
    Cn = graph(vn);
    CnE = edges();
    for (vertex v = 0; v < vn - 1; v++)
        addEdge(Cn, CnE, v, v + 1);
    addEdge(Cn, CnE, vn - 1, 0);

}

//Star graph
void graphe_star(graph& Sn, edges& SnE, index n)
{
    vertex vn = n + 1;
    Sn = graph(vn);
    SnE = edges();
    for (vertex v = 0; v < n; v++)
        addEdge(Sn, SnE, v, n);


}

//Wheel graph
void graphe_wheel(graph& Wn, edges& WnE, index n)
{
    vertex vn = n + 1;
    Wn = graph(vn);
    WnE = edges();
    for (vertex v = 0; v < n - 1; v++)
    {
        addEdge(Wn, WnE, v, v + 1);
        addEdge(Wn, WnE, v, n);
    }
    addEdge(Wn, WnE, n - 1, 0);
    addEdge(Wn, WnE, n - 1, n);

}

// 
//  Read from file fname of edges to adjacencies lists (graph)
//
int graphe_read(graph& G, edges& GE, string fname)
{
    ifstream fin;
    fin.open(fname.c_str());
    if (fin.fail()) {
        cerr << "unable to open file " << fname.c_str() << " for reading" << endl;
        exit(1);
    }
    index graph_type;
    vertex vn;
    edge en;
    fin >> graph_type;
    fin >> vn >> en;
    G = graph(vn);
    
    for (edge e = 0; e < en; e++) 
    {
        vertex v, u;
        fin >> v >> u;
        addEdge(G, GE, v, u);
    }

    int evaluation_mode;
    fin >> evaluation_mode;
    return evaluation_mode;
}
// 
//  Write from adjacencies lists (graph) to stream
//  - vertices and edges numbers
//  - edges

void graphe_write(graph& G, edges& GE, ofstream& fout)
{
    //  Vertices number 
    vertex vn = G.size();
    //  Count edges number from adjacencies lists
    edge en = countEdges(G, GE);

    //  Write edges         
    fout << "Graph with " << vn << " vertices and " << en << " edges " << endl;
    fout << "Adjacencies lists" << endl;
    for (vertex v = 0; v < vn; v++) {
        fout << v << "\t:";
        for (index i = 0; i < G[v].size(); i++)
            fout << "\t" << G[v][i];
        fout << endl;
    }
    fout << "Edges" << endl;
    for (vertex v = 0; v < vn; v++)
        for (index i = 0; i < G[v].size(); i++)
            if (v <= G[v][i])
                fout << v << "-" << G[v][i] << ": " << GE[{v, i}] << endl;
}

//
//  EXERCISE 26
//
void line_graphe(graph& G, edges& GE, graph& LG, edges& LGE)
{
    vertex vn = G.size();
    edge en = countEdges(G,GE);

    LG = graph(en);

    for(vertex i = 0; i<vn; i++)
        for (index j = 0; j < G[i].size(); j++)
            for (index k = j + 1; k < G[i].size(); k++)
                addEdge(LG, LGE, GE[{i, j}], GE[{i, k}]);              
}