using UnityEngine;
using System.Collections;

public class player : MonoBehaviour {
    public float radius;
    public GameObject electron;
    public GameObject proton;
	// Use this for initialization
	void Start () {
	    for (int i = 0; i < 6; i++)
        {
            GameObject clone;
            if (i < 3)
            {
               clone = Instantiate(electron, new Vector2(transform.position.x + i * 0.0001f, transform.position.y), Quaternion.identity) as GameObject;
            }
            else
            {
                clone = Instantiate(proton, new Vector2(transform.position.x + i * 0.0001f, transform.position.y), Quaternion.identity) as GameObject;
            }
            clone.transform.parent = transform;
            float x = Random.Range(-1f, 1f);
            float y = Random.Range(-1f, 1f);
            Vector2 direction = new Vector2(x,y);
            clone.GetComponent<Rigidbody2D>().AddForce(direction.normalized * 10.5f);
            
        }

        
    }


    void shoot(bool proton)
    {
        string compare = "electron";
        int charge = -1;
        if (proton)
        {
            compare = "proton";
            charge = 1;
        }
        Vector3 worldMousePosition = Camera.main.ScreenToWorldPoint(new Vector2(Input.mousePosition.x, Input.mousePosition.y));
        Vector2 direction = worldMousePosition - transform.position;
        Transform selected = null;
        foreach (Transform child in transform)
        {
            if (child.GetComponent<SpriteRenderer>().name.Contains(compare))
            {
                selected = child;
                break;
            }
        }

        if (selected != null)
        {
            selected.gameObject.AddComponent<chargeScript>();
            selected.gameObject.GetComponent<chargeScript>().charge = charge;
            selected.gameObject.layer = LayerMask.NameToLayer("realTon");
            selected.parent = null;
            selected.transform.position = new Vector2(transform.position.x + direction.normalized.x * 0.7f, transform.position.y + direction.normalized.y * 0.7f);
            selected.GetComponent<Rigidbody2D>().drag = 1.0f;
            selected.GetComponent<Rigidbody2D>().AddForce(direction.normalized * 5.0f, ForceMode2D.Impulse);
            GetComponent<chargeScript>().charge -= charge;
        }
    }

	// Update is called once per frame
	void Update () {

        if (Input.GetMouseButtonUp(0))
        {
            shoot(true);
        }

        if (Input.GetMouseButtonUp(1))
        {
            shoot(false);
        }



    }

    void FixedUpdate()
    {
        foreach (Transform child in transform)
        {

            Vector2 direction = child.transform.localPosition;
            float dist = Vector2.Distance(child.transform.localPosition, new Vector2(0.0f,0.0f));
            if (dist >= 0.35f)
            {
                child.transform.localPosition = direction.normalized * 0.35f;
            }
        }
    }

    void OnCollisionEnter2D(Collision2D coll)
    {

        if (coll.gameObject.layer == LayerMask.NameToLayer("realTon") && transform.childCount < 6)
        {
            GetComponent<chargeScript>().charge += coll.gameObject.GetComponent<chargeScript>().charge;
            coll.gameObject.transform.parent = transform;
            Destroy(coll.gameObject.GetComponent<chargeScript>());
            coll.gameObject.layer = LayerMask.NameToLayer("Ton");
            coll.gameObject.GetComponent<Rigidbody2D>().drag = 0.0f;

            
        }
    }
}
