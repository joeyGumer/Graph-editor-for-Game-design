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
 typedef unsigned int weight;
 typedef vector<vector<pair<vertex,weight>> > wgraph;


 enum EvaluationMode
 {
	 EVL_PATH,
	 EVL_DIJKSTRA,
	 EVL_TPS,
	 EVL_VCOLOR,
	 EVL_ECOLOR,
 };

 wgraph wgraph_complete(index n, weight Mw);
 wgraph wgraph_completeBipartite(index an, index bn, weight Mw);
 wgraph wgraph_cycle(index n, weight Mw);
 wgraph wgraph_star(index n, weight Mw);
 wgraph wgraph_wheel(index n, weight Mw);
 wgraph wgraph_read( string fname, int* mode, vector<vertex>* extra = NULL);
 void wgraph_write(  wgraph& G, ofstream& fout );
 pair<vector<weight>, vector<vertex>> wgraph_dijkstra(wgraph& G, vertex sv);
 weight wgraph_distance(wgraph& G, vertex av, vertex bv);
 weight wgraph_maxDistance(wgraph& G, vertex sv);
 weight wgraph_diameter(wgraph& G);
void wgraph_writeDistances(wgraph& G, vertex sv, ofstream& fout);
void wgraph_writeMinimalPaths(wgraph& G, vertex sv, ofstream& fout);
void wgraph_writeMinimalPath(wgraph& G, vertex sv, vertex ev, ofstream& fout);
void wgraph_writeDijkstra(wgraph& G, vertex sv, ofstream& fout);

//Exercise 19
bool HamiltonianCycle(wgraph& G, ofstream& fout);
index HamiltonianCycles(wgraph& G, ofstream& fout);
weight TravellingSalesmanProblem(wgraph& G, ofstream& fout);
//
weight wgraph_Kruskal(wgraph& G, ofstream& fout);
weight wgraph_Prim(wgraph& G, ofstream& fout);
//Utility
void addEdge(wgraph& wG, vertex v, vertex u, weight w);
bool hasEdge(wgraph& wG, vertex v, vertex u);
 