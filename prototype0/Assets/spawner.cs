using UnityEngine;
using System.Collections;

public class spawner : MonoBehaviour {
    public Transform enemy;
    public float spawnRate;
    private float spawnTimer = 5.0f;
    // Use this for initialization
    void Start () {
	    
	}
	
	// Update is called once per frame
	void Update () {
        if (spawnTimer < 5.0f)
            spawnTimer += Time.deltaTime;

        if (spawnTimer > spawnRate)
        {
            spawnTimer = 0.0f;

            float minX = transform.position.x - transform.localScale.x / 2;
            float maxX = transform.position.x + transform.localScale.x / 2;
            float minY = transform.position.y - 1.5f;
            float maxY = transform.position.y - 1.0f;
            Vector2 location = new Vector2(Random.Range(minX, maxX), Random.Range(minY, maxY));
            Transform enemy_clone = (Transform)Instantiate(enemy, location, transform.rotation);
        }
        
    }
}
