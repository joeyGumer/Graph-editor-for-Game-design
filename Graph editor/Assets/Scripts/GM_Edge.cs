using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GM_Edge : MonoBehaviour
{
    GameObject e_vertex1;
    GameObject e_vertex2;
    public GameObject graphManager;

    public GameObject e_selection;
    public GameObject e_panel;
    public InputField field_weight;
    public Text text_index;

    public float e_depth = 9.0f;

    public int e_weight = 1;
    public int e_index;

    public Color e_hoverColor;
    bool e_hover = false;
    bool e_selected = false;
    bool e_openPanel = false;
    //----------------METHODS
    //Setters
    public void SetVertices(GameObject v1, GameObject v2)
    {
        if (v1 != null && v2 != null && v1.CompareTag("Vertex") && v2.CompareTag("Vertex"))
        {
            e_vertex1 = v1;
            e_vertex1.GetComponent<GM_Vertex>().AddEdge(v2,gameObject);

            e_vertex2 = v2;
            e_vertex2.GetComponent<GM_Vertex>().AddEdge(v1, gameObject);

            UpdatePosition();

        }
        else
            Debug.Log("ERROR: tried to attach to an edge a null object or not a vertex");
    }

    public void SetWeight(int w)
    {
        e_weight = w;
        graphManager.GetComponent<GM_GraphManager>().UpdateWeights();
}

    public void SetColor(Color c)
    {
        gameObject.GetComponent<SpriteRenderer>().color = c;
    }

    //Getters
    public GameObject GetVertex1()
    {
        return e_vertex1;
    }
    public GameObject GetVertex2()
    {
        return e_vertex2;
    }

    public int GetWeight()
    {
        return e_weight;
    }

    //Utiles
    public void UpdatePosition()
    {
        //Get the positions of both vertices
        Vector2 v1pos = e_vertex1.GetComponent<GM_Vertex>().GetPosition();
        Vector2 v2pos = e_vertex2.GetComponent<GM_Vertex>().GetPosition();

        //Get the vector from one vertex to another
        Vector2 e_vector = v2pos - v1pos;

        //Calculate position, angle and scale of the edge from the vector
        Vector2 e_pos = e_vector / 2.0f + v1pos;
        float e_angle = Mathf.Atan2(e_vector.y, e_vector.x) * Mathf.Rad2Deg;
        float e_scale = e_vector.magnitude;

        //Set tranform with the newe info
        gameObject.transform.localPosition = new Vector3(e_pos.x, e_pos.y, e_depth);
        gameObject.transform.localRotation = Quaternion.AngleAxis(e_angle, Vector3.forward);
        gameObject.transform.localScale = new Vector3(e_scale, 1.0f, 1.0f);
    }

    public void DeleteEdge()
    {
        e_vertex1.GetComponent<GM_Vertex>().RemoveEdge(gameObject);
        //e_vertex2.GetComponent<GM_Vertex>().RemoveEdge(gameObject);//NOT NEEDED AS IS REMOVEEDGE
        Destroy(gameObject);
    }

    public void Hover()
    {
        e_hover = true;
    }

    public void Select(Color c)
    {
        e_selected = true;
        e_selection.GetComponent<SpriteRenderer>().enabled = e_selected;
        e_selection.GetComponent<SpriteRenderer>().color = c;
    }
    public void Unselect()
    {
        e_selected = false;
        e_selection.GetComponent<SpriteRenderer>().color = e_hoverColor;
        e_selection.GetComponent<SpriteRenderer>().enabled = e_selected;
    }

    /*public void SetPanelConfiguration()
    {
        text_index = e_panel.transform.GetChild(0).GetChild(1).GetComponent<Text>();
        field_weight = e_panel.transform.GetChild(1).GetChild(1).GetComponent<InputField>();

        field_weight.onValueChanged.AddListener(SetWeight);
    }
    public void OpenPanel(Vector3 pos)
    {
        e_openPanel = true;
        e_panel.SetActive(true);
        e_panel.transform.position = pos;
        text_index.text = e_index.ToString();
        field_weight.text = e_weight.ToString();
        
    }*/

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (!e_selected)
        {
            e_selection.GetComponent<SpriteRenderer>().enabled = e_hover;
            e_hover = false;
        }
    }
}
