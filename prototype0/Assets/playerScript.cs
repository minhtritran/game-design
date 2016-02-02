using UnityEngine;
using System.Collections;

public class playerScript : MonoBehaviour {
    public string myName;
    public float speed = 1.0f;
    public float jumpForce = 4.0f;
    Rigidbody2D rg2d;
    public Transform groundCheck; // Put the prefab of the ground here
    public Transform projectile;
    public LayerMask groundLayer; // Insert the layer here.
    private bool isGrounded;
    public float cd = 0.15f;
    private float timer = 0.0f;
    private bool canShoot = true;
   
    private bool canRotate = true;
    private float rotate_timer = 0.0f;

    public float rotate_cd = 0.16f;
    public float rotate_force = 35.0f;
    public float rotate_upforce = 1.0f;
    // Use this for initialization
    void Start () {
        Debug.Log("I am alive!");
        rg2d = GetComponent<Rigidbody2D>();
        //Physics2D.IgnoreLayerCollision(9, 9);
   
    }
	
    void Update()
    {
        //isGrounded = Physics2D.OverlapCircle(groundCheck.position, 0.15f, groundLayer);
        if (timer < 1.0f)
            timer += Time.deltaTime;
        if (rotate_timer < 1.0f)
            rotate_timer += Time.deltaTime;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (Input.GetKey(KeyCode.A) && rotate_timer >= rotate_cd)
        {
            rotate_timer = 0.0f;
            rg2d.AddForce(new Vector2(0, rotate_upforce), ForceMode2D.Impulse);
            rg2d.AddTorque(rotate_force);

        }
        if (Input.GetKey(KeyCode.D) && rotate_timer >= rotate_cd)
        {
            rotate_timer = 0.0f;
            rg2d.AddForce(new Vector2(0, rotate_upforce), ForceMode2D.Impulse);
            rg2d.AddTorque(-rotate_force);

        }
        //rg2d.velocity = new Vector2(Input.GetAxis("Horizontal") * speed, rg2d.velocity.y);
        if (Input.GetKey(KeyCode.Space) && timer >= cd)
        {
            timer = 0.0f;
            Vector2 force = transform.up;
            rg2d.AddForce(force * jumpForce, ForceMode2D.Impulse);

            Transform bullet = (Transform)Instantiate(projectile, groundCheck.position, groundCheck.rotation);
            Rigidbody2D bulletrb2d = bullet.GetComponent<Rigidbody2D>();
            bulletrb2d.AddForce(force * jumpForce * -3, ForceMode2D.Impulse);
        }

  
    }
}
