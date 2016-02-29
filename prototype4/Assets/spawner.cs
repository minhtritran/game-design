using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.SceneManagement;

public class spawner : MonoBehaviour {
    public GameObject electron;
    public GameObject proton;
    public GameObject enemy;
    public Text winText;

    public int numEnemies;
    public int numBalls;
	// Use this for initialization
	void Start () {
        for (int i = 0; i < numEnemies; i++)
        {
            float x = Random.Range(-8f, 8f);
            float y = Random.Range(-4f, 4f);
            Vector2 vec = new Vector2(x, y);
            while (Vector2.Distance(vec, GameObject.Find("Circle").transform.position) <= 0.7f)
            {
                x = Random.Range(-8f, 8f);
                y = Random.Range(-4f, 4f);
                vec = new Vector2(x, y);
            }

            GameObject clone;
            clone = Instantiate(enemy, vec, Quaternion.identity) as GameObject;
            clone.transform.eulerAngles = new Vector3(0, 0, Random.Range(0, 360));
        }

        for (int i = 0; i < numBalls; i++)
        {
            float x = Random.Range(-8f, 8f);
            float y = Random.Range(-4f, 4f);
            if (Random.Range(-1f, 1f) > 0)
            {
                Instantiate(electron, new Vector2(x, y), Quaternion.identity);
            }
            else
            {
                Instantiate(proton, new Vector2(x, y), Quaternion.identity);
            }
        }


        winText.text = "";
    }
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKeyDown(KeyCode.R))
        {
            Scene scene = SceneManager.GetActiveScene();
            SceneManager.LoadScene(scene.name);

        }

        goalScript goal = FindObjectOfType(typeof(goalScript)) as goalScript;
        enemyScript[] enemies = FindObjectsOfType(typeof(enemyScript)) as enemyScript[];
        if (goal.touchedGoal && enemies.Length <= 1) {
            winText.text = "You won! Press R to reset";
        }

        if (GameObject.Find("Circle") == null)
        {
            winText.text = "You lost! Press R to reset";
        }
    }

}
