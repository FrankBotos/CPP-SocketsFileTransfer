#pragma once
#include <fstream>
#include <string>
#include <vector>

class MakeFile{
public:
   MakeFile(_Uint32t sizeInBytes, char* binaryData, std::vector<char> extension);
   ~MakeFile();
};