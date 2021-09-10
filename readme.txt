The projects can be build with CMake. 
In the following, an installation manual for "Visual Studio Code" (lightweight) and "Visual Studio Community" is included.


1. Installation for CMake:

Download and install CMake from here: https://cmake.org/download/


2.1.1. Visual Studio Code Setup

Download and install Visual Studio Code: https://code.visualstudio.com/
Install the following extensions in Visual Studio Code (The symbol with the four bricks on the left side of the editor):
* C/C++
* Shader languages support for VS Code (optional)

Install a compiler for C++: Download and install Mingw64: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download
* select x86_64, posix, seh.
* Make sure that ..../mingw64/bin/ directory is in the PATH variable. (if you don't know how, see https://www.computerhope.com/issues/ch000549.htm)

In Visual Studio Code: Go to "File"->"Open Folder" and select the folder where this readme is placed in.
Press Ctrl+Shift+P to open the console in Visual Studio Code.
In the console type:
1. CMake: Select a kit -> and select GCC for mingw32
2. CMake: Configure -> this does the program set up
3. CMake: Select Variant -> choose debug for now
4. Cmake: Set Build Target -> All


2.1.2. Visual Studio Code Build and Debug

After 2.1.1, open the console again and type:
1. CMake: Build -> see if all files build correctly, if there are build errors please contact us (if the console outputs: "Build finished with exit code 0" everything worked correctly)
2. CMake: Set Debug Target -> Select a project you would like to debug. Start with example_00_window for example
3. CMake: Debug -> execute the selected debug target.

For each subsequent build, you only need to type:
* CMake: Debug


2.1.3 Visual Studio Code - Add new project

To add a new project, copy and rename one of the example projects.
Afterwards open the Visual Studio Console again and type:
1. CMake: Configure
2. CMake: Set Debug Target -> select your new project
3. CMake: Debug


2.2 Visual Studio Community

Download and install visual studio community from here: https://visualstudio.microsoft.com/de/vs/
During installation, select the C++ components.

Open the CMake GUI (cmake-gui) and select this directory as source directory.
For the build directory, choose any free directory you like.
In the GUI press "Configure" then "Generate" then "Open Project"
We won't explain how to use Visual Studio Community further, there are plenty of C++ Visual Studio tutorials on the web

To add a new project refer to 2.1.3 but repeat the steps inside the cmake-gui instead ("Configure" then "Generate" then "Open Project")

