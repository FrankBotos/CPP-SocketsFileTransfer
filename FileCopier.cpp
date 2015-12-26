#include "FileCopier.h"
#include <iostream>
using namespace std;

FileCopier::FileCopier(){
   //initialize chunk size
   chunkSize = CHUNK_BYTES;
   oneChunk = true;

   /*
   ALGORITHM:
   1.get fileNames
   2.define chunk size
   3.find file size
   4.find # of chunks needed
   5.save a chunk, write that chunk, repeat until all data is written
   */
   //doWork();

}
FileCopier::~FileCopier(){}

void FileCopier::setIFileName(){
   //get input file name
   cout << "File name: ";
   cin >> iFileName;
}

void FileCopier::setOFileName(std::vector<char> extension){
   //get input file name
   cout << "File name: ";
   cin >> oFileName;
   for (int i = 0; i < extension.size(); i++){
      oFileName.push_back(extension[i]);
   }
   oFile = ofstream(oFileName, ios::out | ios::binary);//we also initialize the file here
}

void FileCopier::setOFileName(char* extension, std::string fName){
   oFileName = fName + extension;
   oFile = ofstream(oFileName, ios::out | ios::binary);//we also initialize the file here
}



void FileCopier::findFileSize(){
   iFile = ifstream(iFileName, ios::in | ios::binary | ios::ate);//ios ate sets position get pointer to end of file
   if (iFile.is_open()){
      fileSize = iFile.tellg();//because get pointer is at end of file, this will reveal file size in bytes
      iFile.seekg(0, ios::beg);//resets file pointer to beginning
   }
}

void FileCopier::findNumChunks(){
   if (fileSize > chunkSize){
      oneChunk = false;
      numChunks = fileSize / chunkSize;
   }
   else {
      numChunks = 1;
   }
}

void FileCopier::doWork(){//once all data is avaialble, actually copy file
   oFile = ofstream(oFileName, ios::out | ios::binary);

   if (oneChunk){//if entire file fits into single chunk, this block executes
      char* currentChunk = new char[fileSize];
      iFile.read(currentChunk, fileSize);
      oFile.write(currentChunk, fileSize);
      delete[] currentChunk;
   }
   else {//if more than one chunks needed, this block executes + makes sure last chunk is correct size
      uint32_t chunksWritten = 0;
      while (chunksWritten <= numChunks){

         if (chunksWritten != numChunks){
            currentChunk = new char[chunkSize];
            iFile.read(currentChunk, chunkSize);
            oFile.write(currentChunk, chunkSize);
            delete[] currentChunk;
         }
         else {//this block will execute on final chunk!
            uint32_t lastChunkSize = fileSize - (chunkSize * numChunks);
            currentChunk = new char[lastChunkSize];
            iFile.read(currentChunk, lastChunkSize);
            oFile.write(currentChunk, lastChunkSize);
            delete[] currentChunk;
         }
         cout << "chunks: " << chunksWritten << "/" << numChunks << "\n";
         chunksWritten++;
      }
   }
}

uint32_t FileCopier::getSize(){
   return fileSize;
}

const char* FileCopier::getExtension(){//extracts and returns the file type
   int indexOf = iFileName.find_last_of(".");
   string temp;

   for (int i = indexOf; i < strlen(iFileName.c_str()); i++){
      temp.push_back(iFileName[i]);
   }

   const char* c = temp.c_str();

   char* ret = new char[strlen(c)];
   strcpy(ret, c);
   return ret;
}

uint32_t FileCopier::getNumChunks(){
   return numChunks;
}

bool FileCopier::getOneChunk(){
   return oneChunk;
}

uint32_t FileCopier::getChunkSize(){
   uint32_t ret = CHUNK_BYTES;
   return ret;
}

void FileCopier::readChunk(char* chunk, uint32_t chunkSize){
   iFile.read(chunk, chunkSize);
}
void FileCopier::writeChunk(char* chunk, uint32_t chunkSize){
   oFile.write(chunk, chunkSize);
}