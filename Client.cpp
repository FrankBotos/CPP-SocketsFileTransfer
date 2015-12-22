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
   receiveData();
   close();
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

void Client::receiveData(){
   /* STEPS
   1. receive size of file in bytes
   2. receive file extension length,
   3. then receive actual extension
   4. while loop to receive entire file
   */

   //1.Recieive file size in bytes
   recv(Socket, (char*)(&recSizeInBytes), sizeof(uint32_t), 0);

   //2.Receive file extension length
   uint32_t recLength;
   recv(Socket, (char*)(&recLength), sizeof(uint32_t), 0);
   extensionLength = ntohl(recLength);

   //3.Receive actual extension
   char* temp = new char[extensionLength];
   recv(Socket, temp, strlen(temp), 0);
   for (int i = 0; i < extensionLength; i++){
      eVec.push_back(temp[i]);
   }
   char* fileExtension = new char[eVec.size()];
   for (int i = 0; i < eVec.size(); i++){
      std::cout << eVec[i];
   }

   //4.a while loop to receive data!
   recBinary = new char[recSizeInBytes];
   uint32_t totalBytesReceived = 0;
   uint32_t n = 0;

   while (totalBytesReceived < recSizeInBytes){
      n = recv(Socket, recBinary + totalBytesReceived, recSizeInBytes, 0);
      if (n == -1) { break; }
      totalBytesReceived += n;
      std::cout << "\r" << totalBytesReceived / 1000000 << "/" << recSizeInBytes / 1000000 << " MB received.";//divide by a million to find MB value
   }
   std::cout << "\r" << totalBytesReceived / 1000000 << "/" << recSizeInBytes / 1000000 << " MB received.";


   //finally, clean up dynamic memory
   delete[] temp;
   delete[] fileExtension;

}