#include <iostream>
#include "Server.h"
#include "Client.h"
using namespace std;

int main(){

	if (htonl(47) == 47) {
		// Big endian
		cout << "BIG ENDIAN\n\n";
	}
	else {
		// Little endian.
		cout << "LITTLE ENDIAN\n\n";
	}

   std::cout << "Press 1 for client, 2 for server!";
   int x;
   cin >> x;
   if (x == 1){
      Client c;
   }
   else {
      Server s;
   }

   int xasdf = 0;
   cin >> xasdf;
   return 0;
}