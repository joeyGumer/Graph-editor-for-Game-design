using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class GM_Camera : MonoBehaviour
{
    public float cam_maxSize = 20.0f;
    public float cam_minSize = 2.5f;
    public float cam_defaultSize = 5f;
    public float cam_zoomSpeed = 0.1f;
    public float cam_panSpeed = 1.0f;
    public float cam_panHalfWidth = 100.0f;
    public float cam_panHalfHeigth = 100.0f;

    Vector3 mouse_dragOrigin = Vector3.zero;

    Camera cam;

    public Canvas UI_canvas;
    public EventSystem eventSystem;

    // Start is called before the first frame update
    void Start()
    {
        cam = gameObject.GetComponent<Camera>();
        cam.orthographicSize = cam_defaultSize;
    }

    // Update is called once per frame
    void Update()
    {
        //Check if the cursor is over the UI using the graphic raycaster of the canvas
        PointerEventData pt = new PointerEventData(eventSystem);
        pt.position = Input.mousePosition;
        List<RaycastResult> results = new List<RaycastResult>();
        UI_canvas.GetComponent<GraphicRaycaster>().Raycast(pt, results);

        if (results.Count == 0)
        {
            if (Input.GetMouseButtonDown(2))
            {
                mouse_dragOrigin = cam.ScreenToWorldPoint(Input.mousePosition);
            }
            else if (Input.GetMouseButton(2))
            {
                Vector3 cam_pos = transform.position;
                Vector3 mouse_position = cam.ScreenToWorldPoint(Input.mousePosition);
                Vector3 mouse_dragDelta = mouse_position - mouse_dragOrigin;
                mouse_dragOrigin = mouse_position;

                transform.position += cam_panSpeed * mouse_dragDelta * Time.deltaTime;

                //Limits camera panning
                if (transform.position.x < -cam_panHalfWidth || transform.position.x > cam_panHalfWidth ||
                    transform.position.y < -cam_panHalfHeigth || transform.position.y > cam_panHalfHeigth)
                {
                    transform.position = cam_pos;
                }

            }
            //Zoom function
            else if (Input.mouseScrollDelta.y != 0)
            {
                float sp = 5 * cam_zoomSpeed;
                cam.orthographicSize += sp * Input.mouseScrollDelta.y * Time.deltaTime;

                if (cam.orthographicSize < cam_minSize)
                    cam.orthographicSize = cam_minSize;
                else if (cam.orthographicSize > cam_maxSize)
                    cam.orthographicSize = cam_maxSize;
            }

            //Reset camera position
            if (Input.GetKeyDown(KeyCode.F))
            {
                transform.position = new Vector3(0.0f, 0.0f, -10.0f);
            }

        }
    }
}
