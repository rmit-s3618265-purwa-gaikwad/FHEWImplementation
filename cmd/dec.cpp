#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "../LWE.h"
#include "common.h"
#include <cassert>
#include <string>
#include<sstream>
#include "variables.h"
using namespace std;
char *sk_fn, *ct_fn;
std::stringstream binaryStr;
void help(char* cmd) {
  cerr << "\nusage: " << cmd << " SecretKeyFileName CipherTextFileName  \n\n" 
       << "  Decrypt the CipherText under some SecretKey and print it on the std output.\n\n";
  exit(0);
}
int main(int argc, char *argv[]) {
  if (argc != 4) help(argv[0]);
  sk_fn = argv[1]; 
  ct_fn = argv[2]; 
  char* invert = argv[3];
  bool addOne = false;
  FHEW::Setup();
  int arraySize = getValueFromFile("size.txt");
  if(*invert == 'a' || *invert == 't' || *invert == 'f')
  {
	arraySize = arraySize + 1;
  }
  else
  {
	arraySize = arraySize * 2;
  }
  long dec = 0, rem, base = 1;
  
  LWE::SecretKey* SK = LoadSecretKey(sk_fn);
  LWE::CipherText* ct = LoadCipherText(ct_fn, arraySize, true);
  LWE::CipherText* temp;
  LWE::CipherText** ctAns = new LWE::CipherText*[arraySize];

 for(int i=0; i < arraySize;i++)
  {
        temp = get(i, true);
        ctAns[i] = new LWE::CipherText;
        ctAns[i] = temp;
  	if(*invert == 't' && LWE::Decrypt(*SK,*ctAns[0]) == 1)
	{
		if(i == 0)
			continue;
		addOne = true;
		if(LWE::Decrypt(*SK,*ctAns[i]) == 1)
		{
			binaryStr << 0;
		}
		else
		{
			binaryStr << 1;
		}
	}
	else
	{
		if(i == 0 && *invert == 'f')
			continue;
        	binaryStr << LWE::Decrypt(*SK,*ctAns[i]);
  	}
  }
  int binaryNum;
  binaryStr >> binaryNum;
       while (binaryNum > 0)
     {
         rem = binaryNum % 10;
         dec = dec + rem * base;
         base = base * 2;
         binaryNum = binaryNum / 10;
     }
  cout << "The number is : ";
  if(addOne)
  {
        dec = dec + 1;
	cout << "-";
  }
	 cout<<dec;
}
