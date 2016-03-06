using UnityEngine;
using System.Collections;

public class chargeScript : MonoBehaviour {
    public int charge;
	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void FixedUpdate () {
        chargeScript cs = GetComponent<chargeScript>();
        chargeScript[] objs = FindObjectsOfType(typeof(chargeScript)) as chargeScript[];
        
        foreach (chargeScript obj_cs in objs)
        {
            GameObject obj = obj_cs.gameObject;
            
            if (obj != this.gameObject)
            {
                //attraction
                if ((cs.charge > 0 && obj_cs.charge < 0) || (cs.charge < 0 && obj_cs.charge > 0))
                {
                    int difference = Mathf.Abs(obj_cs.charge - cs.charge);
                    float distance = Mathf.Abs(Vector2.Distance(obj.transform.position, transform.position));
                    Vector2 direction = obj.transform.position - transform.position;
                    if (difference >= distance)
                    {
                        if (GetComponent<Rigidbody2D>())
                            GetComponent<Rigidbody2D>().AddForce(direction.normalized * difference * (obj.transform.localScale.x/2.0f) / distance);
                    }
                }
                //repulsion
                else if ((cs.charge > 0 && obj_cs.charge > 0) || (cs.charge < 0 && obj_cs.charge < 0))
                {
                    int difference = Mathf.Abs(cs.charge + obj_cs.charge);
                    float distance = Vector2.Distance(obj.transform.position, transform.position);
                    Vector2 direction = transform.position - obj.transform.position;
                    if (difference >= distance)
                    {
                        if (GetComponent<Rigidbody2D>())
                            GetComponent<Rigidbody2D>().AddForce(direction.normalized * difference * (obj.transform.localScale.x / 2.0f) / distance);
                    }
                }
            }
        }
    }
}
