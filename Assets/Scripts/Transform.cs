using System.Runtime.CompilerServices;

namespace GameEngine
{
    public class Transform
    {
        public Vector3 position
        {
            get
            {
                get_position_Injected(out var ret);
                return ret;
            }
            set
            {
                set_position_Injected(ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void get_position_Injected(out Vector3 ret);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void set_position_Injected(ref Vector3 value);
    }
}