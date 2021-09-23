#include "graphe.h"

//For some reason it gives an error if i don't declare countEdges here
//despite being declared at graphe.h
edge countEdges(graph& G, edges& GE);
bool Hierholzer(graph& G, edges& GE, ofstream& fout)
{
	vertex vn = G.size();
	edge en = countEdges(G,GE);
	
	bool eulerian = true;
	//Check if it is an eulerian graph
	for (vertex v = 0; v < vn; v++)//if any vertex degree is not pair, G not eulerian
	{
		index degree = G[v].size();
		for (index i = 0; i < G[v].size(); i++)//So loops are counted twice
			if (G[v][i] == v)
				degree++;

		if (degree % 2 == 1)
			eulerian = false;
	}

	//Check if G is connected
	if ( GE.size() == 0 || BFS(G) != 1)
		eulerian = false;

	//End if the graph is not eulerian
	if (!eulerian)
	{
		fout << "This graph is not eulerian\n";
		return eulerian;
	}

	//Run Hierholzer if eulerian
	vector<vertex> ECv;
	vector<bool> Gl(en, true);

	vector<index> rdeg(vn);
	for (vertex v = 0; v < vn; v++)
		rdeg[v] = G[v].size();

	vertex v0 = 0;
	//Fast way to know if the eulerian tour is complete
	do
	{
		//WARNING: CHECK FOR BETTER OPTIONS; BFS OR DFS?
		//We find a cycle containing V0
		//Using a similar algorithm to the one used to find Hamiltonian cycles
		vector<vertex> Cv;
		vector<bool> Cl(vn, false);
		vector<index> Ci(vn, 0);
		vertex v = v0;
		Cv.push_back(v);
		Cl[v] = true;
		while (true)
		{
			while (Ci[v] < G[v].size() && !Gl[GE[{v, Ci[v]}]])
				Ci[v]++;

			if (Ci[v] < G[v].size())
			{
				v = G[v][Ci[v]++];

				if (Cl[v] == false)
				{
					Gl[GE[{Cv.back(), Ci[Cv.back()] - 1}]] = false;
					Cv.push_back(v);
					Cl[v] = true;
				}
				else if (v == v0)
				{
					//A cycle is found
					Gl[GE[{Cv.back(), Ci[Cv.back()] - 1}]] = false;//Closing edge
					for (index i = 0; i < Cv.size(); i++)
					{
						v = Cv[i];
						ECv.push_back(v);
						rdeg[v] -= 2;
					}
					break;
				}
				else
					v = Cv.back();
			}
			else
			{
				Cv.pop_back();
				Ci[v] = 0;
				Cl[v] = false;
				v = Cv.back();
				Gl[GE[{v, Ci[v] - 1}]] = true;
			}
		}

		//Add the cycle to the eulerian circuit being built
		for (index i = 0; i < ECv.size(); i++)
		{
			v = ECv[i];
			if (rdeg[v] > 0)
			{
				v0 = v;
				rotate(ECv.begin(), ECv.begin() + i, ECv.end());
				break;
			}
		}
		
	} while (ECv.size()< en);

	//Stream resulting circuit
	fout << "Eulerian circuit: " << ECv[0];
	for (index i = 1; i < ECv.size(); i++)
		fout << " - " << ECv[i];
	fout << " - " << ECv[0] << endl;

	return eulerian;
}