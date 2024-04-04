#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QVector>

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxHighlighter(const bool highlightCodeBlocks = false,
                      const bool checkHighlightInCodeBlocks = false,
                      QTextDocument *parent = 0);
    ~SyntaxHighlighter() = default;

    bool highlightCodeBlocks() const;
    bool checkHighlightInCodeBlocks() const;

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression _pattern;
        QTextCharFormat _format;
        bool _checkCodeBlock {false};
    };
    QVector<HighlightingRule> _highlightingRules;

    QRegularExpression _commentStartExpression;
    QRegularExpression _commentEndExpression;

    QTextCharFormat _codeBlocksFormat;
    QTextCharFormat _keywordFormat;
    QTextCharFormat _singleLineCommentFormat;
    QTextCharFormat _multiLineCommentFormat;
    QTextCharFormat _quotationFormat;
    QTextCharFormat _functionFormat;
    QTextCharFormat _digitsFormat;

    bool _highlightCodeBlocks {false};
    bool _checkHighlightInCodeBlocks {false};

    bool isIncludedInCodeBlock(const QString &text, int blockStart, int blockLength);
};

#endif // SYNTAXHIGHLIGHTER_H
