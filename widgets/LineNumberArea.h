#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>

#include "CodeEditor.h"

class LineNumberArea : public QWidget
{
    Q_OBJECT
    friend class CodeEditor;
public:
    explicit LineNumberArea(CodeEditor *parent);
    ~LineNumberArea() = default;

    QSize sizeHint() const final;

protected:
    void paintEvent(QPaintEvent *event) final;

    void paintImage(const QImage &image, const int blockNum);
    void removeImage(const int blockNum);
    void removeImages();

private:
    CodeEditor *_codeEditor {nullptr};
    QMap<int, QImage> _bImages;
};

#endif // LINENUMBERAREA_H
