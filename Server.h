#pragma once

#include <string>
#include <sstream>
#include "SaveFile.h";

#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

class Server{

private:
   WSADATA WsaData;
   SOCKET Socket;
   SOCKADDR_IN serverInfo;
public:
   Server();
   ~Server();

   void start(SaveFile f);
   void close();

   int initWinsock();
   int initSocket();

};