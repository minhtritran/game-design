using UnityEngine;
using System.Collections;

public class goalScript : MonoBehaviour {

    public bool touchedGoal;
	// Use this for initialization
	void Start () {
        touchedGoal = false;
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
