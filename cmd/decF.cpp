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
void decrypt(int arraySize)
{
LWE::SecretKey* SK = LoadSecretKey(sk_fn);
LWE::CipherText* ct = LoadCipherText(ct_fn, arraySize, true);
LWE::CipherText* temp;
  LWE::CipherText** ctAns = new LWE::CipherText*[arraySize];
long dec = 0, rem, base = 1;
int exponent;
int beforeDecimal;
double afterDecimal;
  // load entire number
  for(int i=0; i< arraySize; i++)
  {
	temp = get(i, true);
	ctAns[i] = new LWE::CipherText;
        ctAns[i] = temp;
	cout << LWE::Decrypt(*SK,*ctAns[i]);
  }
  cout << endl;
// getting expo
  for(int i=1; i < 9;i++)
  {
        binaryStr << LWE::Decrypt(*SK,*ctAns[i]);
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
  dec = dec - 127;
  exponent = dec + 1;
  //getting the number before decimal
  std::stringstream binaryStr2;
  binaryStr2 << 1;
	for(int i=9; i< 9+exponent; i++)
	{
		binaryStr2 << LWE::Decrypt(*SK,*ctAns[i]);
	}
	int decimalNum;
	binaryStr2 >> decimalNum;
dec = 0; 
base = 1;
	 while (decimalNum > 0)
         {
		rem = decimalNum % 10;
         	dec = dec + rem * base;
         	base = base * 2;
         	decimalNum = decimalNum / 10;
     }
	beforeDecimal = dec;
    int j = 1;
    double Ans = 0;
    for(int i = 9+exponent; i < 32; i++)
    {
 	int ansDec = LWE::Decrypt(*SK,*ctAns[i]);
	if(ansDec == 1)
	{
		Ans = Ans + pow(2, -1 * j);
	}
	j++;
    }
    afterDecimal = Ans;
	Ans = afterDecimal + beforeDecimal;
    cout << "Answer: " << Ans << endl;
}
int main(int argc, char *argv[]) {
  if (argc != 3) help(argv[0]);
  sk_fn = argv[1]; 
  ct_fn = argv[2]; 
  FHEW::Setup();
  int arraySize = 32;
  decrypt(arraySize);
}
