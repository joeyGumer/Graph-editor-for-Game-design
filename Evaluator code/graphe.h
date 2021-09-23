#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include "graph.h"
using namespace std;
typedef pair<vertex, index> vip;
typedef map<vip, edge> edges;


//Utilityedge countEdges(graph& G, edges& GE);
//Methods
void graphe_complete(graph& Kn, edges& KnE, index n);
void graphe_cycle(graph& Cn, edges& CnE, index n);
void graphe_bipartite_complete(graph& Kn1n2, edges& Kn1n2E, index n1, index n2);
void graphe_star(graph& Sn, edges& SnE, index n);
void graphe_wheel(graph& Wn, edges& WnE, index n);
void line_graphe(graph& G, edges& GE, graph& LG, edges& LGE);
int graphe_read(graph& G, edges& GE, string fname);
void graphe_write(graph& G, edges& GE, ofstream& fout);
color MinimalVertexColoring(graph& G, vector<color>& Gc, ofstream& fout);
color MinimalEdgeColoring(graph& G, edges& GE, ofstream& fout);
bool Hierholzer(graph& G, edges& GE, ofstream& fout);