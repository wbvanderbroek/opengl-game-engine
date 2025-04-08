using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.Utilities;
using System.ComponentModel.Composition;

namespace VisualStudioExtension
{
    [Export(typeof(IWpfTextViewCreationListener))]
    [ContentType("csharp")]
    [TextViewRole(PredefinedTextViewRoles.Document)]
    internal sealed class EngineMessageAdornmentInitializer : IWpfTextViewCreationListener
    {
        public void TextViewCreated(IWpfTextView textView)
        {
            _ = new EngineMessageAdornmentManager(textView);
        }
    }
}
