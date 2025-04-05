using System;
using GameEngine;


    public class PlayerController
    {
        public Transform transform { get; set; }

        public void Start()
        {
            transform.position = new Vector3(3, 2, 1);
        }

        public void Update()
        {
            var pos = transform.position;
            pos.x += 1;
            transform.position = pos;        //Console.WriteLine($"[C#] Position is: {transform.position.x}, {transform.position.y}, {transform.position.z}");
        }
    }

