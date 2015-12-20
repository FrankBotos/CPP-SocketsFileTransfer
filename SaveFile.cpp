#include <iostream>
#include "SaveFile.h"

SaveFile::SaveFile(){//default constructor
   cout << "Please enter file directory: ";
   cin >> fileName;

   //ios::ate means that the get pointer will be positioned at end of the file
   ifstream file(fileName, ios::in|ios::binary|ios::ate);

   if (file.is_open()){
         fileSize = file.tellg();//because get pointer is at end of file, tellg will return size of file in question
         memBlock = new char[fileSize];//allocating memory for the file

         file.seekg(0, ios::beg);//return get position to beginning of file
         file.read(memBlock, fileSize);//read file into memBlock variable
         file.close();//close file

         //default destructor will take care of cleanup
   }

}

SaveFile::~SaveFile(){}//default destructor

streampos SaveFile::getSize(){
   return (_Uint32t)fileSize;
}

char* SaveFile::getMemBlock(){
   return memBlock;
}

const char* SaveFile::getExtension(){//extracts and returns extension of file in question
   int indexOf = fileName.find_last_of(".");
   string temp;

   for (int i = indexOf; i < strlen(fileName.c_str()); i++){
      temp.push_back(fileName[i]);
   }

   const char* c = temp.c_str();

   char* ret = new char[strlen(c)];
   strcpy(ret, c);
   return ret;

}
