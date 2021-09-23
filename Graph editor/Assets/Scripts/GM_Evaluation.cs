using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using UnityEngine;
using UnityEngine.UI;

public class GM_Evaluation : MonoBehaviour
{
    //---------------------- ATTRIBUTES

    private enum showMode {SHW_PATH, SHW_DIJKSTRA, SHW_TSP, SHW_VCOLOR, SHW_ECOLOR, SHW_SELECTVERTICES , SHW_SELECTSOURCE, SHW_IDLE }
    private enum evaluationMode { EVL_PATH, EVL_DIJKSTRA, EVL_TPS , EVL_VCOLOR, EVL_ECOLOR }
    //---Minimal path 
    public Button EUL_BtnEvaluation;
    public Button EUL_BtnShow;
    public Button EUL_BtnVertices;
    public Text EUL_TxtVertex1;
    public Text EUL_TxtVertex2;
    public Text EUL_TxtDistance;
    public Text EUL_TxtExists;

    public Color startColor;
    public Color endColor;

    List<GameObject> path_vertices = new List<GameObject>();
    List<GameObject> path_edges = new List<GameObject>();

    //---Dijkstra tree
    public Button DJK_BtnEvaluation;
    public Button DJK_BtnShow;
    public Button DJK_BtnVertices;
    public Text DJK_TxtSourceVertex;

    List<GameObject> djk_vertices = new List<GameObject>();
    List<GameObject> djk_edges = new List<GameObject>();

    GameObject SourceVertex;

    //---TPS
    public Button TSP_BtnEvaluation;
    public Button TSP_BtnShow;
    public Text TSP_TxtExists;
    public Text TSP_TxtWeight;

    List<GameObject> tsp_vertices = new List<GameObject>();
    List<GameObject> tsp_edges = new List<GameObject>();


    //--- Min Vertex Color
    public Button MVC_BtnEvaluation;
    public Button MVC_BtnShow;
    public Text MVC_TxtColorNumber;

    List<Color> mvc_vertexColors = new List<Color>();
    //--- Min Edge Color
    public Button MEC_BtnEvaluation;
    public Button MEC_BtnShow;
    public Text MEC_TxtColorNumber;

    List<Color> mec_edgeColors = new List<Color>();

    public List<Color> colorOptions;

    //
    public Color resultColor;

    GameObject StartVertex = null;
    GameObject EndVertex = null;

    public List<GameObject> g_vertices;
    public List<GameObject> g_edges;

    List<GameObject> res_vertices =  new List<GameObject>();
    List<GameObject> res_edges  = new List<GameObject>();

    showMode currentShowMode = showMode.SHW_IDLE;

    //---Hamiltonian Cycle




    //---------------------- METHODS
    public void SetOpenConfiguarion()
    {
        

        //Eulerian
        EUL_BtnEvaluation.interactable = false;
        EUL_BtnShow.interactable = false;
        EUL_BtnVertices.interactable = true;
        EUL_TxtVertex1.text = "";
        EUL_TxtVertex2.text = "";
        EUL_TxtDistance.text = "";
        EUL_TxtExists.text = "";

        //Dijkstra
        DJK_BtnEvaluation.interactable = false;
        DJK_BtnShow.interactable = false;
        DJK_BtnVertices.interactable = true;
        DJK_TxtSourceVertex.text = "";

        //Hamiltonian
        TSP_BtnEvaluation.interactable = true;
        TSP_BtnShow.interactable = false;
        TSP_TxtExists.text = "";
        TSP_TxtWeight.text = "";

        //Vertex Coloring
        MVC_BtnEvaluation.interactable = true;
        MVC_BtnShow.interactable = false;
        MVC_TxtColorNumber.text = "";
        //Edge Coloring
        MEC_BtnEvaluation.interactable = true;
        MEC_BtnShow.interactable = false;
        MEC_TxtColorNumber.text = "";


        SetShowMode(showMode.SHW_IDLE);
    }

    public void CloseEvaluationWindow()
    {
        SetEndVertex(null);
        SetStartVertex(null);
        SetSourceVertex(null);
        HideResult();
    }

    void EUL_SelectVertices()
    {
        EUL_BtnEvaluation.interactable = false;
        EUL_BtnShow.interactable = false;


    }

    //Setters
    void SetShowMode(showMode mode)
    {
        //WARNING it has to be less than last
        if ((uint)mode < (uint)showMode.SHW_IDLE + 1)
        {
            currentShowMode = mode;
            switch (currentShowMode)
            {
                case showMode.SHW_SELECTVERTICES:
                    {                        
                        HideResult();
                        SetStartVertex(null);
                        SetEndVertex(null);
                        EUL_BtnEvaluation.interactable = false;
                        EUL_BtnShow.interactable = false;
                        EUL_TxtExists.text = "";
                        EUL_TxtDistance.text = "";
                    }
                    break;
                case showMode.SHW_SELECTSOURCE:
                    {
                        HideResult();
                        SetSourceVertex(null);
                        DJK_BtnEvaluation.interactable = false;
                        DJK_BtnShow.interactable = false;
                    }
                    break;
            }
        }
        else
        {
            //ERROR
            //Debug.Log("ERROR: No such command avaliable");
        }
    }

    void SetStartVertex(GameObject v)
    {
        if(v != null)
        {
            StartVertex = v;
            StartVertex.GetComponent<GM_Vertex>().Select(startColor);
            EUL_TxtVertex1.text = StartVertex.GetComponent<GM_Vertex>().v_index.ToString();
        }
        else if(StartVertex != null)
        {
            StartVertex.GetComponent<GM_Vertex>().Unselect();
            StartVertex = null;
            EUL_TxtVertex1.text = "";
        }
    }

    void SetEndVertex(GameObject v)
    {
        if (v != null)
        {
            EndVertex = v;
            EndVertex.GetComponent<GM_Vertex>().Select(endColor);
            EUL_TxtVertex2.text = EndVertex.GetComponent<GM_Vertex>().v_index.ToString();

            EUL_BtnEvaluation.interactable = true;
            SetShowMode(showMode.SHW_IDLE);
        }
        else if(EndVertex != null)
        {            
            EndVertex.GetComponent<GM_Vertex>().Unselect();
            EndVertex = null;
            EUL_TxtVertex2.text = "";
        }

    }

    void SetSourceVertex(GameObject v)
    {
        if (v != null)
        {
            SourceVertex = v;
            SourceVertex.GetComponent<GM_Vertex>().Select(startColor);
            DJK_TxtSourceVertex.text = SourceVertex.GetComponent<GM_Vertex>().v_index.ToString();

            DJK_BtnEvaluation.interactable = true;
            SetShowMode(showMode.SHW_IDLE);
        }
        else if (SourceVertex != null)
        {
            SourceVertex.GetComponent<GM_Vertex>().Unselect();
            SourceVertex = null;
            DJK_TxtSourceVertex.text = "";
        }
    }
    //Utiles
    GameObject GetEdge(GameObject v1, GameObject v2)
    {
        foreach(GameObject e in g_edges)
        {
            GameObject fv = e.GetComponent<GM_Edge>().GetVertex1();
            GameObject sv = e.GetComponent<GM_Edge>().GetVertex2();
            if ((fv==v1 && sv == v2)||(fv == v2 && sv == v1))
            {
                return e;
            }            
        }
        return null;
    }

    //Evaluation methods

    void HideResult()
    {
        foreach (GameObject v in g_vertices)
        {
            v.GetComponent<GM_Vertex>().Unselect();
        }
        foreach (GameObject e in g_edges)
        {
            e.GetComponent<GM_Edge>().Unselect();
        }
        res_vertices = null;
        res_edges = null;
        SetStartVertex(null);
        SetEndVertex(null);
    }
    void ShowResult(showMode mode)
    {
        SetShowMode(showMode.SHW_IDLE);

        //Unselect previous result if changing mode
        if (mode != currentShowMode)
        {
            HideResult();
        }

        switch (mode)
        {
            case showMode.SHW_PATH:
                {
                    res_vertices = path_vertices;
                    res_edges = path_edges;
                }
                break;
            case showMode.SHW_DIJKSTRA:
                {
                    res_vertices = djk_vertices;
                    res_edges = djk_edges;
                }
                break;
            case showMode.SHW_TSP:
                {
                    res_vertices = tsp_vertices;
                    res_edges = tsp_edges;
                }
                break;
            case showMode.SHW_VCOLOR:
                {
                    for(int i=0; i < g_vertices.Count; i++)
                    {
                        g_vertices[i].GetComponent<GM_Vertex>().Select(mvc_vertexColors[i]);
                    }
                }
                break;
            case showMode.SHW_ECOLOR:
                {
                    for(int i=0; i< g_edges.Count; i++)
                    {
                        g_edges[i].GetComponent<GM_Edge>().Select(mec_edgeColors[i]);
                    }
                }
                break;
        }
        if (res_vertices != null && res_edges != null)
        {
            foreach (GameObject v in res_vertices)
            {
                v.GetComponent<GM_Vertex>().Select(resultColor);
            }
            foreach (GameObject e in res_edges)
            {
                e.GetComponent<GM_Edge>().Select(resultColor);
            }
        }
    }

    void RunGraphEvaluator()
    {
        ProcessStartInfo startInfo = new ProcessStartInfo();
        string name = Application.dataPath + "/../../GraphEvaluator/GPrc4.exe";
        startInfo.FileName = name;
        startInfo.WorkingDirectory = Path.GetDirectoryName(name);
        Process GraphEvaluator = Process.Start(startInfo);
        GraphEvaluator.WaitForExit();
        GraphEvaluator.Close();

    }

    void EvaluateGraph(evaluationMode mode)
    {
        //Path of file
        string path = Application.dataPath + "/../../GraphEvaluator/GraphEvaluation.in";

        //Create file if it doesn't exist
        if (File.Exists(path))
        {
            File.Delete(path);
        }

        string text = "";
        if (mode == evaluationMode.EVL_ECOLOR || mode == evaluationMode.EVL_VCOLOR)
        {
            text += "0\n";
        }
        else
        {
            text += "1\n";
        }


            //Write the edges info of the graph
            text += g_vertices.Count + "\t" + g_edges.Count + "\n";
            foreach (GameObject e in g_edges)
            {
                GameObject v1 = e.GetComponent<GM_Edge>().GetVertex1();
                GameObject v2 = e.GetComponent<GM_Edge>().GetVertex2();

                text += v1.GetComponent<GM_Vertex>().v_index + "\t" + v2.GetComponent<GM_Vertex>().v_index;
            if (mode != evaluationMode.EVL_ECOLOR && mode != evaluationMode.EVL_VCOLOR)
                text += "\t" + e.GetComponent<GM_Edge>().e_weight + "\n";
            else
                text += "\n";
            }

        //Write evaluation mode info
        text += (int)mode + "\n";

        if (mode == evaluationMode.EVL_PATH)
            text += StartVertex.GetComponent<GM_Vertex>().v_index + "\t" + EndVertex.GetComponent<GM_Vertex>().v_index + "\n";
        else if (mode == evaluationMode.EVL_DIJKSTRA)
            text += SourceVertex.GetComponent<GM_Vertex>().v_index + "\n";

        //Send file and run external evaluation program
        File.WriteAllText(path, text);
        RunGraphEvaluator();

        ReadEvaluationFile(mode);
    }

    void ReadEvaluationFile(evaluationMode mode)
    {
        string path = Application.dataPath + "/../../GraphEvaluator/GraphEvaluation.out";

        //WARNING: make it safe if there is no file
        switch (mode)
        {
            case evaluationMode.EVL_PATH:
                {
                    path_vertices.Clear();
                    path_edges.Clear();
                    using (TextReader text = File.OpenText(path))
                    {
                        string line = text.ReadLine();
                        string[] bits = line.Split('\t');
                        int vn = int.Parse(bits[0]);                        
                        if (vn == 0)
                        {
                            EUL_TxtExists.text = "No";
                        }
                        else
                        {
                            int en = int.Parse(bits[1]);
                            for (int i = 0; i < vn; i++)
                            {
                                line = text.ReadLine();
                                path_vertices.Add(g_vertices[int.Parse(line)]);
                            }
                            for (int i = 0; i < en; i++)
                            {
                                line = text.ReadLine();
                                bits = line.Split('\t');
                                int v1 = int.Parse(bits[0]);
                                int v2 = int.Parse(bits[1]);

                                path_edges.Add(GetEdge(g_vertices[v1], g_vertices[v2]));
                            }

                            EUL_TxtDistance.text = text.ReadLine();
                            EUL_TxtExists.text = "Yes";
                            EUL_BtnShow.interactable = true;
                        }
                    }

                }
                break;
            case evaluationMode.EVL_DIJKSTRA:
                {
                    djk_edges.Clear();
                    djk_vertices.Clear();
                    using (TextReader text = File.OpenText(path))
                    {
                        string line = text.ReadLine();
                        string[] bits = line.Split('\t');
                        int vn = int.Parse(bits[0]);
                        int en = int.Parse(bits[1]);

                        for (int i = 0; i < vn; i++)
                        {
                            line = text.ReadLine();
                            djk_vertices.Add(g_vertices[int.Parse(line)]);
                        }
                        for (int i = 0; i < en; i++)
                        {
                            line = text.ReadLine();
                            bits = line.Split('\t');
                            int v1 = int.Parse(bits[0]);
                            int v2 = int.Parse(bits[1]);

                            djk_edges.Add(GetEdge(g_vertices[v1], g_vertices[v2]));
                        }
                        DJK_BtnShow.interactable = true;
                    }
                }
                break;
            case evaluationMode.EVL_TPS:
                {
                    tsp_vertices.Clear();
                    tsp_edges.Clear();
                    using(TextReader text = File.OpenText(path))
                    {
                        string line = text.ReadLine();
                        string[] bits = line.Split('\t');
                        int vn = int.Parse(bits[0]);
                        int en = int.Parse(bits[1]);

                        if(vn == 0)
                        {
                            TSP_TxtExists.text = "No";
                        }
                        else
                        {
                            for (int i = 0; i < vn; i++)
                            {
                                line = text.ReadLine();
                                tsp_vertices.Add(g_vertices[int.Parse(line)]);
                            }
                            for (int i = 0; i < en; i++)
                            {
                                line = text.ReadLine();
                                bits = line.Split('\t');
                                int v1 = int.Parse(bits[0]);
                                int v2 = int.Parse(bits[1]);

                                tsp_edges.Add(GetEdge(g_vertices[v1], g_vertices[v2]));
                            }                            
                            TSP_TxtExists.text = "Yes";
                            TSP_TxtWeight.text = text.ReadLine();
                            TSP_BtnShow.interactable = true;
                        }
                    }
                }
                break;
            case evaluationMode.EVL_VCOLOR:
                {
                    mvc_vertexColors.Clear();
                    using (TextReader text = File.OpenText(path))
                    {
                        string line = text.ReadLine();
                        string[] bits = line.Split('\t');
                        int vn = int.Parse(bits[0]);
                        MVC_TxtColorNumber.text = bits[1];

                        for (int i = 0; i< vn; i++)
                        {
                            line = text.ReadLine();
                            mvc_vertexColors.Add(colorOptions[int.Parse(line)]);
                        }
                    }
                    MVC_BtnShow.interactable = true;
                }
                break;
            case evaluationMode.EVL_ECOLOR:
                {
                    mec_edgeColors.Clear();
                    using (TextReader text = File.OpenText(path))
                    {
                        string line = text.ReadLine();
                        string[] bits = line.Split('\t');
                        int en = int.Parse(bits[0]);
                        MEC_TxtColorNumber.text = bits[1];

                        for (int i = 0; i < en; i++)
                        {
                            line = text.ReadLine();
                            mec_edgeColors.Add(colorOptions[int.Parse(line)]);
                        }
                    }
                    MEC_BtnShow.interactable = true;
                }
                break;
            {

            }
        }

    }

    // Start is called before the first frame update
    void Start()
    {
        EUL_BtnVertices.onClick.AddListener(delegate { SetShowMode(showMode.SHW_SELECTVERTICES); });
        EUL_BtnEvaluation.onClick.AddListener(delegate { EvaluateGraph(evaluationMode.EVL_PATH); });
        EUL_BtnShow.onClick.AddListener(delegate { ShowResult(showMode.SHW_PATH); });

        DJK_BtnVertices.onClick.AddListener(delegate { SetShowMode(showMode.SHW_SELECTSOURCE); });
        DJK_BtnEvaluation.onClick.AddListener(delegate { EvaluateGraph(evaluationMode.EVL_DIJKSTRA); });
        DJK_BtnShow.onClick.AddListener(delegate { ShowResult(showMode.SHW_DIJKSTRA); });

        TSP_BtnEvaluation.onClick.AddListener(delegate { EvaluateGraph(evaluationMode.EVL_TPS); });
        TSP_BtnShow.onClick.AddListener(delegate { ShowResult(showMode.SHW_TSP); });

        MVC_BtnEvaluation.onClick.AddListener(delegate { EvaluateGraph(evaluationMode.EVL_VCOLOR); });
        MVC_BtnShow.onClick.AddListener(delegate { ShowResult(showMode.SHW_VCOLOR); });

        MEC_BtnEvaluation.onClick.AddListener(delegate { EvaluateGraph(evaluationMode.EVL_ECOLOR); });
        MEC_BtnShow.onClick.AddListener(delegate { ShowResult(showMode.SHW_ECOLOR); });
    }

    // Update is called once per frame
    void Update()
    {
        switch(currentShowMode)
        {
            case showMode.SHW_IDLE:
                {

                }
                break;
            case showMode.SHW_SELECTVERTICES:
                {
                    EUL_BtnVertices.Select();

                    Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                    Vector2 mousePos2D = new Vector2(mousePos.x, mousePos.y);
                    RaycastHit2D hit = Physics2D.Raycast(mousePos2D, Vector2.zero);
                    if(hit.collider != null)
                    {
                        if(Input.GetMouseButtonDown(0) && hit.collider.gameObject.CompareTag("Vertex"))
                        {
                            if(!StartVertex)
                            {
                                SetStartVertex(hit.collider.gameObject);
                            }
                            else if(StartVertex == hit.collider.gameObject)
                            {
                                SetStartVertex(null);
                            }
                            else
                            {
                                SetEndVertex(hit.collider.gameObject);
                            }
                        }
                    }


                }
                break;
            case showMode.SHW_SELECTSOURCE:
                {
                    DJK_BtnVertices.Select();

                    Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                    Vector2 mousePos2D = new Vector2(mousePos.x, mousePos.y);
                    //WARNING maybe get graph manager hit?
                    RaycastHit2D hit = Physics2D.Raycast(mousePos2D, Vector2.zero);
                    if(hit.collider != null)
                    {
                        if(Input.GetMouseButtonDown(0) && hit.collider.gameObject.CompareTag("Vertex"))
                        {
                            SetSourceVertex(hit.collider.gameObject);
                        }
                    }
                }
                break;

        }
    }
}
