#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "widgets/CodeEditor.h"
#include "QtTemplateInterpreterEditorVersion.h"

#include <QRegExp>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("QtTemplate Interpreter Editor - %1")
                   .arg(QT_TEMPLATE_INTERPRETER_EDITOR_VERSION_STR));

    ui->toolBar->setMovable(false);
    QAction *act = ui->toolBar->addAction(QIcon(":/icons/icons/start_16.png"), QString("Run"));
    connect(act, &QAction::triggered, this, &MainWindow::onRun);

    _defaultTextColor = palette().color(QPalette::Text);

    QFont defFont = font();
    defFont.setPointSize(12);

    _codeWidget = new CodeEditor(this);
    _codeWidget->setFont(defFont);
    _codeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    _codeWidget->setSyntaxHighlighter(new SyntaxHighlighter(true, true));
    ui->verticalLayoutCode->addWidget(_codeWidget);

    _previewWidget = new CodeEditor(this);
    _previewWidget->setFont(defFont);
    _previewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    ui->verticalLayoutPreView->addWidget(_previewWidget);

    _consoleWidget = new CodeEditor(this);
    _consoleWidget->setFont(defFont);
    _consoleWidget->setEnabled(true);
    _consoleWidget->setReadOnly(true);
    _consoleWidget->setMaximumHeight(150);
    _consoleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    ui->verticalLayoutConsole->addWidget(_consoleWidget);

    //
    // Output value in console.
    //
    // [void] var_dump(value)
    //
    _interpreter.appendHelpFunction(new QtTIHelperFunction<QVariant>("var_dump", [this](const QVariant &val) {
        QString tmpStr;
        QDebug dbg(&tmpStr);
        dbg.nospace() << val;
        _consoleWidget->setTextColor(QColor(Qt::gray));
        _consoleWidget->append(QString("[Var-Dump] %1\n").arg(tmpStr).trimmed());
        return "";
    }));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::highlightErrorLine(const QString errorMsg)
{
    QRegExp rx(".*in line (\\d+) \\(position (\\d+)\\)");
    if (rx.lastIndexIn(errorMsg) == -1)
        return;
    _codeWidget->highlightErrorLine(rx.cap(1).toInt(), rx.cap(2).toInt());
}

void MainWindow::onRun()
{
    _consoleWidget->clear();
    _previewWidget->clear();
    _codeWidget->clearHighlightLine();

    const QString codeData = _codeWidget->toPlainText();
    if (codeData.isEmpty()) {
        _consoleWidget->setTextColor(QColor(Qt::red));
        _consoleWidget->append("[Error] Code data is Empty!");
        return;
    }

    // run
    QElapsedTimer eTimer;
    eTimer.start();
    bool isOk = false;
    QString result;
    QString error;
    std::tie(isOk, result, error) = _interpreter.interpret(codeData);
    const qint64 wTime = eTimer.elapsed();
    if (!isOk) {
        _consoleWidget->setTextColor(QColor(Qt::red));
        _consoleWidget->append(QString("[Error] %1").arg(error));
        highlightErrorLine(error);
        return;
    }

    // show result
    _previewWidget->setText(result);
    _consoleWidget->setTextColor(QColor(Qt::darkGreen));
    _consoleWidget->append(QString("Interpretation completed successfully in %1 seconds")
                           .arg(wTime / 1000.0));
}

