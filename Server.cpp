#include <iostream>
#include "Server.h"

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

   //retreiving data to be sent
   char* retData1 = f.getMemBlock();
   _Uint32t fileSize = f.getSize();
   std::string s = std::to_string(fileSize);
   char const* retData2 = s.c_str();

   std::cout << fileSize << std::endl;
   std::cout << retData2;

   //sending data
   send(Socket, (char*)&fileSize, sizeof(fileSize), 0);
   send(Socket, retData1, fileSize, 0);



   //SEND EXTENSION LENGTH
   _Uint32t extensionLength = htonl(strlen(f.getExtension()));
   send(Socket, (const char*)(&extensionLength), sizeof(_Uint32t), 0);
   //SEND ACTUAL EXTENSION
   const char* extension = f.getExtension();
   send(Socket, extension, strlen(extension), 0);

   close();
}

void Server::close(){
   shutdown(Socket, SD_SEND);
   closesocket(Socket);
   WSACleanup();
}