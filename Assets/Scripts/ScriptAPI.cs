// scriptapi.cs
using System;
using System.Runtime.CompilerServices;

namespace Engine
{
    public class GameObject
    {
        public string Name
        {
            get => GetName_Internal();
            set => SetName_Internal(value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetName_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetName_Internal(string name);
    }

    public class MyScript
    {
        public void Update()
        {
            Console.WriteLine("[C#] MyScript.Update() called");

            var obj = new GameObject();
            Console.WriteLine("[C#] Name from C++: " + obj.Name);
            obj.Name = "ChangedFromCSharp";
        }
    }
}
