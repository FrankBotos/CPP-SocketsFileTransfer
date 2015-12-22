#include <iostream>
#include "Server.h"
#include <stdint.h>

Server::Server(){
   initWinsock();
   initSocket();
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

//OLD NON WORKING CODE - FOR COMPARISON/LEARNING PURPOSES
//void Server::start(SaveFile f){
//   listen(Socket, 1);
//   SOCKET tempSocket = SOCKET_ERROR;
//   while (tempSocket == SOCKET_ERROR){
//      std::cout << "Waiting for incoming connections...\n";
//      tempSocket = accept(Socket, NULL, NULL);
//   }
//   Socket = tempSocket;
//
//   std::cout << "\n\nClient has connected!\n\n";
//
//   //retreiving data to be sent
//   char* retData1 = f.getMemBlock();
//   _Uint32t fileSize = f.getSize();
//   std::string s = std::to_string(fileSize);
//   char const* retData2 = s.c_str();
//
//   std::cout << fileSize << std::endl;
//   std::cout << retData2;
//
//   //sending data
//   send(Socket, (char*)&fileSize, sizeof(fileSize), 0);
//   send(Socket, retData1, fileSize, 0);
//   close();
//}

//CODE SUPPLIED BY ED
void Server::start(SaveFile f){
   listen(Socket, 1);
   SOCKET tempSocket = SOCKET_ERROR;
   while (tempSocket == SOCKET_ERROR){
      std::cout << "Waiting for incoming connections...\n";
      tempSocket = accept(Socket, NULL, NULL);
   }
   Socket = tempSocket;

   std::cout << "\n\nClient has connected!\n\n";

   sendData(f);

   close();
}

void Server::close(){
   shutdown(Socket, SD_SEND);
   closesocket(Socket);
   WSACleanup();
}

void Server::sendData(SaveFile f){
   /* STEPS
   1. send size of file in bytes
   2. send file extension length, 
   3. then send actual extension
   4. while loop to make sure all of file is sent
   */

   //1. send the size of the file
   uint32_t fileSizeInBytes = f.getSize();
   send(Socket, (char*)&fileSizeInBytes, sizeof(fileSizeInBytes), 0);
   
   //2. send the file extension length
   uint32_t extensionLength = htonl(strlen(f.getExtension()));
   send(Socket, (const char*)(&extensionLength), sizeof(uint32_t), 0);

   //3. send actual file extension as c-style string
   const char* extension = f.getExtension();
   send(Socket, extension, strlen(extension), 0);

   //4. while loop to send all data in file
   const char* fileInBinary = f.getMemBlock();
   uint32_t totalBytesSent = 0;
   uint32_t bytesLeft = fileSizeInBytes;
   int n;
   
   while (totalBytesSent < bytesLeft){
      n = send(Socket, fileInBinary + totalBytesSent, bytesLeft, 0);
      if (n == -1) { break; }
      totalBytesSent += n;
      bytesLeft -= n;
      std::cout << "\r" << totalBytesSent / 1000000 << "/" << fileSizeInBytes / 1000000 << " MB sent.";//divide by a million to find MB value
   }
   std::cout << "Data transfer complete... ERROR CODE (-1 means transfer failed!): " << n << std::endl;

}