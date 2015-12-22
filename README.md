# CPP-SocketsFileTransfer
A C++ program that uses Winsock2 to transfer files between computers. Because this is a rather specific project, using a Windows only library, there are a few requirements: Have visual studio installed on PC, and compile on Visual Studio! I compiled and tested this in Visual Studio 2013.

# Features Being Worked On
Right now I am working on making this capable of transferring files of all sizes! Because of the single send function it is not guaranteed that larger files make it across the network. So currently, I am working on a function that splits the data into a buffer, and loads it chunk by chunk, ensuring that the entire file is loaded.

Secondly, I am exploring options to either implement a GUI, multi-file support, or both. But my main focus for the time being is functionality!
