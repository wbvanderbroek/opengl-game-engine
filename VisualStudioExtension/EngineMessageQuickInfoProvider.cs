using Microsoft.VisualStudio.Language.Intellisense;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Utilities;
using System.ComponentModel.Composition;
using System.Threading;
using System.Threading.Tasks;

namespace VisualStudioExtension
{
    [Export(typeof(IAsyncQuickInfoSourceProvider))]
    [Name("EngineMessageQuickInfo")]
    [ContentType("csharp")]
    internal class EngineMessageQuickInfoProvider : IAsyncQuickInfoSourceProvider
    {
        public IAsyncQuickInfoSource TryCreateQuickInfoSource(ITextBuffer textBuffer)
        {
            if (!ActivationCheck.ShouldActivate())
                return null;

            return new EngineMessageQuickInfoSource(textBuffer);
        }
    }

    internal class EngineMessageQuickInfoSource : IAsyncQuickInfoSource
    {
        private readonly ITextBuffer buffer;

        public EngineMessageQuickInfoSource(ITextBuffer textBuffer)
        {
            buffer = textBuffer;
        }

        public async Task<QuickInfoItem> GetQuickInfoItemAsync(IAsyncQuickInfoSession session, CancellationToken cancellationToken)
        {
            var triggerPoint = session.GetTriggerPoint(buffer);
            if (triggerPoint == null)
                return null;

            var snapshotPoint = triggerPoint.GetPoint(buffer.CurrentSnapshot);
            var snapshot = snapshotPoint.Snapshot;
            var line = snapshot.GetLineFromPosition(snapshotPoint.Position);
            var text = line.GetText();

            foreach (var method in SupportedMethods.methods)
            {
                int index = text.IndexOf(method);
                if (index >= 0)
                {
                    var spanStart = line.Start + index;
                    var snapshotSpan = new SnapshotSpan(snapshot, spanStart, method.Length);
                    var trackingSpan = snapshot.CreateTrackingSpan(snapshotSpan, SpanTrackingMode.EdgeInclusive);
                    return new QuickInfoItem(trackingSpan, $"Engine Message: {method}");
                }
            }

            return null;
        }


        public void Dispose() { }
    }
}
