#include "Client.h"
#include <iostream>
#include <string>
#include "FileCopier.h"
#include <chrono>
#include <iomanip>

Client::Client(){
   recSizeInBytes = 0;
   initWinsock();
   initSocket();
   start();
}
Client::~Client(){}

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

   std::cout << "Please enter new filename: ";
   std::cin >> newFileName;

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

   std::cout << "\n\nSuccessfuly connected to server at: " << hostName << " : " << portNum << "\n";

   return 0;
}

void Client::start(){
   receiveData();
   close();
}
void Client::close(){
   shutdown(Socket, SD_SEND);
   closesocket(Socket);
   WSACleanup();
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
   2. receive file extension length
   3. receive actual extension as c-style string
   4. receive actual binary data from file
   */

   //1. receive size of file in bytes
   uint32_t fileSize = 0;
   uint32_t bytesReceived = 0;

   std::cout << "Receiving filesize... ";
   while (bytesReceived != sizeof(fileSize)){
      bytesReceived = recv(Socket, (char*)(&fileSize), sizeof(fileSize), 0);
      if (bytesReceived == sizeof(fileSize)){
         std::cout << "--->Filesize successfully received!\n";
      }
   }

   //2. receive file extension length
   uint32_t extensionLength = 0;
   bytesReceived = 0;//reset variable for use with next recv

   std::cout << "Receiving file extension length... ";
   while (bytesReceived != sizeof(fileSize)){
      bytesReceived = recv(Socket, (char*)(&extensionLength), sizeof(extensionLength), 0);
      if (bytesReceived == sizeof(extensionLength)){
         std::cout << "--->File extension length successfully received!\n";
      }
   }

   //3. receive actual extension as c-style string
   char* extension = new char[extensionLength];
   bytesReceived = 0;

   std::cout << "Receiving file extension... ";
   while (bytesReceived != sizeof(extension)){
      bytesReceived = recv(Socket, extension, sizeof(extension), 0);
      if (bytesReceived == sizeof(extension)){
         std::cout << "--->File extension successfully received!\n";
         extension[extensionLength] = '\0';//null terminating string if extension was successfully received
      }
   }

   //4. receive actual data in chunks
   FileCopier f;
   f.setOFileName(extension, newFileName);

		//setting up variables for a basic timer to keep track of megabytes per second
		auto start = std::chrono::high_resolution_clock::now();
		auto end = std::chrono::high_resolution_clock::now();
		uint32_t currentTime = 0;
		uint32_t currentBytes = 0;
		uint32_t oldBytes = 0;
		double MBPS;//will hold final result (megabytes per second)

   //receive one chunk at a time, and write to file
   uint32_t chunksReceived = 0;
   char* currentChunk = new char[f.getChunkSize()];
   uint32_t lastChunkSize = fileSize - (f.getChunkSize() * (fileSize / f.getChunkSize()));
   uint32_t allBytesRec = 0;

   while (chunksReceived <= (fileSize / f.getChunkSize())){
      uint32_t numBytesReceived = 0;
      uint32_t n;

      while (numBytesReceived < f.getChunkSize()){
         n = recv(Socket, currentChunk + numBytesReceived, f.getChunkSize(), 0);
         if (n > 0 && n <= f.getChunkSize()){
            numBytesReceived += n;
			allBytesRec += n;
         }
      }

      if (chunksReceived != (fileSize / f.getChunkSize())){
         f.writeChunk(currentChunk, numBytesReceived);
      }
      else {
         f.writeChunk(currentChunk, lastChunkSize);
      }
      
	  //if we get to this point, the complete chunk has been received and written, we can increment to next one!
      chunksReceived++;

		//update timer!
		end = std::chrono::high_resolution_clock::now();
		auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
		//this block will save the amount of bytes downloaded since the last second into "oldBytes"
		if (currentTime != (int)timePassed.count()) {
			  currentTime = (int)timePassed.count();
			  oldBytes = currentBytes;
			  currentBytes = 0;
		}
		else {
			  currentBytes += n;
		}
		MBPS = (double)oldBytes / (double)1000000;//find a ROUGHLY accurate estimation of MBPS

	  //output current status of file transfer
	  std::cout << "\rReceiving file..." << allBytesRec / 1000000 << "/" << fileSize / 1000000 << "MB" << "(" << std::fixed << std::setprecision(2) << MBPS << " MB/s)";

   }
   delete[] currentChunk;
   std::cout << "--->File successfully received!\n";

}