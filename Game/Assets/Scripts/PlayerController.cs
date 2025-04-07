using GameEngine;

public class PlayerController
{
    public Transform transform { get; set; }

    private void Start()
    {
        transform.position = new Vector3(3, 2, 1);
    }

    private void Update()
    {
        var pos = transform.position;
        pos.x += 1;
        transform.position = pos;
    }
}