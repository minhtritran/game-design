using UnityEngine;
using System.Collections;

public class projectileScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	    
	}

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.collider.gameObject.layer != 9)
        {
            Destroy(gameObject);   
        }
        
        if (collision.collider.gameObject.layer == 10)
        {
            Destroy(collision.collider.gameObject);
        }
    }
}
