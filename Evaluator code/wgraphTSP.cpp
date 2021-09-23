#include "wgraph.h"

bool HamiltonianCycle(wgraph& G, ofstream& fout)
{
	return false;
}
index HamiltonianCycles(wgraph& G, ofstream& fout)
{
	return 0;
}
weight TravellingSalesmanProblem(wgraph& G, ofstream& fout)
{
	vertex vn = G.size();

	//If the size it's 2 it's impossible it has a cycle
	if (vn <= 2)
	{
		fout << "0\t0\n";
		return 0;
	}

	vector<vertex> HCv;
	vector<bool> HCl(vn, false);
	vector<index> ind(vn, 0);
	vector<vertex> minimal_HCv;
	weight cycle_w = UINT_MAX;

	vertex rv = 0;
	HCv.push_back(rv);
	HCl[rv] = true;

	vertex v = rv;
	index nrv = G[rv].size();
	bool cycle = false;
	while (true)
	{
		if (ind[v] < G[v].size())
		{
			v = G[v][ind[v]++].first;

			if (HCl[v] == false)
			{
				//Next vertex is not visited
				HCv.push_back(v);
				HCl[v] = true;
			}
			else if (v == rv && HCv.size() == vn)
			{
				//We found a Hamiltonian cycle
				//check the weight
				weight w = 0;
				vertex u = 0;
				for (index i = 0; i < vn - 1; i++)
				{
					u = HCv[i];
					w += G[u][ind[u] - 1].second;
				}
				u = HCv.back();
				w += G[u][ind[u] - 1].second;

				if (w < cycle_w)
				{
					cycle_w = w;
					minimal_HCv = HCv;
				}

				cycle = true;

				//Backtrack to find other cycles
				v = HCv.back();
			}
			else //Vertex already visited and is not the root, backtrack
				v = HCv.back();
		}
		else
		{
			//If the vertex with no more neighbours to check is the root
			//we have checked all possible hamiltonian cycles, end while
			if (v == rv)
				break;

			//No more neighbours to check, reset the vertex info and backtrack
			HCv.pop_back();
			ind[v] = 0;
			HCl[v] = false;
			v = HCv.back();
		}
	}

	//We write the resulting cycle in case it exists, if not emit 0
	if (cycle)
	{
		fout << vn << "\t" << vn << endl;
		for (index i = 0; i < vn; i++)
			fout << minimal_HCv[i] << endl;
		for (index i = 0; i < vn - 1; i++)
			fout << minimal_HCv[i] << "\t" << minimal_HCv[i + 1] << endl;

		fout << minimal_HCv[vn-1] << "\t" << minimal_HCv[0] << endl;
		fout << cycle_w << endl;
	}
	else
		fout << "0\t0\n";

	return cycle_w;
}