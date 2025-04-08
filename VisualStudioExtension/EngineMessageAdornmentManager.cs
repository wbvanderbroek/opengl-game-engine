using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.Text.Formatting;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace VisualStudioExtension
{
    internal class EngineMessageAdornmentManager
    {
        private readonly IWpfTextView _view;
        private readonly IAdornmentLayer _layer;


        public EngineMessageAdornmentManager(IWpfTextView view)
        {
            _view = view;
            _layer = view.GetAdornmentLayer("EngineMessageAdornmentLayer");

            _view.LayoutChanged += OnLayoutChanged;
        }

        private void OnLayoutChanged(object sender, TextViewLayoutChangedEventArgs e)
        {
            if (!ActivationState.IsActive)
                return;

            _layer.RemoveAllAdornments();

            foreach (var line in _view.TextViewLines)
            {
                try
                {
                    var text = line.Extent.GetText();
                    foreach (var method in SupportedMethods.methods)
                    {
                        if (IsExactEngineMethodDeclaration(text, method))
                        {
                            AddAdornmentNextToMethodMetadata(line, method);
                            break;
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("Adornment error: " + ex.Message);
                }
            }
        }
        private bool IsExactEngineMethodDeclaration(string lineText, string method)
        {
            // Simplify: ignore case, trim extra whitespace
            var text = lineText.Trim();

            // Must contain 'void MethodName(' exactly (not preceded/followed by other text)
            // Valid matches:
            // - public void Update(
            // - private void LateUpdate(
            // - protected override void FixedUpdate(
            // Invalid:
            // - void CameraUpdate(
            // - void NotUpdate(

            int methodIndex = text.IndexOf($"void {method}(");
            if (methodIndex == -1)
                return false;

            // Check that the char after method is a (
            int nameStart = text.IndexOf(method);
            int nameEnd = nameStart + method.Length;

            // Check that next char is (
            if (nameEnd >= text.Length || text[nameEnd] != '(')
                return false;

            // Optional: check that char before "void" is space or start of line
            return true;
        }

        private void AddAdornmentNextToMethodMetadata(ITextViewLine methodLine, string method)
        {
            var snapshot = methodLine.Snapshot;
            var lineText = snapshot.GetLineFromPosition(methodLine.Start).GetText().TrimEnd();

            if (!lineText.Contains(method))
                return;

            // Find last char of the line
            int lastCharIndex = snapshot.GetLineFromPosition(methodLine.Start).Start + lineText.Length - 1;
            var lastCharPoint = new SnapshotPoint(snapshot, lastCharIndex);
            var charBounds = methodLine.GetCharacterBounds(lastCharPoint);

            // Try to get the next visible line (line after method)
            var nextPoint = new SnapshotPoint(snapshot, snapshot.GetLineFromLineNumber(methodLine.Start.GetContainingLine().LineNumber + 1).Start.Position);
            var nextLine = _view.GetTextViewLineContainingBufferPosition(nextPoint);

            // Fallback if next line isn’t loaded
            if (nextLine == null)
                return;

            var textBlock = new TextBlock
            {
                Text = $"{method} is called by the engine.",
                Foreground = Brushes.Gray,
                FontStyle = FontStyles.Italic,
                Background = Brushes.Transparent,
                Padding = new Thickness(4, 0, 0, 0)
            };

            textBlock.Measure(new Size(double.PositiveInfinity, double.PositiveInfinity));

            double x = charBounds.Right + 6; // same horizontal alignment
            double y = nextLine.Top - nextLine.Height;

            Canvas.SetLeft(textBlock, x);
            Canvas.SetTop(textBlock, y);

            var container = new Canvas();
            container.Children.Add(textBlock);

            _layer.AddAdornment(
                AdornmentPositioningBehavior.ViewportRelative,
                new SnapshotSpan(lastCharPoint, 0),
                null,
                container,
                null
            );
        }
    }
}
