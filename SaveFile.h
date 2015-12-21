#pragma once

#include <fstream>
#include <string>
using namespace std;

class SaveFile{
private:
   streampos fileSize;//streampos data type holds current position of the buffer pointer or file pointer
   char* memBlock;
   string fileName;
   string fileType;//this stores the file extension (eg, if the file being saved is a .jpg, this variable will contain .jpg)
public:
   SaveFile();
   virtual ~SaveFile();

   streampos getSize();
   char* getMemBlock();
   const char* getExtension();

};
