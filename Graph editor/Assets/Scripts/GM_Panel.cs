using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GM_Panel : MonoBehaviour
{
    public InputField field_weight;
    public Text text_index;

    public GameObject selected_edge = null;

    public void OpenPanel(GameObject edge, Vector3 pos)
    {
        selected_edge = edge;
        gameObject.transform.position = pos;
        text_index.text = selected_edge.GetComponent<GM_Edge>().e_index.ToString();
        field_weight.text = selected_edge.GetComponent<GM_Edge>().GetWeight().ToString();
    }

    void SetEdgeWeight(string wstr)
    {
        if (wstr == "")
            return;

        int w = int.Parse(wstr);
        if(selected_edge != null)
        {
            field_weight.text = w.ToString();
            selected_edge.GetComponent<GM_Edge>().SetWeight(w);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        field_weight.contentType = InputField.ContentType.IntegerNumber;
        field_weight.onEndEdit.AddListener(SetEdgeWeight);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
