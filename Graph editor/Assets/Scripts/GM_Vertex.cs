using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GM_Vertex : MonoBehaviour
{
    //----------------ATTRIBUTES

    public List<Color> v_colors;
    public float v_depth = 0.0f;

    List<GameObject> v_edges = new List<GameObject>();
    List<GameObject> v_neighbours = new List<GameObject>();
    public GameObject graphManager;
    public GameObject selection;
    public int v_index;
    public Color v_hoverColor;

    bool v_selected = false;
    bool v_hover = false;
    //WARNING: v_pos not used right now
    Vector2 v_pos;


    //----------------METHODS
    //Setters
    public void SetPosition(Vector2 pos)
    {
        v_pos = pos;
        gameObject.transform.localPosition = new Vector3(v_pos.x, v_pos.y, v_depth);

        //We also update the edges' incident to the vertex positions
        foreach (GameObject edge in v_edges)
        {
            edge.GetComponent<GM_Edge>().UpdatePosition();
        }
    }

    //WARNING: i don't totally like how this is implemented
    public void AddEdge(GameObject vertex, GameObject edge)
    {
        if (edge != null && vertex != null && vertex.CompareTag("Vertex") && edge.CompareTag("Edge"))
        {
            v_edges.Add(edge);
            v_neighbours.Add(vertex);
        }
        else
            Debug.Log("ERROR: Attempted to attach gameObject, not edge, to a vertex");
    }
    //Safe function, call it whenever you want to safely delete an edge attached to a vertex, so that it is counted
    //as deleted for all neighbours
    public void RemoveEdge(GameObject edge)
    {
        GameObject nVer = GetAdjacentVertex(edge);

        nVer.GetComponent<GM_Vertex>().RemoveEdgeUnilateral(gameObject, edge);
        v_neighbours.Remove(nVer);
        v_edges.Remove(edge);
    }

    //Not a safe function, deletes the edge data only for this vertex, not for the neighbour, for that use previos function
    public void RemoveEdgeUnilateral(GameObject vertex, GameObject edge)
    {
        v_neighbours.Remove(vertex);
        v_edges.Remove(edge);
    }

   public void DeleteIncidentEdges()
    {
        //WARNING: check if there's a better way to do this
        //There seems to be an error here
        int size = v_edges.Count;
        for (int i = 0; i< size; i++)
        {
            GameObject e = v_edges[0];
            graphManager.GetComponent<GM_GraphManager>().DeleteEdge(e);
            //e.GetComponent<GM_Edge>().DeleteEdge();
        }
    }
    public void DeleteVertex()
    {
        DeleteIncidentEdges();
        Destroy(gameObject);
    }

    //Getters
    public Vector2 GetPosition()
    {
        Vector3 pos = transform.localPosition;
        return new Vector2(pos.x, pos.y);
    }

    public GameObject GetAdjacentVertex(GameObject edge)
    {
        if (edge != null && edge.CompareTag("Edge"))
        {
            GameObject res = edge.GetComponent<GM_Edge>().GetVertex2();
            if (res == gameObject)
                res = edge.GetComponent<GM_Edge>().GetVertex1();
            return res;
        }
        else
        {
            Debug.Log("ERROR: GetAdjacemtVertex recieves only gameobjects of the type 'Edge'");
            return null;
        }
    }

    public List<GameObject> GetAllNeighbours()
    {
        return v_neighbours;
    }
    //Utiles

    public void Select(Color c)
    {
        v_selected = true;
        selection.GetComponent<SpriteRenderer>().enabled = v_selected;
        selection.GetComponent<SpriteRenderer>().color = c;

    }
    public void Unselect()
    {
        v_selected = false;
        selection.GetComponent<SpriteRenderer>().color = v_hoverColor;
        selection.GetComponent<SpriteRenderer>().enabled = v_selected;
    }
    public void Hover()
    {
        v_hover = true;
    }
    // Start is called before the first frame update
    void Start()
    {
        //Set vertex position in 2D coordinates for easy acces
        v_pos = new Vector2(transform.localPosition.x, transform.localPosition.y);

        //Set the color of the vertex
        SpriteRenderer sp =  gameObject.GetComponent(typeof(SpriteRenderer)) as SpriteRenderer;
        sp.color = v_colors[(int)Random.Range(0, v_colors.Count)];
        
    }

    // Update is called once per frame
    void Update()
    {
        if (!v_selected)
        {
            selection.GetComponent<SpriteRenderer>().enabled = v_hover;
            v_hover = false;
        }
    }
}
