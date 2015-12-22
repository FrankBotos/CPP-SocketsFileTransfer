# CPP-SocketsFileTransfer
A C++ program that uses Winsock2 to transfer files between computers. Because this is a rather specific project, using a Windows only library, there are a few requirements: Have visual studio installed on PC, and compile on Visual Studio! I compiled and tested this in Visual Studio 2013.

# Features Being Worked On
Currently, I am working on figuring out how far I can push this thing. What is the largest file I can transfer? So far, I've transferred a file of 600mb. Because this uses uint32_t data type to store all byte values, I expect to be able to transfer 4GB. I will need to experiment and see what's possible with this program. Ideally, it would be nice to be able to transfer files of any size with this reliably.

Secondly, I am exploring options to either implement a GUI, multi-file support, or both. But my main focus for the time being is functionality!

At some point, I might also create some sort of tutorial or documentation to talk about the possible uses and implementation of this program, as I do believe that it is modular enough (or has potential to be) to be implemented in other projects!
