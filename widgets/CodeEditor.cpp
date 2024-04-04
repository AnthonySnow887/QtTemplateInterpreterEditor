#include "CodeEditor.h"
#include "LineNumberArea.h"

#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>

CodeEditor::CodeEditor(QWidget *parent)
    : QTextEdit(parent)
{
    _lineNumberArea = new LineNumberArea(this);

    connect(document(), &QTextDocument::blockCountChanged,
            this, &CodeEditor::updateLineNumberAreaWidth);

    connect(this, &QTextEdit::textChanged,
            this, &CodeEditor::clearHighlightLine);

    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            [this](int) {
        _lineNumberArea->update();
    });

    //    connect(this, &QTextEdit::cursorPositionChanged,
    //            this, &CodeEditor::updateExtraSelection);

    //    connect(this, &QTextEdit::selectionChanged,
    //            this, &CodeEditor::onSelectionChanged);

    setTabReplace(true);
    setTabReplaceSize(4);
}

int CodeEditor::getFirstVisibleBlock()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(document());
    curs.movePosition(QTextCursor::Start);
    for(int i = 0; i < document()->blockCount(); ++i) {
        QTextBlock block = curs.block();

        QRect r1 = viewport()->geometry();
        QRect r2 = document()->documentLayout()->blockBoundingRect(block).translated(viewport()->geometry().x(),
                                                                                     viewport()->geometry().y() - verticalScrollBar()->sliderPosition()).toRect();
        if (r1.intersects(r2))
            return i;

        curs.movePosition(QTextCursor::NextBlock);
    }
    return 0;
}

void CodeEditor::setTabReplace(const bool enabled)
{
    _isTabReplace = enabled;
}

bool CodeEditor::isTabReplace() const
{
    return _isTabReplace;
}

void CodeEditor::setTabReplaceSize(const int val)
{
    _tabReplaceStr.clear();
    _tabReplaceStr.fill(' ', val);
}

int CodeEditor::tabReplaceSize() const
{
    return _tabReplaceStr.size();
}

void CodeEditor::setSyntaxHighlighter(SyntaxHighlighter *highliter)
{
    if (_highliter)
        delete _highliter;
    if (!highliter)
        return;
    _highliter = highliter;
    _highliter->setParent(this);
    _highliter->setDocument(document());
}

void CodeEditor::updateLineNumberAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount)
    setViewportMargins(_lineNumberArea->sizeHint().width(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect)
{
    _lineNumberArea->update(0,
                            rect.y(),
                            _lineNumberArea->sizeHint().width(),
                            rect.height());
    updateLineGeometry();
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::highlightErrorLine(const int line)
{
    highlightLine(QImage(":/icons/icons/error_16.png"), QColor(Qt::red).lighter(185), line);
}

void CodeEditor::highlightWarningLine(const int line)
{
    highlightLine(QImage(":/icons/icons/warning_16.png"), QColor("#e5be22").lighter(185), line);
}

void CodeEditor::clearHighlightLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(Qt::transparent));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
    _lineNumberArea->removeImages();
}

void CodeEditor::highlightLine(const QImage &image, const QColor &color, const int line)
{
    if (document()->blockCount() < (line - 1))
        return;

    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()) {
        QTextCursor cursor(document()->findBlockByNumber(line - 1));
        setTextCursor(cursor);

        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(color);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
        _lineNumberArea->paintImage(image, (line - 1));
    }
    setExtraSelections(extraSelections);
}

void CodeEditor::paintEvent(QPaintEvent *e)
{
    updateLineNumberArea(e->rect());
    QTextEdit::paintEvent(e);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);
    updateLineGeometry();
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (_isTabReplace
        && e->key() == Qt::Key_Tab
        && e->modifiers() == Qt::NoModifier)
        return insertPlainText(_tabReplaceStr);

    QTextEdit::keyPressEvent(e);
}

void CodeEditor::updateLineGeometry()
{
    QRect cr = contentsRect();
    _lineNumberArea->setGeometry(QRect(cr.left(),
                                       cr.top(),
                                       _lineNumberArea->sizeHint().width(),
                                       cr.height()));
}
