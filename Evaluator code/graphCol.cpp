#include "graph.h"
#include "graphe.h"

//Checks with which color a vertex v has to be colored to not have the same color as it's neighbours
//it is not limited by the maximum color
color ColorVertex(graph& G, vector<color>& COLv, color  maxCol, vertex v, color minCol = 1)
{
	if (v >= G.size())
		return 0;

	//We use a vector to check if a color is being used by the neighbours
	//So that we don't need to reiterate each time we need to check for a new color
	vector<bool> COLl(maxCol + 1, false);
	for (vertex j = 0; j < G[v].size(); j++)
		if( G[v][j]< v)
			COLl[COLv[G[v][j]]] = true;

	//Check for the minimum color of the list that is not being used by the neighbours
	color col;
	for (col = minCol; col <= maxCol; col++)
		if (!COLl[col])
			break;

	COLv[v] = col;

	return col;
}

color ColorationBrelaz(graph& G, vector<color>& colors, ofstream& fout)
{
	vertex vn = G.size();
	vector<color> COLv(vn, 0); // vector that stores the colors of each vertex
	color maxCol = 1; //maximum color

	for (vertex i = 0; i < vn; i++)
	{
		//We count a vertex as uncolored if the color is 0
		if (COLv[i] == 0)
		{
			color c = ColorVertex(G, COLv, maxCol, i);
			if (c > maxCol)
				maxCol = c;
		}
	}
	
	//Return the number of colors used by Brelaz coloration method
	colors = COLv;
	return maxCol;

}
color MinimalVertexColoring(graph& G, vector<color>& res, ofstream& fout)
{
	vertex vn = G.size();
	vector<color> COLv(vn, 0);
	color cn = ColorationBrelaz(G, COLv, fout);
	res = COLv;
	vertex cnv = 0;
	bool stop = true;

	//If cn is smaller than two, it cannot be reduced more, unless G has no edges
	//in which case the initial cn would already be one by Brelaz method
	while (cn > 2)
	{
		for (cnv = 0; cnv < vn; cnv++)
			if (COLv[cnv] == cn)
				break;

		while (cnv < vn)
		{
			do
			{
				cnv--;
				if (cnv == 0)
				{
					//Print the result
					fout << vn << "\t" << cn << endl;
					for (vertex i = 0; i < vn; i++)
						fout << res[i] << endl;

					return cn;
				}

			} while (ColorVertex(G, COLv, cn, cnv, COLv[cnv] + 1) >= cn);

			for (cnv += 1; cnv < vn && ColorVertex(G, COLv, cn, cnv) < cn; cnv++);
		}
		cn--;
		res = COLv;
	} 

	//Print the result
	fout << vn << "\t" << cn << endl;
	for (vertex i = 0; i < vn; i++)
		fout << res[i] << endl;

	return cn;
}

color MinimalEdgeColoring(graph& G, edges&GE, ofstream& fout)
{
	//Create the line graph, stored in LG
	graph LG;
	edges LGE;
	line_graphe(G, GE, LG, LGE); 

	//Calculate the minimal vertex coloring of LG
	vector<color> COLe;
	color cn = MinimalVertexColoring(LG, COLe, fout);
	edge en = LG.size();

	return cn;
}