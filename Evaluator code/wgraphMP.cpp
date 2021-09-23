#include "wgraph.h"

//
// Exercise 11
//
//we make dijkstra return the distances and predecessor vectors
pair<vector<weight>, vector<vertex>> wgraph_dijkstra(wgraph& G, vertex sv)
{
    vertex vn = G.size();//vertex number

    weight infty = UINT_MAX;
    vector<bool> Dl(vn, false); //Checked vertices vector
    vector<weight> Dd(vn, infty); //Registered distances vector, starts with maximum distances
    vector<vertex> P(vn, vn); //Predecessors vector
    Dd[sv] = 0;
    vertex mdv = sv;

    do {
        //  Label mdv and check distance from sv to its adjacents	
        Dl[mdv] = true;
        for (index i = 0; i < G[mdv].size(); i++)
        {
            pair<vertex, weight> v = G[mdv][i];
            //Check if the current registered vertex distance is greater than going through
            //the edge joining the current mdv to v, if so, put the last as a new distance
            //as it is smaller, and mdv as the new predecessor to v
            if (Dd[v.first] > Dd[mdv] + v.second)
            {
                Dd[v.first] = Dd[mdv] + v.second;
                P[v.first] = mdv;
            }
        }

        //  Find new not labeled vertex mdv with minimum distance to sv
        weight md = infty;
        mdv = vn;
        for (vertex v = 0; v < vn; v++)
            if (md > Dd[v] && !Dl[v]) {
                md = Dd[v];
                mdv = v;
            }
    } while (mdv < vn);

    return { Dd, P };
}

//Distance function
weight wgraph_distance(wgraph& G, vertex av, vertex bv)
{
    return wgraph_dijkstra(G, av).first[bv];
}

//Max distance function
weight wgraph_maxDistance(wgraph& G, vertex sv)
{
    vector<weight> Dd = wgraph_dijkstra(G, sv).first;
    index n = G.size();
    weight md = 0;
    for (int i = 0; i < n; i++)
        if (md < Dd[i] && Dd[i] != UINT_MAX)
            md = Dd[i];

    return md;
}

//Diameter function
weight wgraph_diameter(wgraph& G)
{
    index n = G.size();
    weight md = 0;

    for (int i = 0; i < n; i++)
    {
        weight vmd = wgraph_maxDistance(G, i);
        if (md < vmd)
            md = vmd;
    }
    return md;
}

//Write distance from source vertex to all other vertices
//(disconnected if not in the same component (i.e. infinite distance)
void wgraph_writeDistances(wgraph& G, vertex sv, ofstream& fout)
{
    vertex vn = G.size();
    vector<weight> Dd = wgraph_dijkstra(G, sv).first;

    fout << endl <<"Distances from vertex " << sv << " :" << endl;

    for (vertex i = 0; i < vn; i++)
    {
        if (Dd[i] < UINT_MAX)
            fout << i << ":\t" << Dd[i]<< endl;
        else
            fout << i << ":\tdisconnected"<< endl;
    }
}

//Write all minimal paths between the source vertex to all other vertices
void wgraph_writeMinimalPaths(wgraph& G, vertex sv, ofstream& fout)
{
    vertex vn = G.size();
    vector<vertex> P = wgraph_dijkstra(G, sv).second;

    fout << endl << "Minimal paths from vertex " << sv << " :" << endl;

    for (vertex i = 0; i < vn; i++)
    {
        fout << i << ":\t";
        if (P[i] != vn)
        {
            vertex p = P[i];
            fout << i;
            while (p != sv)
            {
                fout << " - " << p;
                p = P[p];
            }
            fout << " - " << sv << endl;
        }
        else if (i != sv)
            fout << "disconnected" << endl;
        else
            fout << "source" << endl;
    }
}

void wgraph_writeMinimalPath(wgraph& G, vertex sv, vertex ev, ofstream& fout)
{
    vertex vn = G.size();
    pair<vector<weight>,vector<vertex>> dijkstra = wgraph_dijkstra(G, sv);
    vector<vertex> P = dijkstra.second;
    vector<vertex> result;
    weight distance = dijkstra.first[ev];
    if (P[ev] != vn)
    {
        vertex p = P[ev];
        result.push_back(ev);
        while (p != sv)
        {
            result.push_back(p);
            p = P[p];
        }
        result.push_back(sv);
    }
    else
    {
        fout << "0" << endl;
        return;
    }

    vn = result.size();
    edge en = vn - 1;

    fout << vn << "\t" << en << endl;
    for (int i = 0; i < vn; i++)
    {
        fout << result[i]<< endl;
    }
    for (int i = 0; i < en; i++)
    {
        fout << result[i] << "\t" << result[1 + i] << endl;
    }
    fout << distance << endl;
}

void wgraph_writeDijkstra(wgraph& G, vertex sv, ofstream& fout)
{
    vertex vn = G.size();

    //Run dijkstra algorithm and get the predecessors information
    vector<vertex> P = wgraph_dijkstra(G, sv).second;

    //Vertices and edges result info to be written on the file
    vector<vertex> res_v;
    vector<pair<vertex, vertex>> res_e;

    //Get all the info from the predecessors list
    res_v.push_back(sv);
    for (int i = 0; i < vn; i++)
    {
        if (P[i] != vn)
        {
            res_v.push_back(i);
            res_e.push_back({ i, P[i] });
        }
    }

    //Write all info in the correct format
    vn = res_v.size();
    edge en = res_e.size();
    fout << vn << "\t" << en << endl;
    for (int i = 0; i < vn; i++)
        fout << res_v[i] << endl;
    for (int i = 0; i < en; i++)
        fout << res_e[i].first << "\t" << res_e[i].second << endl;

}