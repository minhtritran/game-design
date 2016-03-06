using UnityEngine;
using System.Collections;

public class goalScript : MonoBehaviour {

    public bool touchedGoal;

    public float xMin;
    public float xMax;
    public float yMin;
    public float yMax;
    // Use this for initialization
    void Start () {
        touchedGoal = false;
        transform.position = new Vector2(Random.Range(xMin, xMax), Random.Range(yMin, yMax));
    }
	
	// Update is called once per frame
	void Update () {
	
	}

    void FixedUpdate() {
 
    }
    void OnTriggerEnter2D(Collider2D coll)
    {
        if (coll.gameObject.layer == LayerMask.NameToLayer("Player"))
        {
            touchedGoal = true;
        }
    }

    void OnTriggerExit2D(Collider2D coll)
    {
        if (coll.gameObject.layer == LayerMask.NameToLayer("Player"))
        {
            touchedGoal = false;
        }
    }

}
