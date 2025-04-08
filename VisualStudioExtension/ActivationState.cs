using Microsoft.VisualStudio.Shell;
using System;
using System.IO;

namespace VisualStudioExtension
{
    internal class ActivationState
    {
        public static bool IsActive { get; private set; } = false;
        public static void Update()
        {
            ThreadHelper.ThrowIfNotOnUIThread();

            var dte = (EnvDTE.DTE)ServiceProvider.GlobalProvider.GetService(typeof(EnvDTE.DTE));
            var doc = dte?.ActiveDocument;
            if (doc == null)
            {
                IsActive = false;
                return;
            }

            var projectItem = doc.ProjectItem;
            var containingProject = projectItem?.ContainingProject;

            IsActive = containingProject != null &&
                       string.Equals(containingProject.Name, "Game", StringComparison.OrdinalIgnoreCase) &&
                       Path.GetFileNameWithoutExtension(dte.Solution.FullName)
                           .Equals("opengl-game-engine", StringComparison.OrdinalIgnoreCase);
        }
    }
}
