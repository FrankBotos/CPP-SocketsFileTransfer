#include <iostream>
#include "Client.h"
#include <string>
#include <vector>

Client::Client(){
   recBinary = NULL;
   recSizeInBytes = 0;
   initWinsock();
   initSocket();
}
Client::~Client(){
   if (recBinary){
      delete[] recBinary;
   }
   recBinary = NULL;
}

//OLD NON WORKING CODE
//void Client::start(){
//   int nDataLength2 = recv(Socket, (char*)(&recData1), sizeof(_Uint32t), 0);
//   int nDataLength1 = recv(Socket, recData2, DATA_SIZE, 0);
//   close();
//}

//CODE SUPPLIED BY ED
void Client::start(){
   int nDataLength2 = recv(Socket, (char*)(&recSizeInBytes), sizeof(uint32_t), 0);
   recBinary = new char[recSizeInBytes];
   int nDataLength1 = recv(Socket, recBinary, recSizeInBytes, 0);



   //RECEIVE EXTENSION LENGTH
   uint32_t recLength;
   int nDataLength3 = recv(Socket, (char*)(&recLength), sizeof(uint32_t), 0);
   extensionLength = ntohl(recLength);

   //receive actual extension
   char* temp = new char[extensionLength];
   recv(Socket, temp, strlen(temp), 0);

   //std::vector<char> eVec;
   for (int i = 0; i < extensionLength; i++){
      eVec.push_back(temp[i]);
   }

   char* fileExtension = new char[eVec.size()];
   for (int i = 0; i < eVec.size(); i++){
      std::cout << eVec[i];
   }

   close();

   //free up allocated memory
   delete[] temp;
   delete[] fileExtension;

}

void Client::close(){
   shutdown(Socket, SD_SEND);
   closesocket(Socket);
   WSACleanup();
}

int Client::initWinsock(){
   if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0){
      std::cout << "Winsock initialization failed!\n";
      WSACleanup();
      return 1;
   }
   return 0;
}

int Client::initSocket(){

   Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (Socket == INVALID_SOCKET){
      std::cout << "Socket creation failed!\n";
      WSACleanup();
      return 1;
   }

   std::cout << "Please enter server's hostname or IP address: ";
   std::string s;
   std::cin >> s;
   char const* hostName = s.c_str();
   if ((host = gethostbyname(hostName)) == NULL){
      std::cout << "Failed to resolve hostname!\n";
      WSACleanup();
      return 2;
   }

   std::cout << "Please enter server's port number: ";
   int portNum;
   std::cin >> portNum;
   SockAddress.sin_port = htons(portNum);
   SockAddress.sin_family = AF_INET;
   SockAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr);

   if (connect(Socket, (SOCKADDR*)(&SockAddress), sizeof(SockAddress)) != 0){
      std::cout << "Failed to connect with server!\n";
      WSACleanup();
      return 3;
   }

   return 0;
}

char* Client::getBinary(){
   return recBinary;
}

uint32_t Client::getSizeInBytes(){
   return recSizeInBytes;
}

std::vector<char> Client::getExtension(){
   return eVec;
}