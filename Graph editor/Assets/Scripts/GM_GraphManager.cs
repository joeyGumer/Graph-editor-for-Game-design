using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.IO;
using System.Diagnostics;

public class GM_GraphManager : MonoBehaviour
{
    //-------------ATTRIBUTES
    public GameObject prefab_vertex;
    public GameObject prefab_edge;

    List<GameObject> g_vertices = new List<GameObject>();
    List<GameObject> g_edges = new List<GameObject>();

    GameObject new_vertex;
    GameObject new_edge;

    public Canvas UI_canvas;
    public EventSystem eventSystem;

    float depth_edge = 9.0f;
    float depth_vertex = 1.0f;

    bool show_weight = false;
    int max_weight = 1;
    int min_weight = 1;

    public Color max_weightColor;
    public Color mid_weightColor;
    public Color min_weightColor;

    bool selectedObj = false;
    RaycastHit2D hit;

    //Feedback objects
    LineRenderer lr;

    //Commands
    private enum GM_command { CMD_Idle, CMD_AddVertex, CMD_AddConnectedVertex, CMD_DuplicateVertex, CMD_DuplicateConnectedVertex, CMD_AddEdge, 
     CMD_DeleteVertex, CMD_DeleteEdge, CMD_Move, CMD_Evaluation }

    private uint GM_comandSize = 10;
    private GM_command currentCommand = GM_command.CMD_Idle;

    //Buttons
    public Button BTN_cmdAddVertex;
    public Button BTN_cmdAddConnectedVertex;
    public Button BTN_cmdMove;
    public Button BTN_cmdAddEdge;
    public Button BTN_cmdDuplicateVertex;
    public Button BTN_cmdDeleteVertex;
    public Button BTN_cmdDeleteEdge;
    public Button BTN_cmdReset;
    public Button BTN_cmdDuplicateConnectedVertex;
    public Button BTN_cmdEvaluation;
    public Toggle BTN_cmdShowWeights;

    bool cmdChangeProhibited = false;

    public GameObject PANEL_Edge;

    public GameObject PNL_Evaluation;

    //WARNING: temporal
    public GameObject prev_vertex;

    //-------------METHODS

    //Setters
    void SetCommand(GM_command cmd)
    {
        
        if (!selectedObj)//Don't change command if it still has a selected object, the action is not finished
        {
            if ((uint)cmd < GM_comandSize)
            {
                if (currentCommand == GM_command.CMD_Evaluation)
                {
                    PNL_Evaluation.GetComponent<GM_Evaluation>().CloseEvaluationWindow();
                    PNL_Evaluation.SetActive(false);
                }

                if (cmd != GM_command.CMD_Idle)
                    PANEL_Edge.SetActive(false);


                if (currentCommand == cmd)
                    currentCommand = GM_command.CMD_Idle;
                else
                    currentCommand = cmd;
            }

            else
            {
                //ERROR
                //Debug.Log("ERROR: No such command avaliable");
            }
        }
    }
    //Getters

    //Utiles
    void AddEdge(GameObject v1, GameObject v2)
    {
        //Check if the gameobjects are vertices
        if (!HasEdge(v1, v2))
        {
            if (v1 != null && v2 != null && v1.CompareTag("Vertex") && v2.CompareTag("Vertex"))
            {
                new_edge = Instantiate(prefab_edge, Vector3.zero, Quaternion.identity);
                new_edge.GetComponent<GM_Edge>().SetVertices(v1, v2);
                new_edge.GetComponent<GM_Edge>().graphManager = gameObject;
                g_edges.Add(new_edge);
            }
        }
    }
    public void DeleteEdge(GameObject e)
    {
        g_edges.Remove(e);
        e.GetComponent<GM_Edge>().DeleteEdge();
    }

    bool HasEdge(GameObject v1, GameObject v2)
    {
        foreach (GameObject e in g_edges)
        {
            GameObject fv = e.GetComponent<GM_Edge>().GetVertex1();
            GameObject sv = e.GetComponent<GM_Edge>().GetVertex2();
            if ((fv == v1 && sv == v2) || (fv == v2 && sv == v1))
            {
                return true;
            }
        }
        return false;
    }

    //WARNING: look how to define this externally
    //Adds vertex in the position specified
    void AddVertex(Vector2 pos)
    {
        //Vertex instantiate
        new_vertex = Instantiate(prefab_vertex, new Vector3(pos.x, pos.y, depth_vertex), Quaternion.identity);
        new_vertex.GetComponent<GM_Vertex>().graphManager = gameObject;

        //Add Vertex to the list
        g_vertices.Add(new_vertex);
    }

    void DeleteVertex(GameObject v)
    {
        g_vertices.Remove(v);
        v.GetComponent<GM_Vertex>().DeleteVertex();
    }

    void ResetGraph()
    {
        //WARNING: Can't use foreach as we are removing members of the list as we iterate
        //WARNING SHOULD NOT DEACTIVATE THE PANEL HERE;
        PANEL_Edge.SetActive(false);
        int size = g_vertices.Count;
        for (int i = 0; i < size; i++)
        {
            GameObject v = g_vertices[0];
            DeleteVertex(v);
        }
    }
    
    void SetGraphIndexes()
    {
        int size = g_vertices.Count;
        for (int i = 0; i < size; i++)
        {
            g_vertices[i].GetComponent<GM_Vertex>().v_index = i;
        }
        size = g_edges.Count;
        for(int i = 0; i < size; i++)
        {
            g_edges[i].GetComponent<GM_Edge>().e_index = i;
        }
    }

    void OpenEvaluationWindow()
    {
        SetCommand(GM_command.CMD_Evaluation);
        PNL_Evaluation.SetActive(true);
        PNL_Evaluation.GetComponent<GM_Evaluation>().SetOpenConfiguarion();

        PNL_Evaluation.GetComponent<GM_Evaluation>().g_vertices = g_vertices;
        PNL_Evaluation.GetComponent<GM_Evaluation>().g_edges = g_edges;

    }

    public void ShowWeights(bool active)
    {
        if (active)
        {
            show_weight = true;
            UpdateWeights();
        }
        else
        {
            show_weight = false;
            foreach (GameObject e in g_edges)
                e.GetComponent<GM_Edge>().SetColor(Color.white);
        }
        
    }

    public void UpdateWeights()
    {
        if (show_weight)
        {
            if (g_edges.Count != 0)
                max_weight = min_weight = g_edges[0].GetComponent<GM_Edge>().GetWeight();

            foreach (GameObject e in g_edges)
            {
                int w = e.GetComponent<GM_Edge>().GetWeight();
                if (max_weight < w)
                    max_weight = w;
                else if (min_weight > w)
                    min_weight = w;
            }


            if (max_weight != min_weight)
            {
                float mid_weight = (float)(max_weight + min_weight) / 2.0f;
                foreach (GameObject e in g_edges)
                {
                    float w = e.GetComponent<GM_Edge>().GetWeight();

                    if (w <= mid_weight)
                    {
                        float v = (float)(w - min_weight) / (float)(mid_weight - min_weight);
                        e.GetComponent<GM_Edge>().SetColor(Color.Lerp(min_weightColor, mid_weightColor, v));
                    }
                    else
                    {
                        float v = (float)(w - mid_weight) / (float)(max_weight - mid_weight);
                        e.GetComponent<GM_Edge>().SetColor(Color.Lerp(mid_weightColor, max_weightColor, v));
                    }
                }
            }
            else
            {
                foreach (GameObject e in g_edges)
                    e.GetComponent<GM_Edge>().SetColor(min_weightColor);
            }
        }
    }
    void OnGraphChange()
    {
        SetGraphIndexes();
        UpdateWeights();
    }
    // Start is called before the first frame update
    void Start()
    {
        lr = gameObject.GetComponent<LineRenderer>();
        lr.positionCount = 2;

        //Assign button functions
        Button btn = BTN_cmdAddVertex.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_AddVertex); });

        btn = BTN_cmdAddConnectedVertex.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_AddConnectedVertex); });

        btn = BTN_cmdMove.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_Move); });

        btn = BTN_cmdDuplicateVertex.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_DuplicateVertex); });

        btn = BTN_cmdDuplicateConnectedVertex.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_DuplicateConnectedVertex); });

        btn = BTN_cmdAddEdge.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_AddEdge); });

        btn = BTN_cmdDeleteVertex.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_DeleteVertex); });

        btn = BTN_cmdDeleteEdge.GetComponent<Button>();
        btn.onClick.AddListener(delegate { SetCommand(GM_command.CMD_DeleteEdge); });

        BTN_cmdEvaluation.onClick.AddListener(OpenEvaluationWindow);

        btn = BTN_cmdReset.GetComponent<Button>();
        btn.onClick.AddListener(ResetGraph);

        
        //WARNING, change thisOn);
        BTN_cmdShowWeights.onValueChanged.AddListener(ShowWeights);
    }

    // Update is called once per frame
    void Update()
    {
        int v_size = g_vertices.Count;
        int e_size = g_edges.Count;
        
        Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        Vector2 mousePos2D = new Vector2(mousePos.x, mousePos.y);
        hit = Physics2D.Raycast(mousePos2D, Vector2.zero);
        
        //Check if the cursor is over the UI using the graphic raycaster of the canvas
        PointerEventData pt = new PointerEventData(eventSystem);
        pt.position = Input.mousePosition;
        List<RaycastResult> results = new List<RaycastResult>();
        UI_canvas.GetComponent<GraphicRaycaster>().Raycast(pt, results);

   
        //Don't use command actions if pointer is over UI, unless there's a selected object 
        if (selectedObj || results.Count == 0)
        {
            //Select vertex or edge
            if (hit.collider != null)
            {
                if (hit.collider.gameObject.CompareTag("Vertex"))
                    hit.collider.gameObject.GetComponent<GM_Vertex>().Hover();
                else if (hit.collider.gameObject.CompareTag("Edge"))
                    hit.collider.gameObject.GetComponent<GM_Edge>().Hover();

            }

            //Management of actions when user has no command selected
            if (!selectedObj && Input.GetMouseButtonDown(1))
            {
                if (currentCommand != GM_command.CMD_Idle)
                {
                    SetCommand(GM_command.CMD_Idle);
                }

                if (hit.collider != null && hit.collider.gameObject.CompareTag("Edge"))
                {

                    PANEL_Edge.SetActive(true);
                    PANEL_Edge.GetComponent<GM_Panel>().OpenPanel(hit.collider.gameObject, Input.mousePosition);

                }
                else
                    PANEL_Edge.SetActive(false); //WARNING: Should do a bool to not do this everyframe

            }
            //Management of commands selected
            else
            {
                switch (currentCommand)
                {
                    //Add Vertex Mode
                    case GM_command.CMD_AddVertex:
                        {
                            BTN_cmdAddVertex.Select();
                            if (Input.GetMouseButtonUp(0))
                            {
                                AddVertex(mousePos);
                            }
                        }
                        break;
                    case GM_command.CMD_AddConnectedVertex:
                        {
                            BTN_cmdAddConnectedVertex.Select();
                            if (Input.GetMouseButtonDown(0))
                            {
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                {
                                    selectedObj = true;
                                    //Create the new vertex
                                    GameObject hitVertex = hit.collider.gameObject;
                                    Vector2 hitVertexPos = hitVertex.GetComponent<GM_Vertex>().GetPosition();

                                    //Create the new vertex where the hit vertex is
                                    AddVertex(hitVertexPos);

                                    //Create the edge that joins the vertices
                                    AddEdge(new_vertex, hitVertex);
                                }
                            }
                            else if (selectedObj == true)
                            {
                                if (Input.GetMouseButtonUp(0))
                                    selectedObj = false;
                                else
                                {
                                    //Update the position of the mouse while the mouse button is still pressed
                                    new_vertex.GetComponent<GM_Vertex>().SetPosition(mousePos2D);
                                }

                            }
                        }
                        break;
                    case GM_command.CMD_Move:
                        {
                            BTN_cmdMove.Select();
                            if (selectedObj == true)
                            {
                                new_vertex.GetComponent<GM_Vertex>().SetPosition(mousePos2D);

                                if (Input.GetMouseButtonUp(0))
                                    selectedObj = false;
                            }
                            else if (Input.GetMouseButtonDown(0))
                            {
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                {
                                    new_vertex = hit.collider.gameObject;
                                    selectedObj = true;
                                }
                            }
                        }
                        break;
                    case GM_command.CMD_DuplicateVertex:
                        {
                            BTN_cmdDuplicateVertex.Select();

                            if (Input.GetMouseButtonDown(0))
                            {
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                {
                                    selectedObj = true;

                                    //Create the new vertex
                                    GameObject hitVertex = hit.collider.gameObject;
                                    Vector2 hitVertexPos = hitVertex.GetComponent<GM_Vertex>().GetPosition();
                                    AddVertex(hitVertexPos);

                                    //Duplicate all edges attached to the new vertex
                                    List<GameObject> nVertices = hitVertex.GetComponent<GM_Vertex>().GetAllNeighbours();
                                    foreach (GameObject v in nVertices)
                                    {
                                        AddEdge(new_vertex, v);
                                    }
                                }
                            }
                            else if (selectedObj == true)
                            {
                                if (Input.GetMouseButtonUp(0))
                                    selectedObj = false;
                                else
                                {
                                    new_vertex.GetComponent<GM_Vertex>().SetPosition(mousePos2D);
                                }

                            }
                        }
                        break;
                    case GM_command.CMD_DuplicateConnectedVertex:
                        {
                            BTN_cmdDuplicateConnectedVertex.Select();

                            if (Input.GetMouseButtonDown(0))
                            {
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                {
                                    selectedObj = true;

                                    //Create the new vertex
                                    GameObject hitVertex = hit.collider.gameObject;
                                    Vector2 hitVertexPos = hitVertex.GetComponent<GM_Vertex>().GetPosition();
                                    AddVertex(hitVertexPos);

                                    //Duplicate all edges attached to the new vertex
                                    List<GameObject> nVertices = hitVertex.GetComponent<GM_Vertex>().GetAllNeighbours();
                                    foreach (GameObject v in nVertices)
                                    {
                                        AddEdge(new_vertex, v);
                                    }
                                    //Also connect to the previous vertex
                                    AddEdge(new_vertex, hitVertex);
                                }
                            }
                            else if (selectedObj == true)
                            {
                                if (Input.GetMouseButtonUp(0))
                                    selectedObj = false;
                                else
                                {
                                    new_vertex.GetComponent<GM_Vertex>().SetPosition(mousePos2D);
                                }

                            }
                        }
                        break;

                    case GM_command.CMD_AddEdge:
                        {
                            BTN_cmdAddEdge.Select();
                            if (Input.GetMouseButtonDown(0))
                            {
                                //If clicking on a vertex, begin mode to check for another vertex
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                {
                                    new_vertex = hit.collider.gameObject;
                                    selectedObj = true;
                                }
                            }
                            else if (selectedObj == true)
                            {
                                //Draw line while keeping the button pressed for feedbak
                                Vector2 pos0 = new_vertex.GetComponent<GM_Vertex>().GetPosition();
                                lr.SetPosition(0, new Vector3(pos0.x, pos0.y, 0.0f));
                                lr.SetPosition(1, new Vector3(mousePos2D.x, mousePos2D.y, 0.0f));

                                if (Input.GetMouseButtonUp(0))
                                {
                                    hit = Physics2D.Raycast(mousePos2D, Vector2.zero);

                                    //If releasing button while on a vertex create an edge between both vertices
                                    if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                    {
                                        GameObject hitVertex = hit.collider.gameObject;
                                        AddEdge(new_vertex, hitVertex);
                                    }

                                    //Erase the feedback line and restart wherever the button is relesead
                                    selectedObj = false;
                                    lr.SetPosition(0, Vector3.zero);
                                    lr.SetPosition(1, Vector3.zero);
                                }
                            }
                        }
                        break;
                    case GM_command.CMD_DeleteVertex:
                        {
                            BTN_cmdDeleteVertex.Select();
                            if (Input.GetMouseButtonDown(0))
                            {
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Vertex"))
                                {
                                    DeleteVertex(hit.collider.gameObject);
                                }
                            }
                        }
                        break;
                    case GM_command.CMD_DeleteEdge:
                        {
                            BTN_cmdDeleteEdge.Select();
                            if (Input.GetMouseButtonDown(0))
                            {
                                if (hit.collider != null && hit.collider.gameObject.CompareTag("Edge"))
                                {
                                    new_edge = hit.collider.gameObject;

                                    DeleteEdge(new_edge);
                                }
                            }
                        }
                        break;
                    case GM_command.CMD_Evaluation:
                        {
                            BTN_cmdEvaluation.Select();
                        }
                        break;
                    default:
                        break;
                }
            }

            if (v_size != g_vertices.Count || e_size != g_edges.Count)
            {
                OnGraphChange();
            }
        }
    }
}

