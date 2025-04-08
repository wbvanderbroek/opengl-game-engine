using System.Collections.Generic;

namespace VisualStudioExtension
{
    internal class SupportedMethods
    {
        public readonly static HashSet<string> methods = new HashSet<string>()
        {
            "Start", "Update", "LateUpdate", "Awake", "OnQuit", "OnDestroy"
        };
    }
}
