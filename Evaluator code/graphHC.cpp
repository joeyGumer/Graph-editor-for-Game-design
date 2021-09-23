#include "graph.h"


bool HamiltonianCycle(graph& G, ofstream& fout)
{
	vertex vn = G.size();

	if (vn <= 2)
	{
		fout << "there's no Hamiltonian cycle\n";
		return false;
	}

	vector<vertex> HCv;
	vector<bool> HCl(vn, false);
	vector<index> ind(vn, 0);

	vertex rv = 0;
	HCv.push_back(rv);
	HCl[rv] = true;

	vertex v = rv;
	bool cycle = false;
	while (true)
	{
		if (ind[v] < G[v].size())
		{
			v = G[v][ind[v]++];

			if (HCl[v] == false)
			{
				HCv.push_back(v);
				HCl[v] = true;
			}
			else if (v == rv && HCv.size() == vn)
			{
				cycle = true;
				break;
			}
			else
				v = HCv.back();
		}
		else 
		{ 
			if (v == rv)
				break;

			HCv.pop_back();
			ind[v] = 0;
			HCl[v] = false;
			v = HCv.back();
		}
	}

	fout << "Hamiltonian cycle: ";
	if (cycle)
	{
		fout << rv;
		for (index i = 1; i < vn; i++)
			fout << "-" << HCv[i];
		fout << endl;
	}
	else
		fout << "there's no Hamiltonian cycle\n";

	return cycle;
}
index HamiltonianCycles(graph& G, ofstream& fout)
{
	vertex vn = G.size();

	if (vn <= 2)
		return 0;

	vector<vertex> HCv;
	vector<bool> HCl(vn, false);
	vector<index> ind(vn, 0);
	index HCn = 0;

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
			v = G[v][ind[v]++];

			if (HCl[v] == false)
			{
				HCv.push_back(v);
				HCl[v] = true;
			}
			else if (v == rv && HCv.size() == vn)
			{
				//We have another cycle
				HCn++;
				fout << "Hamiltonian cycle "<<HCn<<": ";
				fout << rv;
				for (index i = 1; i < vn; i++)
					fout << "-" << HCv[i];
				fout << endl;

				cycle = true;
				v = HCv.back();
				//break;
			}
			else
				v = HCv.back();
		}
		else
		{
			if (v == rv)
				break;

			HCv.pop_back();
			ind[v] = 0;
			HCl[v] = false;
			v = HCv.back();
		}
	}

	if(!cycle)
		fout << "There's no Hamiltonian cycle\n";

	return HCn;
}