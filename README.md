# Abaqus-UMAT-C++-subroutine

The Requirements are MSVS and an Intel Compiler. The procedure can be found here: https://www.youtube.com/watch?v=T3hAV8gfnec. Shoutout to him.

First: Compilation of the cpp-file.
You need MSVS to create an object-file in the MSVS Terminal. The Compilation Flags can be found in the win86_64 file from Abaqus. 
In my Example you also have to insert Eigen and its folder directionary.

Second: Execution.
In the Abaqus Terminal with the Command: abaqus user=umat job=umat analysis interactive 
Visualization with abaqus cae database=umat.odb
