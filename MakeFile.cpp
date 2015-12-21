#include <iostream>
#include "MakeFile.h"
#include <vector>

//on the serverside, we use a streampos variable to represent the size of the file in bytes
//however, it is actually possible to reconstruct the file by using a _Uint32t, without the need for converting back to streampos, and so we skip that step
MakeFile::MakeFile(uint32_t sizeInBytes, char* binaryData, std::vector<char> extension){
   std::string fileName;
   std::cout << "Please enter new file name: ";
   std::cin >> fileName;

   std::string temp = "";
   for (int i = 0; i < extension.size(); i++){
      temp.push_back(extension[i]);
   }

   std::cout << "NEW FILE: " << fileName + temp;

   std::ofstream file(fileName + temp, std::ios::out | std::ios::binary);
   file.write(binaryData, sizeInBytes);
   file.close();
}
MakeFile::~MakeFile(){}