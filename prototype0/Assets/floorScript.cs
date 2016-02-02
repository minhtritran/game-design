using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class floorScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	    
	}

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.collider.gameObject.layer == 10)
        {
            Debug.Log("hello");

            SceneManager.LoadScene("scene1");
        }
    }
}
