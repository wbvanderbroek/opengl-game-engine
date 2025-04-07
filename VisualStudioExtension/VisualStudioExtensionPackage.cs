using Microsoft.VisualStudio.Language.Intellisense.AsyncCompletion;
using Microsoft.VisualStudio.Language.Intellisense.AsyncCompletion.Data;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Adornments;
using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.Utilities;
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.ComponentModel.Composition;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using Task = System.Threading.Tasks.Task;

namespace VisualStudioExtension
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [Guid(VisualStudioExtensionPackage.PackageGuidString)]
    [ProvideAutoLoad(Microsoft.VisualStudio.Shell.Interop.UIContextGuids80.SolutionExists, PackageAutoLoadFlags.BackgroundLoad)]
    public sealed class VisualStudioExtensionPackage : AsyncPackage
    {
        /// <summary>
        /// VisualStudioExtensionPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "5a8ac03f-c90f-4e36-b7de-8eae1f21ac58";

        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        /// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
        /// <param name="progress">A provider for progress updates.</param>
        /// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            Debug.WriteLine("InitializePackage");
            // When initialized asynchronously, the current thread may be a background thread at this point.
            // Do any initialization that requires the UI thread after switching to the UI thread.
            await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);
        }

        #endregion
    }

    [Export(typeof(IAsyncCompletionSourceProvider))]
    [ContentType("csharp")]
    [Name("EngineCompletionAsyncProvider")]
    public class EngineCompletionProvider : IAsyncCompletionSourceProvider
    {
        public IAsyncCompletionSource GetOrCreate(ITextView textView)
        {
            Debug.WriteLine("InitializePackage2");

            if (!ActivationCheck.ShouldActivate())
                return null;

            return new EngineAsyncCompletionSource();
        }
    }

    public class EngineAsyncCompletionSource : IAsyncCompletionSource
    {
        public Task<CompletionContext> GetCompletionContextAsync(
            IAsyncCompletionSession session,
            CompletionTrigger trigger,
            SnapshotPoint triggerLocation,
            SnapshotSpan applicableToSpan,
            CancellationToken cancellationToken)
        {
            string fullText = applicableToSpan.GetText().Trim();
            string[] tokens = fullText.Split(new[] { ' ', '\t', '(', ')' }, StringSplitOptions.RemoveEmptyEntries);
            string typedText = tokens.Length > 0 ? tokens[tokens.Length - 1] : string.Empty;

            Debug.WriteLine($"Full line: '{fullText}', TypedText: '{typedText}'");

            if (string.IsNullOrWhiteSpace(typedText))
                return Task.FromResult(CompletionContext.Empty);

            string fileText = triggerLocation.Snapshot.GetText();
            var results = new List<CompletionItem>();

            foreach (var method in SupportedMethods.methods)
            {
                bool alreadyExists = fileText.Contains($"void {method}(");
                bool matches = method.StartsWith(typedText, StringComparison.OrdinalIgnoreCase);

                Debug.WriteLine($"Checking: {method}, Exists: {alreadyExists}, Matches: {matches}");

                if (matches && !alreadyExists)
                {
                    string displayText = method;
                    bool isFullLine = false;
                    var line = triggerLocation.GetContainingLine();
                    string lineText = line.GetText().Trim();

                    if (lineText.StartsWith("private void", StringComparison.OrdinalIgnoreCase))
                    {
                        isFullLine = true;
                    }

                    string fullLineText = line.GetText();
                    int indentLength = fullLineText.Length - fullLineText.TrimStart().Length;
                    string baseIndent = fullLineText.Substring(0, indentLength);
                    string deeperIndent = baseIndent + (baseIndent.Contains("\t") ? "\t" : "    ");

                    string insertText;

                    if (isFullLine)
                    {
                        insertText =
                            $"{baseIndent}private void {method}()\n" +
                            $"{baseIndent}{{\n" +
                            $"{deeperIndent}\n" +
                            $"{baseIndent}}}";
                    }
                    else
                    {
                        insertText =
                            $"private void {method}()\n" +
                            $"{baseIndent}{{\n" +
                            $"{deeperIndent}\n" +
                            $"{baseIndent}}}";
                    }

                    var item = new CompletionItem(
                        displayText: displayText,
                        insertText: insertText,
                        sortText: displayText,
                        filterText: displayText,
                        automationText: displayText,
                        source: this,
                        filters: ImmutableArray<CompletionFilter>.Empty,
                        icon: ImageElement.Empty,
                        suffix: string.Empty,
                        attributeIcons: ImmutableArray<ImageElement>.Empty
                    );

                    results.Add(item);
                }
            }

            return Task.FromResult(new CompletionContext(results.ToImmutableArray()));
        }

        public Task<object> GetDescriptionAsync(
            IAsyncCompletionSession session,
            CompletionItem item,
            CancellationToken token)
        {
            return Task.FromResult<object>($"Engine method: {item.DisplayText}()");
        }

        public CompletionStartData InitializeCompletion(
            CompletionTrigger trigger,
            SnapshotPoint triggerLocation,
            CancellationToken token)
        {
            // Only handle letter inputs (skip spaces, punctuation, etc.)
            if (trigger.Reason == CompletionTriggerReason.Insertion &&
                !char.IsLetter(trigger.Character))
            {
                return CompletionStartData.DoesNotParticipateInCompletion;
            }

            var snapshot = triggerLocation.Snapshot;
            int start = triggerLocation.Position;

            // Walk backward to get the identifier (e.g., "Aw" or "Onq")
            while (start > 0 && char.IsLetter(snapshot[start - 1]))
            {
                start--;
            }

            var wordSpan = new SnapshotSpan(snapshot, start, triggerLocation.Position - start);
            var line = triggerLocation.GetContainingLine();
            string lineText = line.GetText().Trim();


            // If the line starts with public void, we want to replace the whole line
            if (lineText.StartsWith("private void", StringComparison.OrdinalIgnoreCase))
            {
                Debug.WriteLine(">> TRIGGER: Replacing full line");
                return new CompletionStartData(
                    CompletionParticipation.ProvidesItems,
                    new SnapshotSpan(line.Start, line.Length));
            }

            return new CompletionStartData(
                CompletionParticipation.ProvidesItems,
                wordSpan);
        }
    }
}