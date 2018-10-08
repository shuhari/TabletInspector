# TabletInspector

Utility program, developed with `Visual C++` and `Qt`, to inspect and verify hardware properties and real-time data.

## Prerequirest

* Visual Studio 2017 with C++ toolset
* Qt 5.x (Tested for Qt **5.11.2**)
* Qt add-in for Visual Studio 2017

## Build From Source

First, environment variable `QTDIR` should be defined and point to Qt directory. otherwise, you should modify TabletInspector.vcxproj to satisfy build requirements.

Open `TabletInspector.sln` in Visual Studio and simply build solution. If no error happened, the generated file should be in `$(SolutionDir)\bin` directory, categoried with platform & configuration. 

The translation files are built with standard `Qt` translation toolset. Currently, only Chinese translation are provided. If you could not see the translation file (`*.qm`), then invoke `lrelease All .ts files in project` command in context menu to create the file. The project are equiped with command that any `.qm` file should be copied to output directory.


## Usage

Open built `.exe` file and conncted tablet. The tablet should be automatically detected and shown in the program. 

The program acess tablet through USB protocol. If any other program (such as tablet driver or USB debugging tools) is also visiting the same device, the program may not work. In such case, you should close those programs first.