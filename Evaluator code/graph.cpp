#include "graph.h"
//Utilities
//This would be better if graph was a class or a struct, and this function a method
void addEdge(graph &G, vertex v, vertex u)
{
    G[v].push_back(u);
    G[u].push_back(v); 
}

bool hasEdge(graph& G, vertex v, vertex u)
{
    int d = G[v].size();
    for (int i = 0; i < d; i++)
        if (G[v][i] == u)
            return true;

    return false;
}

vertex CoordToVertex(index a, index b, vertex n1)
{
    return a + b * n1;
}

// 
//  Complete graph with vn vertices to adjacencies lists (graph)
//
 graph
    graph_complete( index n )
{    
    vertex vn = n;
    graph Kn( vn );
    for( vertex v=0; v<vn; v++) 
        for( vertex u=v+1; u<vn; u++ ) {
             Kn[v].push_back( u );
             Kn[u].push_back( v );
        } 
    return Kn;
}

//
// EXERCISE 8 -----------------------
//

//Complete bipartite graph
 graph graph_completeBipartite(index an, index bn)
 {
     vertex vn = an + bn;
     graph Kab(vn);
     for (vertex v = 0; v < an; v++)
         for (vertex u = an; u < vn; u++)
             addEdge(Kab, v, u);

     return Kab;        
 }

 //Cycle graph
 graph graph_cycle(index n)
 {
     vertex vn = n;
     graph Cn(vn);
     for (vertex v = 0; v < vn-1; v++)
         addEdge(Cn, v, v + 1);
     addEdge(Cn, vn - 1, 0);
 
     return Cn;
 }

 //Star graph
 graph graph_star(index n)
 {
     vertex vn = n + 1;
     graph Sn(vn);
     for (vertex v = 0; v < n; v++)
         addEdge(Sn, v, n);

     return Sn;
 }

 //Wheel graph
 graph graph_wheel(index n)
 {
     vertex vn = n + 1;
     graph Wn(vn);
     for (vertex v = 0; v < n-1; v++)
     {
         addEdge(Wn, v, v+1);
         addEdge(Wn, v, n);
     }
     addEdge(Wn, n-1, 0);
     addEdge(Wn, n - 1, n);

     return Wn;
 }
 //
 //Exercise 10
 //

 //King graph
 graph graph_king(index n1, index n2)
 {
     vertex vn = n1 * n2;
     graph G(vn);

     for (index i = 0; i < n1; i++)
         for (index j = 0; j < n2; j++)
         {
             for (int t = 0; t < 4; t++)
             {
                 int a = i;
                 int b = j;
                 switch(t)
                 {
                 case 0:
                     a++;
                     break;
                 case 1:
                     b++;
                     break;
                 case 2:
                     a++,b++;
                     break;
                 case 3:
                     a--, b++;
                     break;               
                 }
                 if (a >= 0 && a < n1 && b < n2)//not needed to check b--
                     addEdge(G, CoordToVertex(i, j, n1), CoordToVertex(a, b, n1));                    
             }
         }

     return G;
 }

 //Rook graph
 graph graph_rook(index n1, index n2)
 {
     vertex vn = n1 * n2;
     graph G(vn);

     for (index i = 0; i < n1; i++)
         for (index j = 0; j < n2; j++)
         {
             for (int a = i + 1; a < n1; a++)
                 addEdge(G, CoordToVertex(i, j, n1), CoordToVertex(a, j, n1));
             for (int b = j + 1; b < n2; b++)
                 addEdge(G, CoordToVertex(i, j, n1), CoordToVertex(i, b, n1));
         }

     return G;
 }

 //Bishop graph
 graph graph_bishop(index n1, index n2)
 {
     vertex vn = n1 * n2;
     graph G(vn);

     for (index i = 0; i < n1; i++)
         for (index j = 0; j < n2; j++)
         {
             for (int a = i + 1, b = j + 1; a < n1 && b < n2; a++, b++)
                 addEdge(G, CoordToVertex(i, j, n1), CoordToVertex(a, b, n1));
             for (int a = i - 1, b = j + 1; a >= 0 && b < n2; a--, b++)
                 addEdge(G, CoordToVertex(i, j, n1), CoordToVertex(a, b, n1));
         }

     return G;
 }
  
 //Knight graph
 graph graph_knight(index n1, index n2)
 {
     vertex vn = n1 * n2;
     graph G(vn);

     for (index i = 0; i < n1; i++)
         for (index j = 0; j < n2; j++)
         {
             for (int t = 0; t < 4; t++)
             {
                 int a = i;
                 int b = j;
                 switch (t)
                 {
                 case 0:
                     a -= 2; b++;
                     break;
                 case 1:
                     a--; b+=2;
                     break;
                 case 2:
                     a++, b+=2;
                     break;
                 case 3:
                     a+=2, b++;
                     break;
                 }
                 if (a >= 0 && a < n1 && b < n2)//not needed to check b--
                     addEdge(G, CoordToVertex(i, j, n1), CoordToVertex(a, b, n1));
             }
         }

     return G;
 }

// 
//  Read from file fname of edges to adjacencies lists (graph)
//
 graph 
	graph_read( string fname )
{    
	ifstream fin;
    fin.open( fname.c_str() );    
    if( fin.fail() ) {
        cerr << "unable to open file " << fname.c_str() << " for reading" << endl;
        exit( 1 );
    }
 	vertex vn;
    edge en;
	fin >> vn >> en;
	graph G( vn );
    for( edge e=0; e<en; e++ ) {
         vertex v, u;
         fin >> v >> u; 
         G[v].push_back(u);
         if( u != v ) 
             G[u].push_back(v);
	}    
    return G;
}
// 
//  Write from adjacencies lists (graph) to stream
//  - vertices and edges numbers
//  - edges
  void 
	graph_write(  graph& G, ofstream& fout )
{    
//  Vertices number 
	vertex vn = G.size();
//  Count edges number from adjacencies lists
//  PLEASE CORRECT WHEN THERE ARE LOOPS (this should already print loops)
    edge en = 0; 
    for( vertex v = 0; v < vn; v++ ) 
         en += G[v].size();
    en /= 2;
//  Write edges         
    fout << "Graph with " << vn << " vertices and " << en << " edges " << endl;
    fout << "Adjacencies lists" << endl;
    for( vertex v=0; v<vn; v++ )  {
		fout << v << "\t:";
        for( index i=0; i<G[v].size(); i++) 
            fout << "\t" << G[v][i];
        fout << endl;
    }
    fout << "Edges" << endl;
    for( vertex v = 0; v < vn; v++ ) 
        for( index i = 0; i < G[v].size(); i++ ) 
            if( v <= G[v][i] )
                fout << v << "-" << G[v][i] << endl;
}


