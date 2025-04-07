using EnvDTE;
using Microsoft.VisualStudio.Shell;
using System;

namespace VisualStudioExtension
{
    internal class ActivationCheck
    {
        public static bool ShouldActivate()
        {
            return true;
            try
            {
                var dte = (DTE)ServiceProvider.GlobalProvider.GetService(typeof(DTE));
                if (dte?.Solution?.Projects == null)
                    return false;

                foreach (Project project in dte.Solution.Projects)
                {
                    string projectFile = project.FullName;
                    if (!string.IsNullOrEmpty(projectFile) &&
                        projectFile.EndsWith("GameScripts.csproj", StringComparison.OrdinalIgnoreCase))
                    {
                        return true;
                    }
                }

            }
            catch (Exception ex)
            {
            }

            return false;
        }
    }
}