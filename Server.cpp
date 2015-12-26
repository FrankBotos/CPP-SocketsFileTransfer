#include "Server.h"
#include <stdint.h>
#include <iostream>
#include <sstream>

Server::Server(){
   initWinsock();
   initSocket();
   start();
}
Server::~Server(){}

int Server::initWinsock(){
   if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0){
      std::cout << "Winsock could not initialize!\n";
      WSACleanup();
      return 1;
   }
   return 0;
}

int Server::initSocket(){
   Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (Socket == INVALID_SOCKET){
      std::cout << "Socket creation failed!";
      return 1;
   }

   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr.s_addr = INADDR_ANY;
   serverInfo.sin_port = htons(4444);

   if (bind(Socket, (SOCKADDR*)(&serverInfo), sizeof(serverInfo)) == SOCKET_ERROR){
      std::cout << "Was unable to bind socket!\n";
      WSACleanup();
      return 2;
   }
   return 0;
}

void Server::start(){
   FileCopier f;
   f.setIFileName();
   f.findFileSize();
   f.findNumChunks();

   listen(Socket, 1);
   SOCKET tempSocket = SOCKET_ERROR;
   while (tempSocket == SOCKET_ERROR){
      std::cout << "Waiting for incoming connections...\n";
      tempSocket = accept(Socket, NULL, NULL);
   }
   Socket = tempSocket;
   std::cout << "\n\nClient has connected!\n\n";

   //WE WILL SEND DATA RIGHT HERE
   sendData(f);

   close();
}

void Server::close(){
   shutdown(Socket, SD_SEND);
   closesocket(Socket);
   WSACleanup();
}

void Server::sendData(FileCopier& f){
   /* STEPS
   1. send size of file in bytes
   2. send file extension length
   3. send actual extension
   4. break up data into chunks, send each chunk one by one, until no more chunks remain
   */

   //1. send size of file in bytes
   uint32_t fileSize = f.getSize();
   uint32_t bytesSent = 0;

   std::cout << "Sending filesize... ";
   while (bytesSent != sizeof(fileSize)){
      bytesSent = send(Socket, (char*)(&fileSize), sizeof(fileSize), 0);
      if (bytesSent == sizeof(fileSize)){
         std::cout << "--->Filesize successfully sent!\n";
      }
   }

   //2. send file extension length
   uint32_t extensionLength = strlen(f.getExtension());
   bytesSent = 0;//reset variable for use with next send

   std::cout << "Sending file extension length... ";
   while (bytesSent != sizeof(extensionLength)){
      bytesSent = send(Socket, (char*)(&extensionLength), sizeof(extensionLength), 0);
      if (bytesSent == sizeof(extensionLength)){
         std::cout << "--->File extension length successfuly sent!\n";
      }
   }

   //3. send actual extension as c-style string!
   const char* extension = f.getExtension();
   bytesSent = 0;

   std::cout << "Sending file extension... ";
   while (bytesSent != sizeof(extension)){
      bytesSent = send(Socket, extension, sizeof(extension), 0);
      if (bytesSent == sizeof(extension)){
         std::cout << "--->File extension successfully sent!\n";
      }
   }

   //4. send data in chunks

   //send one chunk at a time, until all chunks sent
   //for last chunk, we can simply ignore extra data and parse on client side (this makes for simpler code on both ends)
   uint32_t chunksSent = 0;
   while (chunksSent <= f.getNumChunks()){
      char* currentChunk = new char[f.getChunkSize()];
      f.readChunk(currentChunk, f.getChunkSize());
      uint32_t numBytesSent = 0;
      uint32_t numBytesLeft = f.getChunkSize();
      uint32_t n;

      while (numBytesSent < numBytesLeft) {
         n = send(Socket, currentChunk + numBytesSent, numBytesLeft, 0);
         if (n > 0){
            numBytesSent += n;
            numBytesLeft -= n;
            std::cout << "\rSENT ->" << numBytesSent << "/" << f.getChunkSize();
         }
      }
      delete[] currentChunk;
      std::cout << " ----- CHUNKS SENT: " << chunksSent;
      chunksSent++;
      std::cout << std::endl;
   }

}