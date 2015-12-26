#pragma once
#include <string>
#include <fstream>
#include <stdint.h>
#include <vector>

#define CHUNK_BYTES 1024;

class FileCopier{
private:

   std::string iFileName;//input file name
   std::string oFileName;//output file name
   std::ifstream iFile;//input file
   std::ofstream oFile;//output file
   uint32_t chunkSize;//size of chunks to use for copying (size in bytes)
   uint32_t fileSize;//size of file in bytes
   uint32_t numChunks;//total number of chunks
   bool oneChunk;//true if only need 1 chunk
   char* currentChunk;//will hold the bytes of the current chunk
public:
   FileCopier();
   virtual ~FileCopier();

   void setIFileName();

   void setOFileName(std::vector<char> extension);
   void setOFileName(char* extension, std::string fName);

   void findFileSize();
   void findNumChunks();
   void doWork();

   uint32_t getSize();
   uint32_t getNumChunks();
   bool getOneChunk();
   const char* getExtension();
   uint32_t getChunkSize();

   void readChunk(char* chunk, uint32_t chunkSize);
   void writeChunk(char* chunk, uint32_t chunkSize);

};