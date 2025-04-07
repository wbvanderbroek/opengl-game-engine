using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.Utilities;
using System.ComponentModel.Composition;

namespace VisualStudioExtension
{
    internal static class AdornmentLayerDefinitions
    {
        [Export(typeof(AdornmentLayerDefinition))]
        [Name("EngineMessageAdornmentLayer")]
        [Order(After = PredefinedAdornmentLayers.Text)]
        [TextViewRole(PredefinedTextViewRoles.Document)]
        public static AdornmentLayerDefinition EngineMessageLayer;
    }
}
