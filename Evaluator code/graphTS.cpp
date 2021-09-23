//
//	TREE SEARCH IN GRAPHS
//  - BFS method
//  - DFS method
//  - Dijkstra (without weights) method
//
#include "graph.h"
//
//  Breadth First Search: returns components/trees number (connected components)
//
 component
    BFS( graph &G ) 
{
    vertex vn = G.size();
    component Tn = 0;
//  Vertices list
	vector<vertex> BFSv( vn );
//  Label vertices as unvisited
	vector<bool> BFSl( vn, false );
//  List index
	index BFSn = 0;
//  Find non labeled vertex as root vertex rv of new component/tree Tn
	for( vertex rv=0; rv<vn; rv++ ) 
		if( !BFSl[rv] ) {
		//  Actualize display index
			index BFSi = BFSn;
		//  New root rv found for new component
			Tn++;
		//  Label and list rv
			BFSl[rv] = true;
			BFSv[BFSn++] = rv;
			if( G[rv].size() )
			do {
			//  New vertex v to display 
				vertex v = BFSv[BFSi++]; 				
				for( index i=0; i<G[v].size(); i++ ) 
					if( !BFSl[G[v][i]] ) {
					//  Label and list adjacent G[v][i]
						BFSl[G[v][i]] = true;
						BFSv[BFSn++] = G[v][i];
					}
			} while( BFSi < BFSn );
		}
   return Tn;
}
//
//  Recursive function for Depth First Search from v
//
 void 
    DFS( graph &G, vector<bool> &DFSl,  vertex v ) 
{
	for( index i=0; i<G[v].size(); i++ ) 
        if( !DFSl[G[v][i]] ) {
		//  Label adjacent vertex as visited
            DFSl[G[v][i]] = true;
		//  DFS recursive for non visited adjacent vertices   
            DFS( G, DFSl, G[v][i] );
        }
}
//
//  Depth First Search: returns components/trees number
//
 component 
    DFS(  graph &G ) 
{
    vertex vn = G.size();
//  Initialize number of components/trees to 0
    component Tn = 0;
//  Label vertices as unvisited
    vector<bool> DFSl( vn, false );
    for( vertex rv=0; rv<vn; rv++) 
         if( !DFSl[rv] ){
		 //  New root found
		 //  Count new component
			 Tn++;
		 //  Label root vertex
			 DFSl[rv] = true;
		 //  Display component
             DFS( G, DFSl, rv );
         }
    return Tn;
}
//
//  Minimal path lengths by Dijkstra's method to stream
//  - Distances to all vertices: lengths of minimal paths
//
 void
    Dijkstra(  graph &G,  vertex sv, ofstream& fout ) 
{
    vertex vn = G.size();
//  Inialize vertices as unlabeled
    vector<bool> Dl( vn, false );
//  Infinity for lengths
	length infty = UINT_MAX;
//  Inialitize distance to sv to infty for all vertices but sv
	vector<length> Dd( vn, infty );
	Dd[sv]=0;
//  Vertex mdv with minimum distance to starting vertex sv: sv
	vertex mdv = sv; 
	do {
	//  Label mdv and check distance from sv to its adjacents	
		Dl[mdv] = true;
		for( index i=0; i<G[mdv].size(); i++ ) 
			if( Dd[G[mdv][i]] > Dd[mdv]+1 )	
				Dd[G[mdv][i]] = Dd[mdv]+1; 
	//  Find new not labeled vertex mdv with minimum distance to sv
		length md = infty;
		mdv = vn;
		for( vertex v=0; v<vn; v++ )
			if( md > Dd[v] && !Dl[v] ) {
				md = Dd[v];
				mdv = v;
			}
	} while( mdv<vn );
    fout << "\nDistances from starting vertex " << sv << endl;
	for( vertex v=0; v<vn; v++ )
        if( Dd[v]<infty )
		    fout << v << "\t" << Dd[v]  << endl;
        else
            fout << v << "\tNot connected" << endl;
}

 //
 //EXERCISE 9
 //
 void BFS_Trees(graph &G, ofstream &fout)
 {
	 vertex vn = G.size();
	 graph H(vn);// Graph formed by the spanning trees of the components
	 component Tn = 0;
	 vector<vertex> BFSv(vn);
	 vector<bool> BFSl(vn, false);
	 index BFSn = 0;

	 vector<length> BFSd(vn, 0); // Vector to store the depth of each vertex 

	 fout << "Vertices\n";
	 for (vertex rv = 0; rv < vn; rv++)
	 {
		 if (!BFSl[rv])
		 {
			 index BFSi = BFSn;
			 index Ti = 0;
			 Tn++;

			 BFSl[rv] = true;
			 BFSv[BFSn++] = rv;

			 fout << "Component BFS tree " << Tn << "\n";
			 fout << "(Root) " << rv << ":\tIndex: 0" << ",\tDepth: 0\n";

			 do
			 {
				 vertex v = BFSv[BFSi++];
				 vertex neigh = G[v].size();
				 for (index i = 0; i < neigh; i++)
				 {
					 if (!BFSl[G[v][i]])
					 {
						 vertex u = G[v][i];
 						 BFSd[u] = BFSd[v] + 1;	
						 BFSl[u] = true;
						 BFSv[BFSn++] = u;

						 fout << u << ":\tIndex: " << ++Ti << ",\tParent: " << v << ",\tDepth: " << BFSd[u] << "\n";
						 addEdge(H, v, u);
					 }
				 }
			 } while (BFSi < BFSn);
		 }
	 }
	 //Edges
	 fout << "Edges\n";
	 for (vertex v = 0; v < vn; v++)
		 for (vertex i = 0; i < G[v].size(); i++)
		 {
			 vertex u = G[v][i];
			 if (v <= u)
			 {
				 string ans("No");
				 if (hasEdge(H, v, u))
					 ans = "Yes";
				 fout << v << "-" << u << ", In trees: " << ans.c_str() << "\n";					
			 }
		 }
 }

 void DFS_Trees(graph& G, graph& H, vector<bool>& DFSl, vertex v, index &Ti, length d, ofstream &fout)
 {
	 for (index i = 0; i < G[v].size(); i++)
	 {
		 vertex u = G[v][i];
		 if (!DFSl[u]) {
			 //  Label adjacent vertex as visited
			 DFSl[u] = true;
			 fout << u << ":\tIndex: " << ++Ti << ",\tParent: " << v << ",\tDepth: " << d+1 << "\n";
			 addEdge(H, v, u);
			 //  DFS recursive for non visited adjacent vertices   
			 DFS_Trees(G, H, DFSl, u, Ti, d + 1, fout);
		 }
	 }
 }
 component DFS_Trees(graph &G, ofstream &fout)
 {
	 vertex vn = G.size();
	 component Tn = 0;

	 vector<bool> DFSl(vn, false);
	 graph H(vn);
	 for (vertex rv = 0; rv < vn; rv++)
		 if (!DFSl[rv]) {

			 Tn++;
			 DFSl[rv] = true;
			 index Ti = 0;

			 fout << "Component DFS tree " << Tn << "\n";
			 fout << "(Root) " << rv << ":\tIndex: 0" << ",\tDepth: 0\n";

			 //  Display component
			 DFS_Trees(G, H, DFSl, rv, Ti, 0, fout);
		 }
	 //Edges
	 fout << "Edges\n";
	 for (vertex v = 0; v < vn; v++)
		 for (vertex i = 0; i < G[v].size(); i++)
		 {
			 vertex u = G[v][i];
			 if (v <= u)
			 {
				 string ans("No");
				 if (hasEdge(H, v, u))
					 ans = "Yes";
				 fout << v << "-" << u << ", In trees: " << ans.c_str() << "\n";
			 }
		 }
	 return Tn;
 }

 //Exercise 10
 void chessDijkstra(graph& G, index n1, index n2, vertex sv, ofstream& fout)
 {
	 vertex vn = G.size();
	 //  Inialize vertices as unlabeled
	 vector<bool> Dl(vn, false);
	 //  Infinity for lengths
	 length infty = UINT_MAX;
	 //  Inialitize distance to sv to infty for all vertices but sv
	 vector<length> Dd(vn, infty);
	 Dd[sv] = 0;
	 //  Vertex mdv with minimum distance to starting vertex sv: sv
	 vertex mdv = sv;
	 do {
		 //  Label mdv and check distance from sv to its adjacents	
		 Dl[mdv] = true;
		 for (index i = 0; i < G[mdv].size(); i++)
			 if (Dd[G[mdv][i]] > Dd[mdv] + 1)
				 Dd[G[mdv][i]] = Dd[mdv] + 1;
		 //  Find new not labeled vertex mdv with minimum distance to sv
		 length md = infty;
		 mdv = vn;
		 for (vertex v = 0; v < vn; v++)
			 if (md > Dd[v] && !Dl[v]) {
				 md = Dd[v];
				 mdv = v;
			 }
	 } while (mdv < vn);
	 /*fout << "\nDistances from starting vertex " << sv << endl;
	 for (vertex v = 0; v < vn; v++)
		 if (Dd[v] < infty)
			 fout << v << "\t" << Dd[v] << endl;
		 else
			 fout << v << "\tNot connected" << endl;*/

	 fout << "\nDegrees and distances from starting vertex " << sv << endl;
	 for (index i = 0; i < n1; i++)
	 {
		 for (index j = 0; j < n2; j++)
			 fout << "\t" << G[CoordToVertex(i, j, n1)].size();
		
		 fout << "\t";
		 for (index j = 0; j < n2; j++)
		 {
			 if (Dd[CoordToVertex(i, j, n1)] != infty)
				 fout << "\t" << Dd[CoordToVertex(i, j, n1)];
			 else
				 fout << "\tX";
		 }

		 fout << endl;
	 }

 }