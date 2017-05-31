#include <iostream>
#include <cstdlib>
#include "../LWE.h"
#include "../FHEW.h"
#include "common.h"
#include <cassert>
#include "variables.h"
using namespace std;
void help(char* cmd) {
  cerr << "\nusage: " << cmd << " Message SecretKeyFileName CipherTextFileName  \n\n" 
       << "  Encrypt the Message under some SecretKey and store it in a File.\n\n";
  exit(0);
}
int IEEE754[32];
int binaryArray[64];
char *ct_fn;
LWE::SecretKey* SK;
int main(int argc, char *argv[]) {
  if (argc != 4) help(argv[0]);
  int message = atoi(argv[1]);
  char* sk_fn = argv[2]; 
  ct_fn = argv[3]; 
  int i=0, temp;
  SK = LoadSecretKey(sk_fn);
  if(message == 0)
  {
	binaryArray[i] = message;
	i++;  
  }
  else
  { 
  while(message != 0)
  {
	temp = message%2;
	binaryArray[i++] = temp;
	message /= 2;
  }
 }
  LWE::CipherText ct[i];
  if(i > 0)
  {
      setValueToFile(i, "size.txt");
  }
  for(int j=i-1;j>=0;j--)
  {
	  if (!((binaryArray[j] ==0)||(binaryArray[j] ==1))){
		  cerr << " The message must be 0 or 1.\n";
	  exit(0);
	  }


	LWE::Encrypt(&ct[j], *SK, binaryArray[j]); 	
	SaveCipherText(&ct[j],ct_fn);
  }
}
