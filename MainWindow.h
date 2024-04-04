#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtTemplateInterpreter/QtTemplateInterpreter.h>

#include "widgets/CodeEditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void highlightErrorLine(const QString errorMsg);

private:
    Ui::MainWindow *ui;
    CodeEditor *_codeWidget {nullptr};
    CodeEditor *_previewWidget {nullptr};
    CodeEditor *_consoleWidget {nullptr};

    QtTemplateInterpreter _interpreter;
    QColor _defaultTextColor;

protected slots:
    void onRun();
};
#endif // MAINWINDOW_H
