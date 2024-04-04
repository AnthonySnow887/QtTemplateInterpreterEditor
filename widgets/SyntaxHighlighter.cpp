#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(const bool highlightCodeBlocks,
                                     const bool checkHighlightInCodeBlocks,
                                     QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , _highlightCodeBlocks(highlightCodeBlocks)
    , _checkHighlightInCodeBlocks(checkHighlightInCodeBlocks)
{
    HighlightingRule rule;

    // code blocks
    if (_highlightCodeBlocks) {
        _codeBlocksFormat.setForeground(QBrush(QColor("#bf8002")));
        const QString keywordPatterns[] = {
            QStringLiteral(R"((?<block>{{)((?!}}).)*}})"),
            QStringLiteral(R"((?<block>{%)((?!%}).)*%})"),
            QStringLiteral(R"({{((?!}}).)*(?<block>}}))"),
            QStringLiteral(R"({%((?!%}).)*(?<block>%}))")
        };
        for (const QString &pattern : keywordPatterns) {
            rule._pattern = QRegularExpression(pattern);
            rule._format = _codeBlocksFormat;
            rule._checkCodeBlock = false;
            _highlightingRules.append(rule);
        }
    }

    // functions
    _functionFormat = _codeBlocksFormat;
    _functionFormat.setForeground(QBrush(QColor("#e454e5")));
    const QString functionPatterns[] = {
        QStringLiteral(R"(\b(?<block>([A-Za-z0-9_]+(?:\.))*([A-Za-z0-9_]+)\s*(?=\()))"),
    };
    for (const QString &pattern : functionPatterns) {
        rule._pattern = QRegularExpression(pattern);
        rule._format = _functionFormat;
        rule._checkCodeBlock = _checkHighlightInCodeBlocks;
        _highlightingRules.append(rule);
    }

    // blocks & keywords
    _keywordFormat = _codeBlocksFormat;
    _keywordFormat.setForeground(QBrush(QColor("#fd7608"))); //  e66d0b
    const QString blockPatterns[] = {
        QStringLiteral("{%\\s*(?<block>set)\\s+[A-Za-z0-9_]+\\s*=((?!%}).)*%}"),
        QStringLiteral("{%\\s*(?<block>unset)\\s+[A-Za-z0-9_]+\\s*%}"),
        QStringLiteral("{%\\s*(?<block>for)\\s+([A-Za-z0-9_]+|[A-Za-z0-9_]+\\s*,\\s*[A-Za-z0-9_]+)\\s+in\\s+((?!%}).)*%}"),
        QStringLiteral("{%\\s*for\\s+([A-Za-z0-9_]+|[A-Za-z0-9_]+\\s*,\\s*[A-Za-z0-9_]+)\\s+(?<block>in)\\s+((?!%}).)*%}"),
        QStringLiteral("{%\\s*(?<block>endfor)\\s*%}"),
        QStringLiteral("{%\\s*(?<block>if)\\s+((?!%}).)*%}"),
        QStringLiteral("{%\\s*(?<block>elseif)\\s+((?!%}).)*%}"),
        QStringLiteral("{%\\s*(?<block>else)\\s*%}"),
        QStringLiteral("{%\\s*(?<block>endif)\\s*%}")
    };
    for (const QString &pattern : blockPatterns) {
        rule._pattern = QRegularExpression(pattern);
        rule._format = _keywordFormat;
        rule._checkCodeBlock = false;
        _highlightingRules.append(rule);
    }
    const QString keywordPatterns[] = {
        QStringLiteral("\\b(?<block>(true|True|TRUE|false|False|FALSE))\\b"),
        QStringLiteral("\\b(?<block>(true|True|TRUE|false|False|FALSE))\\b"),
        QStringLiteral("\\b\\s+(?<block>(and|or|&&|\\|\\|))\\s+\\b"),
        QStringLiteral("\\b(?<block>(not))\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule._pattern = QRegularExpression(pattern);
        rule._format = _keywordFormat;
        rule._checkCodeBlock = _checkHighlightInCodeBlocks;
        _highlightingRules.append(rule);
    }

    // digits
    _digitsFormat = _codeBlocksFormat;
    _digitsFormat.setForeground(QBrush(QColor("#257df7")));
    const QString digitPatterns[] = {
        QStringLiteral(R"((?<block>(\+|\-)?(i|ui|l|ul)?\d+))"),
        QStringLiteral(R"((?<block>(\+|\-)?(d|f)?\d+\.\d+))")
    };
    for (const QString &pattern : digitPatterns) {
        rule._pattern = QRegularExpression(pattern);
        rule._format = _digitsFormat;
        rule._checkCodeBlock = _checkHighlightInCodeBlocks;
        _highlightingRules.append(rule);
    }

    // strings
    _quotationFormat = _codeBlocksFormat;
    _quotationFormat.setForeground(Qt::darkGreen);
    const QString stringPatterns[] = {
        QStringLiteral(R"("[^\n"]*")"),
        QStringLiteral(R"('[^\n']*')")
    };
    for (const QString &pattern : stringPatterns) {
        rule._pattern = QRegularExpression(pattern);
        rule._format = _quotationFormat;
        rule._checkCodeBlock = _checkHighlightInCodeBlocks;
        _highlightingRules.append(rule);
    }

    // single line comment
    _singleLineCommentFormat.setForeground(Qt::lightGray);
    rule._pattern = QRegularExpression(QStringLiteral("{#[^\n]*#}"));
    rule._format = _singleLineCommentFormat;
    _highlightingRules.append(rule);

    // multi line comment
    _multiLineCommentFormat.setForeground(Qt::lightGray);
    _commentStartExpression = QRegularExpression(QStringLiteral("{#"));
    _commentEndExpression = QRegularExpression(QStringLiteral("#}"));
}

bool SyntaxHighlighter::highlightCodeBlocks() const
{
    return _highlightCodeBlocks;
}

bool SyntaxHighlighter::checkHighlightInCodeBlocks() const
{
    return _checkHighlightInCodeBlocks;
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(_highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule._pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            int blockStart = match.capturedStart("block");
            int blockLength = match.capturedLength("block");
            if (blockStart != -1 && blockLength > 0) {
                if (rule._checkCodeBlock
                    && !isIncludedInCodeBlock(text, blockStart, blockLength))
                    continue;
                setFormat(blockStart, blockLength, rule._format);
            } else {
                if (rule._checkCodeBlock
                    && !isIncludedInCodeBlock(text, match.capturedStart(), match.capturedLength()))
                    continue;
                setFormat(match.capturedStart(), match.capturedLength(), rule._format);
            }
        }
    }

    // multi line comment
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(_commentStartExpression);
    while (startIndex >= 0) {
        QRegularExpressionMatch match = _commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, _multiLineCommentFormat);
        startIndex = text.indexOf(_commentStartExpression, startIndex + commentLength);
    }
}

bool SyntaxHighlighter::isIncludedInCodeBlock(const QString &text, int blockStart, int blockLength)
{
    QVector<QRegularExpression> checkRx;
    checkRx.append(QRegularExpression(QStringLiteral(R"({{((?!}}).)*}})")));
    checkRx.append(QRegularExpression(QStringLiteral(R"({%((?!%}).)*%})")));
    for (const QRegularExpression &rx : qAsConst(checkRx)) {
        QRegularExpressionMatchIterator matchIterator = rx.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            const int capStart = match.capturedStart();
            const int capLength = match.capturedLength();
            if (capStart < blockStart && capLength > blockLength)
                return true;
        }
    }
    return false;
}
