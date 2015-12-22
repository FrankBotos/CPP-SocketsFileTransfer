#include <iostream>
#include "Server.h"
#include "Client.h"
#include "SaveFile.h"
#include "MakeFile.h"

int main(){

   char whatType;
   bool validInput = false;
   cout << "***************************************\n";
   cout << "*                                     *\n";
   cout << "*      What would you like to do?     *\n";
   cout << "*                                     *\n";
   cout << "*      (R)eceive a file               *\n";
   cout << "*      (S)erve a file                 *\n";
   cout << "*      (H)elp                         *\n";
   cout << "*      (E)xit                         *\n";
   cout << "*                                     *\n";
   cout << "*                                     *\n";
   cout << "***************************************\n";
   
   cin >> whatType;

   if (whatType == 'r' || whatType == 'R'){//RUN CLIENT CODE
      Client c;
      c.start();
      MakeFile f(c.getSizeInBytes(), c.getBinary(), c.getExtension());//pass necessary data to file creation factory, which recreates received file
   }
   else if (whatType == 's' || whatType == 'S'){//RUN SERVER CODE
      SaveFile f1;//class that saves a file in RAM
      Server().start(f1);//serving up file for any available connection
   }
   else if (whatType == 'h' || whatType == 'H'){
      std::cout << "TO BE IMPLEMENTED\n";
   }

   int x;
   std::cout << "enter 1 to exit...";
   std::cin >> x;
   return 0;
}
