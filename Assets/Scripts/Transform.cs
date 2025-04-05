using System;
using System.Runtime.CompilerServices;

namespace GameEngine
{
    public class Transform
    {
        private IntPtr m_nativePtr;

        public Transform() { }


        public Vector3 position
        {
            get
            {
                GameEngine_Transform_get_position_Injected(m_nativePtr, out Vector3 pos);
                return pos;
            }
            set
            {
                GameEngine_Transform_set_position_Injected(m_nativePtr, (Vector3)value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GameEngine_Transform_get_position_Injected(IntPtr nativePtr, out Vector3 outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GameEngine_Transform_set_position_Injected(IntPtr nativePtr, Vector3 value);

    }

}
