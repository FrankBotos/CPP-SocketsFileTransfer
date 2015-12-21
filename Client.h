#pragma once

#include "winsock2.h"
#include <vector>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")

class Client{

private:
   WSADATA WsaData;
   SOCKET Socket;
   struct hostent * host;
   SOCKADDR_IN SockAddress;

   char* recBinary;//this will receive binary info about file RECDATA2
   uint32_t recSizeInBytes;//this will recieve STREAMPOS var about file ie. represents size of file in bytes RECDATA1

   std::vector<char> eVec;//this will receive actual file extension of received file eg. a pdf file woudld send ".pdf" to this variable
   uint32_t extensionLength;//this will receive the #of characters taken up by the received file's extension

public:
   Client();
   virtual ~Client();

   void start();
   void close();

   int initWinsock();
   int initSocket();

   char* getBinary();
   uint32_t getSizeInBytes();
   std::vector<char> getExtension();

};