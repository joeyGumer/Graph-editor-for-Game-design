#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
 using namespace std;
 typedef unsigned int vertex;
 typedef unsigned int edge;
 typedef unsigned int index; 
 typedef unsigned int degree;
 typedef unsigned int component;
 typedef unsigned int length;
 typedef unsigned int color;
 typedef vector<vector<vertex> > graph;
 graph graph_complete( index n );
 graph graph_completeBipartite(index an, index bn);
 graph graph_cycle(index n);
 graph graph_star(index n);
 graph graph_wheel(index n);
 graph graph_read( string fname );
 void graph_write(  graph& G, ofstream& fout );
 component BFS( graph &G );
 component DFS( graph &G );
 void Dijkstra( graph &G, vertex sv, ofstream& fout );
 void BFS_Trees(graph& G, ofstream& fout);
 component DFS_Trees(graph& G, ofstream& fout);
 void DFS_Trees(graph& G, graph& H, vector<bool>& DFSl, vertex v, index& Ti, length d, ofstream& fout);
 //Exercise 18
 bool HamiltonianCycle(graph& G, ofstream& fout);
 index HamiltonianCycles(graph& G, ofstream& fout);

 //Exercise 22
 color ColorationBrelaz(graph& G, vector<color>& colors, ofstream& fout);

 //Exercise 3
 graph graph_king(index n1, index n2);
 graph graph_rook(index n1, index n2);
 graph graph_bishop(index n1, index n2);
 graph graph_knight(index n1, index n2);
 void chessDijkstra(graph& G, index n1, index n2, vertex sv, ofstream& fout);



 //Utility
 void addEdge(graph& G, vertex v, vertex u);
 bool hasEdge(graph& G, vertex v, vertex u);
 vertex CoordToVertex(index a, index b, vertex n1);
 
 