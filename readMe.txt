To run the project, do the following:

1.	Be using the Windows 10 operating system (Windows 11 might work, but I have not tested it)
2.	Ensure you have CMAKE installed and added to the system path on your computer.
3.	Ensure you have Visual Studio 2022 installed with the C++ workload.


If the above are all as stated, perform the following steps:

1.	When you pulled this repo from git, ensure that you used git clone --recurse-submodules <repository-url>  (otherwise, the assimp submodule will not be included)
2.	In the Solution Directory, navigate to dep/assimp
3.	In the “assimp” directory, open a bash terminal and type “cmake CMakeLists.txt” an push enter.
4.	Then type “cmake --build .” and push enter again. This will set up the assimp submodule.
5.	Currently, there is an issue where the lib files do not appear in the dependencies after cloning and building.
6.	The current remedy is to return to the solution directory and open the “extra” directory. In there, you will find a directory called “lib”
7.	Copy the whole folder and paste it into the dep/assimp directory.
8.	Now double-click the visual studio solution file in the solution directory “zenditeEngineV2.sln” to open it in Visual Studio 2022.
9.	Press F5 to build and run.
