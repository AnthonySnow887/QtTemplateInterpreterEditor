# QtTemplateInterpreterEditor

[![License](https://img.shields.io/github/license/AnthonySnow887/QtTemplateInterpreterEditor)](https://github.com/AnthonySnow887/QtTemplateInterpreterEditor/blob/master/LICENSE)
[![Latest Release](https://img.shields.io/github/v/release/AnthonySnow887/QtTemplateInterpreterEditor?label=release)](https://github.com/AnthonySnow887/QtTemplateInterpreterEditor/releases)
![Last Commit](https://img.shields.io/github/last-commit/AnthonySnow887/QtTemplateInterpreterEditor/develop)

Text editor for the template interpreter QtTemplateInterpreter (https://github.com/AnthonySnow887/QtTemplateInterpreter) based on Qt 5.
This text editor supports syntax highlighting of template code and preview of the result of template execution.

## Build

To build, clone the project along with its submodules:

```bash
$> git clone --recurse-submodules https://github.com/AnthonySnow887/QtTemplateInterpreterEditor
```

Next, build the project:

```bash
$> qmake QtTemplateInterpreterEditor.pro
$> make
```

## Run

To start the editor use the command:

```bash
$> ./QtTemplateInterpreterEditor &
```

## Example of work

![Run](https://github.com/AnthonySnow887/QtTemplateInterpreterEditor/blob/main/Run.png?raw=true)
![Error](https://github.com/AnthonySnow887/QtTemplateInterpreterEditor/blob/main/Err.png?raw=true)
