#pragma once

#include "winsock2.h"
#include <vector>
#pragma comment(lib, "ws2_32.lib")

#define DATA_SIZE 10000000//10 million bytes, or 10mb


class Client{

private:
   WSADATA WsaData;
   SOCKET Socket;
   struct hostent * host;
   SOCKADDR_IN SockAddress;

   char* recBinary;//this will receive binary info about file RECDATA2
   _Uint32t recSizeInBytes;//this will recieve STREAMPOS var about file ie. represents size of file in bytes RECDATA1

   std::vector<char> eVec;//this will receive actual file extension of received file eg. a pdf file woudld send ".pdf" to this variable
   _Uint32t extensionLength;//this will receive the #of characters taken up by the received file's extension

public:
   Client();
   ~Client();

   void start();
   void close();

   int initWinsock();
   int initSocket();

   char* getBinary();
   _Uint32t getSizeInBytes();
   std::vector<char> getExtension();

};