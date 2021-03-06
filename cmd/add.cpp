#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"
#include "variables.h"
#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

FHEW::EvalKey* EK;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " EvalKeyFileName InCTFileName1 InCTFileName2 OutCTFileName  \n\n" 
       << "  Perform Homomorphic NAND computation.\n\n";
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 5) help(argv[0]);
  char* ek_fn = argv[1]; 
  char* ict1_fn = argv[2]; 
  char* ict2_fn = argv[3]; 
  char* oct_fn = argv[4]; 
  
  int arraySize = getValueFromFile("size.txt");
  FHEW::Setup();

  EK = LoadEvalKey(ek_fn);

  LWE::CipherText *ct3,*ct4, *ct5, *ct6, *ct7, *ctIn, *ct8, *ct9, *ct10, *ct11, *ctOut;
  LoadCipherText(ict1_fn, arraySize, true);
  LWE::CipherText **ct1 = new LWE::CipherText*[arraySize];
  if(arraySize == 1)
	ct1[0] = LoadCipherText(ict1_fn, arraySize, true);
  else
  {
	for(int i=0;i<arraySize;i++)
  	{
 		 ct1[i] = get(i, true);
  	}
  }
  LoadCipherText(ict2_fn, arraySize, false);
  LWE::CipherText **ct2 = new LWE::CipherText*[arraySize];
  if(arraySize == 1)
	ct2[0] = LoadCipherText(ict2_fn, arraySize, true);
  else
  {
  	for(int i=0;i<arraySize;i++)
  	{     
        	 ct2[i] = get(i, false);
  	}
  }
  ct3 = new LWE::CipherText;
  ct4 = new LWE::CipherText;
  ct5 = new LWE::CipherText;
  ct6 = new LWE::CipherText;
  ct7 = new LWE::CipherText;
  ct8 = new LWE::CipherText;
  ct9 = new LWE::CipherText;
  ct10 = new LWE::CipherText;
  ct11 = new LWE::CipherText; 
  ctOut = new LWE::CipherText;
  ctIn = LoadCipherText("0.ct", 1, true);
  LWE::CipherText **temp = new LWE::CipherText*[arraySize];
  for(int i=arraySize-1; i>=0;i--)
  {
 	FHEW::HomNAND(ct3, *EK,*ct1[i],*ct2[i]);	
  	FHEW::HomNAND(ct4, *EK,*ct1[i],*ct3);
  	FHEW::HomNAND(ct5, *EK,*ct3,*ct2[i]);	
  	FHEW::HomNAND(ct6, *EK,*ct4,*ct5);		
  	FHEW::HomNAND(ct7, *EK,*ct6,*ctIn);	
  	FHEW::HomNAND(ct8, *EK,*ct6,*ct7);	
  	FHEW::HomNAND(ct9, *EK,*ct7,*ctIn);	
  	ct10 = new LWE::CipherText;
  	FHEW::HomNAND(ct10, *EK,*ct8,*ct9);	
  	temp[i] = ct10;
  	FHEW::HomNAND(ctOut, *EK,*ct7,*ct3);	
	ctIn = ctOut;
  }
  SaveCipherText(ctOut, oct_fn);

  for(int i=0;i<arraySize;i++)
  {
  	SaveCipherText(temp[i], oct_fn);
  }

}
