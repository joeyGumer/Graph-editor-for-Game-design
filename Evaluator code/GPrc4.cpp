#include <ctime>
#include "graph.h"
#include "wgraph.h"
#include "graphe.h"

 int 
	main()
{
    ofstream fout;

    //cout << "Press enter to start evaluation" << endl;
    //cin.get();
    
    int evaluation_mode = 0;
    int graph_type = 0;
    vector<vertex> extra_info;

    ifstream fin;
    fin.open("GraphEvaluation.in");
    if (fin.fail()) {
        cout << "unable to open file for reading" << endl;
        cin.get();
        exit(1);
    }
    fin >> graph_type;
    fin.close();

    if (graph_type == 1)
    {
        wgraph WGraph = wgraph_read("GraphEvaluation.in", &evaluation_mode, &extra_info);
        fout.open("GraphEvaluation.out");
        switch (evaluation_mode)
        {
        case EVL_PATH:
        {
            wgraph_writeMinimalPath(WGraph, extra_info[0], extra_info[1], fout);
        }
        break;
        case EVL_DIJKSTRA:
        {
            wgraph_writeDijkstra(WGraph, extra_info[0], fout);
        }
        break;
        case EVL_TPS:
        {
            TravellingSalesmanProblem(WGraph, fout);
        }
        break;
        }
    }
    else if(graph_type == 0)
    {
        graph G; edges GE;
        evaluation_mode = graphe_read(G, GE, "GraphEvaluation.in");
        fout.open("GraphEvaluation.out");
        switch (evaluation_mode)
        {

        case EVL_VCOLOR:
        {
            vector<color> colors;
            MinimalVertexColoring(G, colors, fout);
        }
        break;
        case EVL_ECOLOR:
        {
            MinimalEdgeColoring(G, GE, fout);
        }
        break;
        default:
            break;
        }
    }
    fout.close();
//
//  Ending
//
    //cout << "Press enter to finish..." << endl;
    //cin.get();
}
