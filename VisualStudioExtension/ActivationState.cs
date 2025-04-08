using EnvDTE;
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

            try
            {
                var dte = (DTE)ServiceProvider.GlobalProvider.GetService(typeof(DTE));
                if (dte == null || dte.Solution == null || string.IsNullOrEmpty(dte.Solution.FullName))
                {
                    IsActive = false;
                    return;
                }

                var doc = dte.ActiveDocument;
                if (doc?.ProjectItem?.ContainingProject == null)
                {
                    IsActive = false;
                    return;
                }

                string projectName = doc.ProjectItem.ContainingProject.Name;
                string solutionName = Path.GetFileNameWithoutExtension(dte.Solution.FullName);

                IsActive = string.Equals(projectName, "Game", StringComparison.OrdinalIgnoreCase) &&
                           string.Equals(solutionName, "opengl-game-engine", StringComparison.OrdinalIgnoreCase);

            }
            catch (Exception ex)
            {
                IsActive = false;
            }
        }

    }
}
