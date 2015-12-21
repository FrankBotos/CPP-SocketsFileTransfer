#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

class MakeFile{
public:
   MakeFile(uint32_t sizeInBytes, char* binaryData, std::vector<char> extension);
   virtual ~MakeFile();
};