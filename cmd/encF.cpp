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
void convertToIEEE754(double num);
char *ct_fn;
LWE::SecretKey* SK;
int main(int argc, char *argv[]) {
  if (argc != 4) help(argv[0]);
  double message = atof(argv[1]);
  char* sk_fn = argv[2]; 
  ct_fn = argv[3]; 
  int i=0, temp;
  SK = LoadSecretKey(sk_fn);
  convertToIEEE754(message);
}
void convertToIEEE754(double num)
{
	for(int j = 0; j < 32; j++)
		IEEE754[j] = 0;
	int integerPart = num, intTemp[32], index = 0, mantissa;
	double decimalPart = num - integerPart;
	int deciTemp[32];
	int i = 0;
	IEEE754[i] = 0;
	i++;
	while(integerPart != 0)
  	{
        	intTemp[i++] = integerPart%2;
        	integerPart /= 2;
  	}
	i--;
	while(i>0)
	{
		binaryArray[index++] = intTemp[i--]; 
	}
	mantissa = index-1;
	for(int j=0; j < 23-index; j++)
	{
		decimalPart = decimalPart * 2;
		deciTemp[j] = decimalPart; 
		if(decimalPart >= 1)
			decimalPart = decimalPart - 1;
		binaryArray[index + j] = deciTemp[j];
	}
	mantissa = mantissa + 127;
	i = 8;
	while(mantissa != 0)
        {
                IEEE754[i--] = mantissa%2;
                mantissa /= 2;
        }
	for(int j=1; j<23; j++)
	{
		IEEE754[8+j] = binaryArray[j];
	}
	LWE::CipherText ct[32];
	for(int j = 0; j < 32; j++)
	{
		LWE::Encrypt(&ct[j], *SK, IEEE754[j]);      
        	SaveCipherText(&ct[j],ct_fn);
	}
}
