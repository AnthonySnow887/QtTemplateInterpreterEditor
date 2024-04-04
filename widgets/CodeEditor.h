#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QRect>

#include "SyntaxHighlighter.h"

class LineNumberArea;

class CodeEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor() = default;

    int getFirstVisibleBlock();

    void setTabReplace(const bool enabled);
    bool isTabReplace() const;

    void setTabReplaceSize(const int val);
    int tabReplaceSize() const;

    void setSyntaxHighlighter(SyntaxHighlighter *highliter);

public Q_SLOTS:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect);

    void highlightErrorLine(const int line);
    void highlightWarningLine(const int line);
    void clearHighlightLine();

protected Q_SLOTS:
    void highlightLine(const QImage &image, const QColor &color, const int line);

protected:
    void paintEvent(QPaintEvent* e) override;

    void resizeEvent(QResizeEvent* e) override;

    void keyPressEvent(QKeyEvent* e) override;

private:
    LineNumberArea *_lineNumberArea {nullptr};
    SyntaxHighlighter *_highliter {nullptr};

    bool _isTabReplace {false};
    QString _tabReplaceStr;

    void updateLineGeometry();
};

#endif // CODEEDITOR_H
