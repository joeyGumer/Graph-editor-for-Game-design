#include "wgraph.h"

//Struct constructed for utility
struct wedge
{
	wedge(vertex v1, vertex v2, weight we = 0)
	{
		v.first = v1;
		v.second = v2;
		w = we;
	}

	wedge(weight we = 0)
	{
		v.first = v.second = 0;
		w = we;
	}

	pair<vertex, vertex> v;
	weight w;
};

vector<wedge> getEdges(wgraph& G)
{
	vertex vn = G.size();
	vector<wedge> eG;
	vector<bool> Ev(vn,false);

	edge en = 0;
	for (index i = 0; i < vn; i++)
		en += G[i].size();
	en = en / 2;

	for (vertex i = 0; i < vn && eG.size() < en; i++)
	{
		for (vertex j = 0; j < G[i].size(); j++)
			if (!Ev[G[i][j].first])
			{
				wedge e(i, G[i][j].first, G[i][j].second);
				eG.push_back(e);
			}
		Ev[i] = true;
	}
	return eG;
}

weight wgraph_Kruskal(wgraph& G, ofstream& fout)
{
	vertex vn = G.size();

	//List of G edges to iterate faster
	vector<wedge> eG = getEdges(G);
	edge en = vn - 1;
	wgraph T(vn);
	weight tw = 0;

	fout << "\nKruskal method:-----------------------\n";
	fout<<"Minimal tree total edges:\n";

	//Step 1: get minimal weight edge
	wedge re = eG[0];
	edge in = 0;
	for (edge i = 1; i < eG.size(); i++)
	{
		if (re.w > eG[i].w)
		{
			re = eG[i];
			in = i;
		}
	}
	addEdge(T, re.v.first, re.v.second, re.w);
	fout << re.v.first << "-" << re.v.second << " :\t" << re.w << endl;

	//Step 2:construct minimal weight tree
	//for (edge count = 0; count < en; count++)
	while(true)
	{
		weight mw = UINT_MAX;
		edge in = UINT_MAX;
		for (edge i = 0; i < eG.size(); i++)
		{
			wedge e = eG[i];
			//Check if the edge is already in the tree
			//Check if the 2 vertices of the edge are disconnected in T, to avoid creating
			//We check first if the desired edge has a weight minor to the minimum registered
			//to avoid using wgraph_distance if possible, as it uses dijkstra
			if (!hasEdge(T, e.v.first, e.v.second) 
				&& mw > e.w 
				&& wgraph_distance(T, e.v.first, e.v.second) == UINT_MAX)
			{		
				mw = e.w;
				in = i;				
			}
		}
		if (in == UINT_MAX)
			break;
		addEdge(T, eG[in].v.first, eG[in].v.second, eG[in].w);
		fout << eG[in].v.first << "-" << eG[in].v.second << " :\t" << eG[in].w << endl;
	}

	//Components info using BFS
	vn = T.size();
	vector<bool> BFSl(vn, false);
	vector<vertex> BFSv(vn);
	index BFSn = 0;
	component c = 0;
	for (vertex rv = 0; rv < vn; rv++)
	{
		if (!BFSl[rv])
		{
			c++;
			fout << "\nComponent " << c << " :\n";
			weight cw = 0;
			index BFSi = BFSn;
			BFSv[BFSn++] = rv;
			BFSl[rv] = true;

			fout << "Vertices: ";
			fout << rv << ", ";
			if(T[rv].size())
				do
				{					
					vertex v = BFSv[BFSi++];
					for (index i = 0; i < T[v].size(); i++)
						if (!BFSl[T[v][i].first])
						{
							BFSl[T[v][i].first] = true;
							BFSv[BFSn++] = T[v][i].first;

							fout << T[v][i].first << ", ";
							cw += T[v][i].second;
						}
				} while (BFSi < BFSn);
				fout << "\nComponent total weight: " << cw <<endl;
				tw += cw;
		}
	}
	return tw;
}

weight wgraph_Prim(wgraph& G, ofstream& fout)
{
	vertex vn = G.size();
	weight tw = 0;
	vector<wedge> Te;
	vector<bool> Pv(vn, false); //Check if the vertex is already added in the minimal tree
	vector<bool> Pcv(vn, false);//Check if a vertex in a component is already visited

	wgraph T(vn);
	fout << "\nPrim method:-------------------\n";

	for (component c = 1;; c++)
	{
		//Get non-visited edge with minimal weight
		wedge se(UINT_MAX);
		weight cw = 0;
		for(vertex i = 0 ; i<vn; i++)
			if (!Pv[i])
			{
				if (G[i].size())
				{
					for (vertex j = 0; j < G[i].size(); j++)
						if (!Pv[G[i][j].first] && se.w > G[i][j].second)
						{
							se.v.first = i;
							se.v.second = G[i][j].first;
							se.w = G[i][j].second;
						}
				}
				else
				{				
					//Components of only one vertex
					fout << "\nComponent " << c << " :\n";
					fout << "Edges: none\n";
					fout << "Vertex: " << i << endl;
					c++;
					Pv[i] = true;
				}
			}

		if (se.w == UINT_MAX)
			break;

		fout << "\nComponent " << c << " :\n";
		fout << "Edges:\n";

		//Add root edge of the component
		Pcv[se.v.first] = Pcv[se.v.second] = Pv[se.v.first] = Pv[se.v.second] = true;
		Te.push_back(se);
		addEdge(T, se.v.first, se.v.second, se.w);
		cw += se.w;

		fout << se.v.first << "-" << se.v.second << ":\t" << se.w << endl;
		
		//Create minimal tree for the chosen edge
		//Look only for edges atached to visited vertices in the component
		//(That's the reason for Pcv)
		while(true)
		{
			wedge e(UINT_MAX);
			for (vertex i = 0; i < vn; i++)
				if (Pcv[i])
					for (edge j = 0; j < G[i].size(); j++)
						if (e.w > G[i][j].second && !Pcv[G[i][j].first])
						{
							e.v.first = i;
							e.v.second = G[i][j].first;
							e.w = G[i][j].second;
						}

			if (e.w == UINT_MAX)
				break;

			Te.push_back(e);
			addEdge(T, e.v.first, e.v.second, e.w);
			cw += e.w;
			Pcv[e.v.first] = Pcv[e.v.second] = Pv[e.v.first] = Pv[e.v.second] = true;
			fout << e.v.first << "-" << e.v.second << ":\t" << e.w << endl;
		} 

		//Show Component vertices info and reset component track list
		fout << "Vertices: ";
		for (vertex i = 0; i < vn; i++)
			if (Pcv[i])
			{
				fout << i << ", ";
				Pcv[i] = false;
			}
		
		tw += cw;
		fout << "\nComponent total weight: " << cw << endl;		
	}

	//List all edges
	fout << "\nMinimal tree total edges:\n";
	for (edge i = 0; i < Te.size(); i++)
		fout << Te[i].v.first << "-" << Te[i].v.second << ":\t"<< Te[i].w << endl;

	return tw;
}