#include "wgraph.h"

//Utilities
//This would be better if graph was a class or a struct, and this function a method
void addEdge(wgraph &wG, vertex v, vertex u, weight w)
{
    wG[v].push_back(pair<vertex,weight>(u,w));
    wG[u].push_back(pair<vertex, weight>(v,w));
}

bool hasEdge(wgraph& wG, vertex v, vertex u)
{
    int d = wG[v].size();
    for (int i = 0; i < d; i++)
        if (wG[v][i].first == u)
            return true;

    return false;
}
// 
//  Weighted complete graph with vn vertices and random weights up to Mw 
//  to weighted adjacencies lists (wgraph) 
//


 wgraph
    wgraph_complete( index n,  weight Mw )
 {    
    vertex vn = n;
    wgraph WKn( vn );
    for( vertex v = 0; v < vn; v++ ) 
        for( vertex u = v+1; u<vn; u++ ) {
			weight w = rand()%Mw+1;
			WKn[v].push_back(pair<vertex,weight>(u,w));
			WKn[u].push_back(pair<vertex,weight>(v,w));
        }
    return WKn;
}

 //
 //EXERCISE 8
 //
 //Complete bipartite graph
 wgraph wgraph_completeBipartite(index an, index bn, weight Mw)
 {
     vertex vn = an + bn;
     wgraph Kab(vn);
     for (vertex v = 0; v < an; v++)
         for (vertex u = an; u < vn; u++)
         {
             weight w = rand() % Mw + 1;
             addEdge(Kab, v, u, w);
         }

     return Kab;
 }

 //Cycle graph
 wgraph wgraph_cycle(index n, weight Mw)
 {
     vertex vn = n;
     wgraph Cn(vn);
     for (vertex v = 0; v < vn - 1; v++)
     {
         weight w = rand() % Mw + 1;
         addEdge(Cn, v, v + 1, w);
     }
     weight w = rand() % Mw + 1;
     addEdge(Cn, vn - 1, 0, w);

     return Cn;
 }

 //Star graph
 wgraph wgraph_star(index n, weight Mw)
 {
     vertex vn = n + 1;
     wgraph Sn(vn);
     for (vertex v = 0; v < n; v++)
     {
         weight w = rand() % Mw + 1;
         addEdge(Sn, v, n, w);
     }
     
     return Sn;
 }

 //Wheel graph
 wgraph wgraph_wheel(index n, weight Mw)
 {
     vertex vn = n + 1;
     wgraph Wn(vn);
     for (vertex v = 0; v < n-1; v++)
     {
         weight w = rand() % Mw + 1;
         addEdge(Wn, v, v + 1, w);

         w = rand() % Mw + 1;
         addEdge(Wn, v, n, w);
     }
     weight w = rand() % Mw + 1;
     addEdge(Wn, n - 1, 0, w);

     w = rand() % Mw + 1;
     addEdge(Wn, n - 1, n, w);

     return Wn;
 }

// 
//  Read from file fname of weighted edges to weighted adjacencies lists
//
 wgraph
	wgraph_read( string fname , int* mode, vector<vertex>* extra)
 {    
	ifstream fin;
    fin.open(fname.c_str());    
    if (fin.fail()) {
        cout << "unable to open file " << fname.c_str() << " for reading" << endl;
        cin.get();
        exit(1);
    }
    index gr_type;
 	vertex vn;
    edge en;
    fin >> gr_type;
    fin >> vn >> en;
	wgraph G( vn );
    for( edge e=0; e<en; e++ ) {
        vertex u, v;
        weight w;
        fin >> u >> v >> w; 
        G[u].push_back(pair<vertex,weight>(v,w));
        if( u != v ) 
            G[v].push_back(pair<vertex,weight>(u,w));
    }    

    
    fin >> *mode;
    if (*mode == EVL_PATH)
    {
        vertex v;
        for (int i = 0; i < 2; i++)
        {
            fin >> v;
            extra->push_back(v);
        }        
    }
    else if (*mode == EVL_DIJKSTRA)
    {
        vertex v;
        fin >> v;
        extra->push_back(v);
    }
    return G;
}
// 
//  Write from weighted adjacencies lists (wgraph) to stream
//  - vertices and edges numbers
//  - weighted edges
//  - degrees and sorted degrees
  void 
	wgraph_write( wgraph& G, ofstream& fout )
 {    
//  Vertices number 
	vertex vn = G.size();
//  Count edges number from adajacency matrix
    edge en = 0; 
    for( vertex v = 0; v < vn; v++) 
         en += G[v].size();
    en /= 2;
//  Write edges         
    fout << "WGraph with " << vn << " vertices and " << en << " edges " << endl;
    fout << "Weighted adjacency lists" << endl;
//  Allocate/read weighted adjacent pairs
    for( vertex v = 0; v < vn; v++ ) {
        fout << v << ":";
        for( index i = 0; i < G[v].size(); i++)  
            fout << " " << G[v][i].first << "[" << G[v][i].second << "] ";
        fout << endl;
    }
    fout << endl;
    fout << "Weighted edges" << endl;
    for( vertex v = 0; v < vn; v++ ) 
        for( index i = 0; i < G[v].size(); i++) 
            if( v <= G[v][i].first )
                fout << v << "-" << G[v][i].first <<  "[" << G[v][i].second << "] " << endl;
             
 }

  
