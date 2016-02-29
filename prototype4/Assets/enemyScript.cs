using UnityEngine;
using System.Collections;

public class enemyScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnCollisionEnter2D(Collision2D coll)
    {
        //collided with electron
        if (coll.gameObject.layer == LayerMask.NameToLayer("realTon") && coll.gameObject.GetComponent<SpriteRenderer>().name.Contains("electron"))
        {
            Destroy(this.gameObject);
        }

        if (coll.gameObject.layer == LayerMask.NameToLayer("Player"))
        {
            Destroy(coll.gameObject);
        }
    }
}
