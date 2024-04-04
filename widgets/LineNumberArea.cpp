#include "LineNumberArea.h"

#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>

LineNumberArea::LineNumberArea(CodeEditor *parent)
    : QWidget(parent)
    , _codeEditor(parent)
{
}

QSize LineNumberArea::sizeHint() const
{
    if (_codeEditor == nullptr)
        return QWidget::sizeHint();

    // Calculating width
    int digits = 1;
    int max = qMax(1, _codeEditor->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

#if QT_VERSION >= 0x050B00
    int space = 13 + 16 + _codeEditor->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits; // 16 = image(16x16)
#else
    int space = 13 + 16 + _codeEditor->fontMetrics().width(QLatin1Char('9')) * digits; // 16 = image(16x16)
#endif

    return QSize(space, 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QColor("#f0f0f0"));
    painter.fillRect(event->rect().width() - 2, event->rect().y(),
                     1, event->rect().height(),
                     QColor("#a5a8a9"));

    int blockNumber = _codeEditor->getFirstVisibleBlock();
    QTextBlock block = _codeEditor->document()->findBlockByNumber(blockNumber);
    int top = (int) _codeEditor->document()->documentLayout()->blockBoundingRect(block).translated(0, -_codeEditor->verticalScrollBar()->value()).top();
    int bottom = top + (int) _codeEditor->document()->documentLayout()->blockBoundingRect(block).height();

    const QColor currentLine = QColor("#828282");
    const QColor otherLines  = QColor("#a5a8a9");

    painter.setFont(_codeEditor->font());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            bool isCurrentLine = _codeEditor->textCursor().blockNumber() == blockNumber;
            painter.setPen(isCurrentLine ? currentLine : otherLines);

            painter.drawText(-5,
                             top,
                             sizeHint().width(),
                             _codeEditor->fontMetrics().height(),
                             Qt::AlignRight,
                             number);

            if (_bImages.contains(blockNumber)) {
                const QImage image = _bImages.value(blockNumber);
                painter.drawImage(QPoint(4, top + 4), image); // 4 = image(16x16) / 4
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) _codeEditor->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void LineNumberArea::paintImage(const QImage &image, const int blockNum)
{
    _bImages.insert(blockNum, image);
    update();
}

void LineNumberArea::removeImage(const int blockNum)
{
    if (!_bImages.contains(blockNum))
        return;
    _bImages.remove(blockNum);
    update();
}

void LineNumberArea::removeImages()
{
    if (_bImages.isEmpty())
        return;
    _bImages.clear();
    update();
}
